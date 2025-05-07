#ifndef JS_H
#define JS_H

gchar *readTextFile(gchar *filename, gsize *length);

typedef	struct {
	WebKitWebView *webView;
  gchar *script;
} OnceCbParamType;

#endif /* JS */