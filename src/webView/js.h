#ifndef JS_H
#define JS_H

#include <webkit2/webkit2.h>

gchar *readTextFile(gchar *filename, gsize *length);

typedef	struct {
	WebKitWebView *webView;
  gchar *script;
} OnceCbParamType;

gboolean once_cb(gpointer user_data);

gboolean repeated_cb(gpointer user_data);

void refresh_site_every_5_minutes(gpointer user_data);

#endif /* JS */