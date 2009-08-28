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
#include <gtk/gtk.h>

#define LEFT_BUTTON 1
#define RIGHT_BUTTON 3

static cairo_surface_t *image;

static gint xc = 85, yc = 85;
static gint width = 0, height = 0;
static gint xshift = 1, yshift = 1;

static gboolean
on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);

    /* Paint the background image first */
    cairo_set_source_surface(cr, image, 0, 0);
    cairo_paint(cr);

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

    return FALSE;
}

static gboolean
clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    return TRUE;
}

static gboolean
time_handler(GtkWidget *widget) {
    if (widget->window == NULL)
        return FALSE;

    gtk_widget_queue_draw(widget);
    return TRUE;
}

int
main(int argc, char *argv[])
{
    GtkWidget *window;

    image = cairo_image_surface_create_from_png("bg.png");
    width = cairo_image_surface_get_width(image);
    height = cairo_image_surface_get_height(image);

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(window, "expose-event",
            G_CALLBACK(on_expose_event), NULL);
    g_signal_connect(window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "button-press-event",
            G_CALLBACK(clicked), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Justin's KICKASS lines!");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_widget_set_app_paintable(window, TRUE);

    g_timeout_add(2, (GSourceFunc) time_handler, (gpointer) window);

    gtk_widget_show_all(window);

    printf("Starting with cairo version %s\n", cairo_version_string());

    gtk_main();

    cairo_surface_destroy(image);

    return 0;
}
