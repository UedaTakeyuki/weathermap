// https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial
#include <gtk/gtk.h>
//#include <webkit2/webkit2.h>
//#include "displaysize.h"
#include "ipc/ipc.h"
#include "screensize/screensize.h"
#include "restrictcpu/restrictcpu.h"
#include "webView/webView.h"
#include "webView/js.h"

// GtkWidget *main_window;

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

static gboolean once_cb(gpointer user_data){
  // https://stackoverflow.com/a/21861770/11073131
  OnceCbParamType *param = user_data;

  webkit_web_view_run_javascript(param->webView,
                                 param->script,
                                 NULL,
                                 NULL, //web_view_javascript_finished,
                                 NULL);
  g_warning("script: %s", param->script);
  g_message("once_cb done.\n");
  return FALSE;
}

static gboolean repeated_cb(gpointer user_data){
  // https://stackoverflow.com/a/21861770/11073131
  OnceCbParamType *param = user_data;

  webkit_web_view_run_javascript(param->webView,
                                 param->script,
                                 NULL,
                                 NULL, //web_view_javascript_finished,
                                 NULL);
//  g_warning("script: %s", param->script);
  g_message("repeated_cb done.\n");
  return TRUE;
}

static void refresh_site_every_5_minutes(gpointer user_data){
  OnceCbParamType *param = user_data;
  for (;;) {
    webkit_web_view_run_javascript(param->webView,
                                   param->script,
                                   NULL,
                                   NULL, //web_view_javascript_finished,
                                   NULL);
    g_message("refresh_site_every_5_minutes done.\n");

//    delay(300000);
//    sleep(300);
    {
      /* XXX Too bad if you don't have select(). */
      struct timeval t;
      t.tv_sec = 300;
      t.tv_usec = 0;
      g_print("before select.\n");
      select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &t);
      g_print("after select.\n");
    }
  }
}

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

  // Create a browser instance
  WebKitWebView *webView = create_browser_instance();
  //WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

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

  // read city ID
  gchar *cityID = readTextFile("cityID.txt", NULL);
/*
  gchar *contents;
  gsize length;
  GError *error;
  if (g_file_get_contents ("cityID.txt",
                     &contents,
                     &length,
                     &error)){
    g_warning("city ID: %s", contents);
  } else {
    g_warning ("Error running javascript: %s", error->message);
    g_error_free (error);
  }
*/
  // Load a web page into the browser instance
  // webkit_web_view_load_uri(webView, "https://openweathermap.org/city/1852278");
  gchar *url = g_strconcat("https://openweathermap.org/city/", cityID, NULL);
//  g_free (contents);
  webkit_web_view_load_uri(webView, url);
  g_free (url);

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);


  // read script
  gchar *allowCookiesScript = readTextFile("allowCookies.js", NULL);
/*
  if (g_file_get_contents ("allowCookies.js",
                     &allowCookiesScript,
                     &length,
                     &error)){
//    g_warning("script: %s", allowCookiesScript);
  } else {
    g_warning ("Error running javascript: %s", error->message);
    g_error_free (error);
  }
*/

  // make param
  OnceCbParamType param;
  param.webView = webView;
  param.script = allowCookiesScript;

  // call script after 1 min 30sec
  //GSource* gsource = g_timeout_source_new_seconds (300);
  g_timeout_add_seconds (90, once_cb, &param);
  //once_cb((gpointer)&param);

  // read script
  gchar *relodeScript = readTextFile("relode.js", NULL);
/*
  if (g_file_get_contents ("relode.js",
                     &relodeScript,
                     &length,
                     &error)){
//    g_warning("script: %s", relodeScript);
  } else {
    g_warning ("Error running javascript: %s", error->message);
    g_error_free (error);
  }
*/
  // concatinate URL
  relodeScript = g_strconcat("const myOpenWeatherURL = '", "https://openweathermap.org/city/", cityID, "'\n", relodeScript, NULL);
  g_warning("script: %s", relodeScript);

  // make param
  OnceCbParamType reloadParam;
  reloadParam.webView = webView;
  reloadParam.script = relodeScript;

  // call once_cb every 5 min.
  //g_timeout_add (300000, repeated_cb, &reloadParam);
  //g_timeout_add_seconds (300, repeated_cb, &reloadParam);
  GThread *thread_refresh_site = g_thread_new("refresh_site thread", (gpointer)&refresh_site_every_5_minutes, &reloadParam);

  // Run the server thread
/*
  g_warning ("Before Thread RUnning");
  GThread *thread_ice = g_thread_new("ICE thread", (gpointer)&server, NULL);
  g_warning ("Before Thread Join");
//  g_thread_join(thread_ice);
  g_warning ("Before Thread unref");
  g_thread_unref(thread_ice);
  g_warning ("After Thread unref");
*/

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

