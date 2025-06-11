#include <stdio.h>
#include <stdlib.h>
#include "image.h"

char gsi_overlay(GSI *img, GSI *overlay, float opacity)
{
	if(!img || !overlay || img->height == 0 || img->width == 0 ||
		 overlay->height != img->height || overlay->width != img->width)
		return IMG_FAIL;
	else if(img->mxvalue <= 0 || img->mxvalue > 255)
		return IMG_FAIL;
	else if(opacity < 0.0f || opacity > 1.0f)
		return OPACITY_FAIL;

	unsigned char diff = 0;

	for(unsigned int y = 0; y < img->height; y++)
	{
		for(unsigned int x = 0; x < img->width; x++)
		{
			if(PIX(img, x, y) < PIX(overlay, x, y))
			{
				diff = PIX(overlay, x, y) - PIX(img, x, y);
				PIX(img, x, y) += (unsigned char)((float)diff * opacity);
			}
			else if(PIX(img, x, y) > PIX(overlay, x, y))
			{
				diff = PIX(img, x, y) - PIX(overlay, x, y);
				PIX(img, x, y) -= (unsigned char)((float)diff * opacity);
			}
		}
	}

	return OVERLAY_OK;
}

int main(void)
{
	GSI* image = gsi_create_by_pgm5("baboon3.pgm");
	if(!image)
		return IMG_FAIL;

	GSI* overlay = gsi_create_by_pgm5("barbara.pgm");
	if(!overlay)
		return IMG_FAIL;
	
	char overlay_ret = gsi_overlay(image, overlay, 0.9);
	if(overlay_ret != OVERLAY_OK)
		return OVERLAY_FAIL;

	char over = gsi_save_as_pgm5(image, "baboon4.pgm", "Success");
	if(over != SAVE_OK)
		return SAVE_FAIL;
}
