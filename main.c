// https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "displaysize.h"

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

static gboolean once_cb(gpointer user_data){
  // https://stackoverflow.com/a/21861770/11073131
  WebKitWebView *webView = user_data;
//  webkit_web_view_run_javascript(webView, "window.scrollTo(230,100)", NULL, NULL, NULL);
  webkit_web_view_run_javascript(webView,
                                 "try {"
                                 "  location.reload();"
                                 "  const footer_panel = document.querySelector('.stick-footer-panel__link');"
                                 "  if (footer_panel != null) {"
                                 "    footer_panel.click();"
                                 "  }"
                                 "} catch (e) {"
                                 "  return e.name + ": " + e.message"
                                 "}"
                                 "return ''",
                                 NULL, NULL, NULL);
  g_print("once_cb done.\n");
//  return FALSE;
}

int main(int argc, char* argv[]){
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create an 800x600 window that will contain the browser instance
  GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  // hide menu bar
  gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);

  // set window size
  gint setting_width = 1000; // default width
  gint setting_height = 800; // default height
  gint result; // exit-status of "displaysize" func, 0: succeeded, 1: fault
  gint width, height;
  result = displaysize(&width, &height);
  if (result == 0){
    setting_width = width * 10/18;
    setting_height = height*9/10 -36;
  }

  gtk_window_set_default_size(GTK_WINDOW(main_window), setting_width, setting_height);
  gtk_window_move(GTK_WINDOW(main_window),0, 36);


  // Create a browser instance
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

  // Set zoom level with current width for width 1024.
  gdouble zoom_level = (gdouble)width / 1024.0;
  g_print("zoom_level: %lf\n", zoom_level);
  //webkit_web_view_set_zoom_level(webView,  zoom_level);

  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

  // Load a web page into the browser instance
  webkit_web_view_load_uri(webView, "https://openweathermap.org/city/1859642");

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // change font size
  WebKitSettings *settings = webkit_web_view_get_settings (webView);
  webkit_settings_set_default_font_size(settings, 48);
  webkit_settings_set_enable_write_console_messages_to_stdout(settings, TRUE);

  // horizontal scroll
//  GtkAdjustment* adjustment = gtk_scrolled_window_get_hadjustment (webView);
//  gtk_adjustment_set_value (adjustment, 32);


  // https://stackoverflow.com/a/21861770/11073131
  //webkit_web_view_run_javascript(webView, "window.scrollTo(1500,500)", NULL, NULL, NULL);

  // call once_cb every 5 min.
  g_timeout_add (300000, once_cb, webView);

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
