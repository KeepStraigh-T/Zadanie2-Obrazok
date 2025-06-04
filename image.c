#include <stdio.h>
#include <stdlib.h>
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

GSI *gsi_create_with_geometry(unsigned int m, unsigned int n)
{
	GSI* image = gsi_create_empty;

	if(image == NULL)
		return NULL;

	image->width = m;
	image->height = n;

	
}
