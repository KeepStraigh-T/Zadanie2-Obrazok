#ifndef _IMAGE_
#define _IMAGE_

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define PIX(im, x, y) ((im)->px[(y) * (im)->width + (x)])

typedef struct 
{
	unsigned int width;
	unsigned int height;
	unsigned int mxvalue;
	unsigned char *px;
}GSI;

GSI* gsi_create_empty(void);
GSI* gsi_create_with_geometry(unsigned int m, unsigned int n);
GSI* gsi_create_with_geometry_and_color(unsigned int m, unsigned int n, unsigned char color);
GSI* gsi_create_by_pgm5(char *file_name);

#endif