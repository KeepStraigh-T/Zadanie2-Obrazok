#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
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
	if(m == 0 || n == 0)
		return image;

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
		return image;

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
		
	// off_t file_pointer = lseek(file_desc, 0, SEEK_SET);
	// if(file_pointer == -1)
	// {
	// 	close(file_desc);
	// 	return NULL;
	// }

	unsigned char buf[256];
	int idx = 0, width = 0, height = 0, max_value = 0;

	while(read(file_desc, &buf[idx], 1) == 1 && buf[idx] != '\n' && idx < 2)
		idx++;
	buf[idx] = '\0';
	if(buf[0] != 'P' || buf[1] != '5')
	{
		close(file_desc);
		return NULL;
	}

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
		while(read(file_desc, &c, 1) == 1 && c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '#')
			buf[idx++] = c;

		buf[idx] = '\0';

		int val = atoi(buf);
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
			max_value = val;

		if(c == '#')
			while(read(file_desc, &c, 1) == 1 && c != '\n');

		n++;
	}

	if(max_value > 255)
	{
		close(file_desc);
		perror("Pixel is not 1 byte size");
		return NULL;
	}	

	GSI* image = gsi_create_with_geometry(width, height);
	if(image == NULL)
	{
		close(file_desc);
		return NULL;
	}

	ssize_t expected = width * height;
	ssize_t got = read(file_desc, image->px, expected);
	close(file_desc);

	if(got != expected)
	{
		free(image->px);
		free(image);
		return NULL;
	}

	return image;
}