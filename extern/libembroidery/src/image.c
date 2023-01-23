/*
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * This file contains all the read and write functions for the
 * library.
 *
 *******************************************************************
 *
 * This backends to the stb libraries and nanosvg library.
 *
 * Use Python PEP7 for coding style.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "embroidery.h"


/* for the PES embedded */
void
writeImage(FILE* file, unsigned char image[][48])
{
    int i, j;

    if (!file) {
        printf("ERROR: format-pec.c writeImage(), file argument is null\n");
        return;
    }
    for (i = 0; i < 38; i++) {
        for (j = 0; j < 6; j++) {
            int offset = j * 8;
            unsigned char output = 0;
            output |= (unsigned char)(image[i][offset] != 0);
            output |= (unsigned char)(image[i][offset + 1] != (unsigned char)0) << 1;
            output |= (unsigned char)(image[i][offset + 2] != (unsigned char)0) << 2;
            output |= (unsigned char)(image[i][offset + 3] != (unsigned char)0) << 3;
            output |= (unsigned char)(image[i][offset + 4] != (unsigned char)0) << 4;
            output |= (unsigned char)(image[i][offset + 5] != (unsigned char)0) << 5;
            output |= (unsigned char)(image[i][offset + 6] != (unsigned char)0) << 6;
            output |= (unsigned char)(image[i][offset + 7] != (unsigned char)0) << 7;
            fwrite(&output, 1, 1, file);
        }
    }
}

/*
 *
 */
float image_diff(unsigned char *a, unsigned char *b, int size)
{
    int i;
    float total = 0.0;
    for (i=0; i<size; i++) {
        int diff = a[i] - b[i];
        total += diff*diff;
    }
    return total;
}

#ifdef LIBEMBROIDERY_CLI
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/src/nanosvg.h"

#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvg/src/nanosvgrast.h"


/* Basic Render
 * ------------
 * Backends rendering to nanosvg/stb_image.
 *
 * The caller is responsible for the memory in p.
 */
int
embPattern_render(EmbPattern *p, char *fname)
{
    const char *tmp_fname = "libembroidery_temp.svg";
	NSVGimage *image = NULL;
	NSVGrasterizer rast;
	unsigned char *img_data = NULL;
	embPattern_writeAuto(p, tmp_fname);
	image = nsvgParseFromFile(tmp_fname, "px", 96.0f);
	img_data = malloc(4*image->width*image->height);
	nsvgRasterize(
	    &rast,
	    image,
	    0, 0, 1,
	    img_data,
	    image->width,
	    image->height,
	    4*image->width);
    stbi_write_png(
 	    fname,
 	    image->width,
	    image->height,
	    4,
	    img_data,
	    4*image->width);
    return 0;
}

/* Simulate the stitching of a pattern, using the image for rendering
 * hints about how to represent the pattern.
 */
int
embPattern_simulate(EmbPattern *pattern, char *fname)
{

    embPattern_render(pattern, fname);
    return 0;
}

EmbImage
embImage_create(int width, int height)
{
    EmbImage image;
    image.width = width;
    image.height = height;
	image.data = malloc(4*width*height);
    return image;
    
}

void
embImage_read(EmbImage *image, char *fname)
{
    int channels_in_file;
    image->data = stbi_load(
        fname,
        &(image->width),
        &(image->height),
        &channels_in_file,
        3);
}

int
embImage_write(EmbImage *image, char *fname)
{
    return stbi_write_png(
 	    fname,
 	    image->width,
	    image->height,
	    4,
	    image->data,
	    4*image->width);
}

void
embImage_free(EmbImage *image)
{
    free(image->data);
}
#endif

