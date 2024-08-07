// https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
//#include "displaysize.h"
#include "ipc/ipc.h"
#include "screensize/screensize.h"

// GtkWidget *main_window;

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

typedef	struct {
	WebKitWebView *webView;
  gchar *script;
} OnceCbParamType;

// https://webkitgtk.org/reference/webkit2gtk/2.5.1/WebKitWebView.html#webkit-web-view-run-javascript-finish
/*
static void
web_view_javascript_finished (GObject      *object,
                              GAsyncResult *result,
                              gpointer      user_data)
{
    WebKitJavascriptResult *js_result;
    JSValueRef              value;
    JSGlobalContextRef      context;
    GError                 *error = NULL;

    js_result = webkit_web_view_run_javascript_finish (WEBKIT_WEB_VIEW (object), result, &error);
    if (!js_result) {
        g_warning ("Error running javascript: %s", error->message);
        g_error_free (error);
//        gtk_main_quit();
        gtk_widget_destroy(main_window);
        return;
    }

    context = webkit_javascript_result_get_global_context (js_result);
    value = webkit_javascript_result_get_value (js_result);
    if (JSValueIsString (context, value)) {
        JSStringRef js_str_value;
        gchar      *str_value;
        gsize       str_length;

        js_str_value = JSValueToStringCopy (context, value, NULL);
        str_length = JSStringGetMaximumUTF8CStringSize (js_str_value);
        str_value = (gchar *)g_malloc (str_length);
        JSStringGetUTF8CString (js_str_value, str_value, str_length);
        JSStringRelease (js_str_value);
        g_print ("Script result: %s\n", str_value);
        g_free (str_value);
    } else {
        g_warning ("Error running javascript: unexpected return value");
    }
    webkit_javascript_result_unref (js_result);
}
*/
static gboolean once_cb(gpointer user_data){
  // https://stackoverflow.com/a/21861770/11073131
  OnceCbParamType *param = user_data;

  webkit_web_view_run_javascript(param->webView,
                                 param->script,
                                 NULL,
                                 NULL, //web_view_javascript_finished,
                                 NULL);
  g_warning("script: %s", param->script);
  g_print("once_cb done.\n");
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
  g_print("once_cb done.\n");
  //return FALSE;
}

int main(int argc, char* argv[]){
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create an 800x600 window that will contain the browser instance
  GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  // hide menu bar
  gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);

  // set window size
  /*
  gint setting_width = 1000; // default width
  gint setting_height = 800; // default height
  gint result; // exit-status of "displaysize" func, 0: succeeded, 1: fault
  gint width, height;
  result = displaysize(&width, &height);
  if (result == 0){
    setting_width = width * 10/18;
//    setting_height = height*9/10 -36;
    setting_height = height -36;
  }
*/

  // get width_height
  struct width_height *wh = getScreenSizeFromGDK();
  gint setting_width = wh->width * 10/18;
  gint setting_height = wh->height -36;

  gtk_window_set_default_size(GTK_WINDOW(main_window), setting_width, setting_height);
  gtk_window_move(GTK_WINDOW(main_window),0, 36);


  // Create a browser instance
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

  // Set zoom level with current width for width 1024.
  gdouble zoom_level = (gdouble)(wh->width) / 1024.0;
  g_print("zoom_level: %lf\n", zoom_level);
  //webkit_web_view_set_zoom_level(webView,  zoom_level);

  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

  // read city ID
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
  // Load a web page into the browser instance
  // webkit_web_view_load_uri(webView, "https://openweathermap.org/city/1852278");
  gchar *url = g_strconcat("https://openweathermap.org/city/", contents, NULL);
//  g_free (contents);
  webkit_web_view_load_uri(webView, url);
  g_free (url);

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

  // get enable_write_console_messages_to_stdout
  g_warning("enable_write_console_messages_to_stdout: %d",
            webkit_settings_get_enable_write_console_messages_to_stdout(settings));

  // read script
  gchar *allowCookiesScript;
  if (g_file_get_contents ("allowCookies.js",
                     &allowCookiesScript,
                     &length,
                     &error)){
//    g_warning("script: %s", allowCookiesScript);
  } else {
    g_warning ("Error running javascript: %s", error->message);
    g_error_free (error);
  }

  // make param
  OnceCbParamType param;
  param.webView = webView;
  param.script = allowCookiesScript;

  // call script after 1 min 30sec
  g_timeout_add_seconds (90, once_cb, &param);
  //once_cb((gpointer)&param);
  //GThread *thread_ice = g_thread_new("ICE thread", (gpointer)&once_cb, &param);
//  g_thread_join(thread_ice);
//  g_thread_unref(thread_ice);
  // read script
  gchar *relodeScript;
//  gsize length;
//  GError *error;
  if (g_file_get_contents ("relode.js",
                     &relodeScript,
                     &length,
                     &error)){
//    g_warning("script: %s", relodeScript);
  } else {
    g_warning ("Error running javascript: %s", error->message);
    g_error_free (error);
  }

  // concatinate URL
  relodeScript = g_strconcat("const myOpenWeatherURL = '", "https://openweathermap.org/city/", contents, "'\n", relodeScript, NULL);
  g_warning("script: %s", relodeScript);

  // make param
  OnceCbParamType reloadParam;
  reloadParam.webView = webView;
  reloadParam.script = relodeScript;

  // call once_cb every 5 min.
  g_timeout_add (300000, repeated_cb, &reloadParam);

  // Run the server thread
  g_warning ("Before Thread RUnning");
  GThread *thread_ice = g_thread_new("ICE thread", (gpointer)&server, NULL);
  g_warning ("Before Thread Join");
//  g_thread_join(thread_ice);
  g_warning ("Before Thread unref");
  g_thread_unref(thread_ice);
  g_warning ("After Thread unref");

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

