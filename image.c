#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "image.h"


GSI* gsi_create_empty(void)
{
	GSI* image = malloc(sizeof(GSI));
	if(image == NULL)
		return NULL;
	
	image->width = 0;
	image->height = 0;
	image->px = NULL;

	return image;
}

GSI* gsi_create_with_geometry(unsigned int m, unsigned int n)
{
	GSI* image = gsi_create_empty();
	if(image == NULL)
		return NULL;
	// Return zero-sized image
	else if(m == 0 || n == 0)
		return NULL;

	image->width = m;
	image->height = n;
	image->px = calloc(m * n, sizeof(unsigned char));
	if(image->px == NULL)
	{
		free(image);
		return NULL;
	}

	return image;
}

GSI* gsi_create_with_geometry_and_color(unsigned int m, unsigned int n, unsigned char color)
{
	GSI* image = gsi_create_with_geometry(m, n);
	if(image == NULL)
		return NULL;
	// Return zero-sized image
	if(m == 0 || n == 0)
		return NULL;

	for(unsigned int i = 0; i < (m * n); i++)
		image->px[i] = color;

	return image;
}

GSI* gsi_create_by_pgm5(char* file_name)
{
	if(file_name == NULL)
		return NULL;
	
	int file_desc = open(file_name, O_BINARY | O_RDONLY);
	if(file_desc < 0)
		return NULL;

	unsigned char buf[256];
	int idx = 0, width = 0, height = 0, max_value = 0;

	// Check for PGMB (P5) format
	while(read(file_desc, &buf[idx], 1) == 1 && buf[idx] != '\n' && idx < 2)
		idx++;
	buf[idx] = '\0';
	if(buf[0] != 'P' || buf[1] != '5')
	{
		close(file_desc);
		return NULL;
	}

	//Check for width, height and maxvalue
	int n = 0;
	while(n < 3)
	{
		char c;
		idx = 0;

		// Skip whitespaces
		while(read(file_desc, &c, 1) == 1 && (c == ' ' || c == '\n' || c == '\t' || c == '\r'));

		if(c == '#')
		{
			while(read(file_desc, &c, 1) == 1 && c != '\n');
			continue;
		}
		// First non-whitespace or non-'#' character
		buf[idx++] = c;
		// Read next non-whitespace or non-'#' characters
		while(read(file_desc, &c, 1) == 1 && c != ' ' && c != '\n'
												&& c != '\t' && c != '\r' && c != '#')
			buf[idx++] = c;

		buf[idx] = '\0';

		int val = atoi((const char*)buf);
		if(val <= 0)
		{
			close(file_desc);
			return NULL;
		}

		if(n == 0)
			width = val;
		else if(n == 1)
			height = val;
		else
		{
			max_value = val;

			if(c != '\n')
				while(read(file_desc, &c, 1) == 1 && c != '\n');
		}
		n++;
	}

	if(max_value > 255)
	{
		close(file_desc);
		printf("Pixel is not 1 byte size\n");
		return NULL;
	}

	GSI* image = gsi_create_with_geometry(width, height);
	if(image == NULL)
	{
		close(file_desc);
		return NULL;
	}

	image->mxvalue = max_value;

	ssize_t expected = width * height;
	ssize_t got = read(file_desc, image->px, expected);
	close(file_desc);

	if(got != expected)
	{
		gsi_destroy(image);
		return NULL;
	}

	return image;
}

char gsi_save_as_pgm5(GSI *img, char *file_name, char *comment)
{
	if(!img)
		return IMG_FAIL;
	else if(!(img->px))
		return IMG_FAIL;
	else if(!file_name)
		return FN_FAIL;
	else if(img->height <= 0 || img->width <= 0 || img->mxvalue <= 0)
		return IMG_FAIL;

	// Create new file to write data to
	int file_desc = open(file_name, O_BINARY | O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR | S_IRUSR);
	// printf("file desc = %d\n", file_desc);
	if(file_desc < 0)
		return IMG_CREATE_FAIL;

	// size_t comment_length = strlen(comment);
	// int w1 = write(file_desc, "P5\n", 3);
	// perror("w1: ");
	// printf("errno: %d\n", errno);
	// int w2 = write(file_desc, "# ", 2);
	// int w3 = write(file_desc, comment, comment_length);
	// int w4 = write(file_desc, "\n", 1);
	// printf("w1: %d w2: %d w3: %d w4: %d\n", w1, w2, w3, w4);

		char buf[128] = {0};
		snprintf(buf, sizeof(buf),"P5%s%s\n%u %u\n%u\n",
						comment ? "\n# " : "",
						comment ? comment : "",
						img->width, img->height, img->mxvalue);

		size_t header_len = strlen(buf);
		if(write(file_desc, buf, header_len) != header_len)
		{
			close(file_desc);
			return -100;
		}
	
	int pixels_amount = img->width * img->height;
	if(write(file_desc, img->px, pixels_amount) != pixels_amount)
	{
		close(file_desc);
		return W_FAIL;
	}

	close(file_desc);
	return SAVE_OK;
}

void gsi_destroy(GSI *img)
{
	free(img->px);
	free(img);
}
