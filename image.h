#ifndef _IMAGE_
#define _IMAGE_

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define PIX(img, x, y) ((img)->px[(y) * (img)->width + (x)])
#define SAVE_OK 1
#define SAVE_FAIL -7
#define OVERLAY_OK 2
#define OVERLAY_FAIL -4
#define IMG_FAIL -1
#define IMG_CREATE_FAIL -2
#define FN_FAIL	-3 					// File name
#define W_FAIL -4						// Write
#define OPACITY_FAIL -5


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
char gsi_save_as_pgm5(GSI *img, char *file_name, char *comment);
void gsi_destroy(GSI *img);
char gsi_overlay(GSI *img, GSI *overlay, float opacity);


#endif