// https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

static gboolean once_cb(gpointer user_data){
  // https://stackoverflow.com/a/21861770/11073131
  WebKitWebView *webView = user_data;
//  webkit_web_view_run_javascript(webView, "window.scrollTo(230,100)", NULL, NULL, NULL);
  webkit_web_view_run_javascript(webView,
                                 "location.reload();"
                                 "const footer_panel = document.querySelector('.stick-footer-panel__link')"
                                 "if (footer_panel != null) {"
                                 "  footer_panel.click()"
                                 "}",
                                 NULL, NULL, NULL);
  g_print("once_cb done.\n");
//  return FALSE;
}

int main(int argc, char* argv[]){
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create an 800x600 window that will contain the browser instance
  GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(main_window), 1000, 800);
  gtk_window_move(GTK_WINDOW(main_window),0, 36);


  // Create a browser instance
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

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
