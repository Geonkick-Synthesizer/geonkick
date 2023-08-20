/**
 * File name: rkpng2c.c
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/**
 * rkpng2c is a part of Redkite GUI toolkit.
 * It converts a PNG image to C array encoded in ARGB32 format.
 */

#define RK_VERSION_STR "1.0.0"

#include <stdio.h>
#include <libgen.h>
#include <cairo/cairo.h>

int main(int argc , char **argv)
{
        if (argc != 4) {
                printf("Redkite GUI toolkit\n");
                printf("rkpng2c version %s\n", RK_VERSION_STR);
                printf("Converts a PNG to C array encoded in ARGB32\n");
                printf("Copyright (C) 2019 Iurie Nistor <iur.nistor@gmail.com>\n");
                printf("License GPLv2\n");
                printf("Usage: rkpng2c <PNG file> <C or C++ file> <array name>\n");
                return 0;
        }

        cairo_surface_t *image = cairo_image_surface_create_from_png(argv[1]);
        int w = cairo_image_surface_get_width(image);
        int h = cairo_image_surface_get_height(image);
        printf("image size: %dx%d\n", w, h);

        FILE *fptr;
        fptr = fopen(argv[2], "wb");
        if (fptr == NULL) {
                printf("can't open file %s\n", argv[2]);
                return 1;
        }

        const unsigned char *buff = cairo_image_surface_get_data(image);
        fprintf(fptr, "/**\n"
                      " * Generated with rkpng2c version %s, part of Redkite GUI toolkit.\n"
                      " * File name: %s\n"
                      " * Image size: %dx%d\n"
                      " */\n"
                      "\n"
                "const unsigned char %s[] = {\n", RK_VERSION_STR, basename(argv[1]), w, h, basename(argv[3]));
        for (int i = 0; i < w * h * 4; i++) {
                if ((i + 1) == 12 || (i + 1) % 12 == 0)
                        fprintf(fptr, "0x%02x,\n", buff[i]);
                else
                        fprintf(fptr, "0x%02x, ", buff[i]);
        }
        fprintf(fptr, "};\n");

        fclose(fptr);
        cairo_surface_destroy(image);
        return 0;
}
