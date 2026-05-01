
/**
 * @copyright  Copyright (C) 2026 Kaden Slater. All Rights Reserved.
 * @license    GNU General Public License version 2 or later; see LICENSE.txt
 */

#include <gtk/gtk.h>

#include "display_utils.h"


#define MEMORY_SIZE 4096


static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *gl_area;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "NESTEd");
  gtk_window_set_default_size(GTK_WINDOW(window), 1024, 1024);

  gl_area = gtk_gl_area_new();

  g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
  g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);

  gtk_window_set_child(GTK_WINDOW(window), gl_area);
  gtk_widget_set_size_request(GTK_WIDGET(gl_area), 1024, 1024);

  gtk_window_present(GTK_WINDOW(window));
}


int
main(int    argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new(
    "template.opengl.gtk.application",
    G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
