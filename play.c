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

#include <cairo.h>
#include <gtk/gtk.h>

#define LEFT_BUTTON 1
#define RIGHT_BUTTON 3

static double coordx[100];
static double coordy[100];

static int count = 0;

static gboolean
on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
    int i;
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 0.5);

    for (i = 0; i < count-1; i++) {
        cairo_move_to(cr, coordx[i], coordy[i]);
        cairo_line_to(cr, coordx[i+1], coordy[i+1]);
    }

    count = 0;
    cairo_stroke(cr);
    cairo_destroy(cr);

    return FALSE;
}

static gboolean
clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (event->button == LEFT_BUTTON) {
        coordx[count] = event->x;
        coordy[count++] = event->y;
    }

    if (event->button == RIGHT_BUTTON) {
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
}

int
main(int argc, char *argv[])
{
    GtkWidget *window;

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
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_widget_set_app_paintable(window, TRUE);

    gtk_widget_show_all(window);

    printf("Starting with cairo version %s\n", cairo_version_string());

    gtk_main();

    return 0;
}
