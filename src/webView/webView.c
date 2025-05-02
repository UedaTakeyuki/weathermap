#include "webView.h"

// Call back for load-changed event
static void web_view_load_changed (WebKitWebView  *web_view,
                                   WebKitLoadEvent load_event,
                                   gpointer        user_data)
{
  const gchar* uri;
  switch (load_event) {
  case WEBKIT_LOAD_STARTED:
    // New load, we have now a provisional URI
    uri = webkit_web_view_get_uri (web_view);
    // Here we could start a spinner or update the
    // location bar with the provisional URI
    g_message("WEBKIT_LOAD_STARTED: provisional_uri = %s\n", uri);
    break;
  case WEBKIT_LOAD_REDIRECTED:
    uri = webkit_web_view_get_uri (web_view);
    g_message("WEBKIT_LOAD_REDIRECTED: redirected_uri = %s\n", uri);
    break;
  case WEBKIT_LOAD_COMMITTED:
    // The load is being performed. Current URI is
    // the final one and it won't change unless a new
    // load is requested or a navigation within the
    // same page is performed
    uri = webkit_web_view_get_uri (web_view);
    g_message("WEBKIT_LOAD_COMMITTED: uri = %s\n", uri);
    break;
  case WEBKIT_LOAD_FINISHED:
    // Load finished, we can now stop the spinner
    g_message("WEBKIT_LOAD_FINISHED:\n");
    break;
  }
}

// Create a browser instance
WebKitWebView * create_browser_instance(){
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

  // set call back for load_changed event
  g_signal_connect(webView, "load_changed", G_CALLBACK(web_view_load_changed), NULL);

  // get settings
  WebKitSettings *settings = webkit_web_view_get_settings (webView);
  // change font size
  webkit_settings_set_default_font_size(settings, 48);
  webkit_settings_set_enable_write_console_messages_to_stdout(settings, TRUE);
  // User Agent spoofing
  webkit_settings_set_user_agent (settings, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36");
  // set settings
  webkit_web_view_set_settings (webView, settings);

  // get enable_write_console_messages_to_stdout
  g_warning("enable_write_console_messages_to_stdout: %d",
            webkit_settings_get_enable_write_console_messages_to_stdout(settings));
  
  return webView;
}