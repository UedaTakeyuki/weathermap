#ifndef JS_H
#define JS_H

#include <webkit2/webkit2.h>

gchar *readTextFile(gchar *filename, gsize *length);

typedef	struct {
	WebKitWebView *webView;
  gchar *script;
} OnceCbParamType;

void load_url_and_add_javascripts(WebKitWebView *webView);

#endif /* JS */