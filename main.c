#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(void)
{
	GSI* image = gsi_create_by_pgm5("baboon.pgm");
	if(image == NULL)
		return -10;

	// printf("%u %u\n%u\n",image->width, image->height, image->mxvalue);
	// printf("%s\n", image->px);
	char return_value = gsi_save_as_pgm5(image, "baboon3.pgm", "Success");

	printf("%d\n", return_value);
}
