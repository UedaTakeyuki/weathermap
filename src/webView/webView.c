#include "js.h"
#include "eventHandler.h"
#include <webkit2/webkit2.h>

// Create a browser instance
WebKitWebView * create_browser_instance(){
  WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

  // set call back for load_changed event
  g_signal_connect(webView, "load_changed", G_CALLBACK(web_view_load_changed), NULL);
  g_signal_connect(webView, "load_failed",  G_CALLBACK(web_view_load_failed), NULL);

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

  // load url and add javascripts
  load_url_and_add_javascripts(webView);
  
  return webView;
}