#include <webkit2/webkit2.h>

// Call back for load-changed event
void web_view_load_changed (WebKitWebView  *web_view,
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