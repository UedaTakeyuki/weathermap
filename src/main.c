// https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial
#include <gtk/gtk.h>

#include "screensize/screensize.h"
//#include "restrictcpu/restrictcpu.h"
#include "webView/webView.h"

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

int main(int argc, char* argv[]){
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create an 800x600 window that will contain the browser instance
  GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  // hide menu bar
  gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);
//  gtk_window_set_decorated(GTK_WINDOW(main_window), TRUE);

  // get width_height
  struct width_height *wh = getScreenSizeFromGDK();
  gint setting_width = wh->width * 10/18;
  gint setting_height = wh->height -36;

  gtk_window_set_default_size(GTK_WINDOW(main_window), setting_width, setting_height);
  gtk_window_move(GTK_WINDOW(main_window),0, 36);

  // Create webView
  WebKitWebView *webView = create_browser_instance();

  // Set zoom level with current width for width 1024.
  gdouble zoom_level = (gdouble)(wh->width) / 1024.0;
  g_message("zoom_level: %lf\n", zoom_level);
  //webkit_web_view_set_zoom_level(webView,  zoom_level);

  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // Restrict CPU usage of WebKitWebProces
  //g_timeout_add_seconds (100, cpu_restrict_cb, NULL);
  //restrict_cpu_usage_of_WebKitWebProces();

  // Run the main GTK+ event loop
  gtk_main();

  return 0;
}

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window){
  gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window){
  gtk_widget_destroy(window);
  return TRUE;
}