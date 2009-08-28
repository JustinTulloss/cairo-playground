/* Copyright (c) 2009 Justin Tulloss
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Large swaths of this code borrowed from:
 * http://zetcode.com/tutorials/cairographicstutorial
 *
 * Thanks!
 */

#include <stdio.h>
#include <math.h>

#include <cairo.h>

#define LEFT_BUTTON 1
#define RIGHT_BUTTON 3

static cairo_surface_t *image;

static int xc = 85, yc = 85;
static int width = 0, height = 0;
static int xshift = 1, yshift = 1;

static int
draw(cairo_surface_t *surface)
{
    cairo_t *cr;
    cr = cairo_create(surface);

    /* Now we have a clear context, draw our circle */
    xc += xshift;
    yc += yshift;
    cairo_arc(cr, xc, yc, 10, 0, 2*M_PI);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr, 4);
    /* We now have an outline, but we aren't done with the arc since we want
     * to fill it in. So, we preserve the stroke, then change things for the
     * fill.
     */
    cairo_stroke_preserve(cr);
    cairo_set_source_rgba(cr, .8, 0.2, 0.1, .7);
    cairo_fill(cr);

    /* Put our text under the bouncy ball */
    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_select_font_face(cr, "Helvetica",
            CAIRO_FONT_SLANT_NORMAL,
            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 24);
    cairo_move_to(cr, width - 400, 50);
    cairo_show_text(cr, "keep your eye on the ball");

    cairo_destroy(cr);

    if (yc <= 0 || yc >= height) {
        yshift = -yshift;
    }
    if (xc <= 0 || xc >= width) {
        xshift = -xshift;
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    image = cairo_image_surface_create_from_png("bg.png");
    width = cairo_image_surface_get_width(image);
    height = cairo_image_surface_get_height(image);

    printf("Starting with cairo version %s\n", cairo_version_string());

    draw(image);

    cairo_surface_write_to_png(image, "output.png");

    cairo_surface_destroy(image);

    return 0;
}
