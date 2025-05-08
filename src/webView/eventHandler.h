#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <webkit2/webkit2.h>

void web_view_load_changed (WebKitWebView  *web_view,
                            WebKitLoadEvent load_event,
                            gpointer        user_data);

#endif /* EVENTHANDLER */