#include <glib.h>
#include "js.h"

gchar *readTextFile(gchar *filename, gsize *length){
  GError *error;
  gchar *contents;
  if (g_file_get_contents (filename,
                     &contents,
                     length,
                     &error)){
    return contents;
  } else {
    g_warning ("Error: can't read file %s: %s", filename, error->message);
    g_error_free (error);
    return NULL;
  }
}

gboolean once_cb(gpointer user_data){
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

gboolean repeated_cb(gpointer user_data){
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

void refresh_site_every_5_minutes(gpointer user_data){
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

void load_url_and_add_javascripts(WebKitWebView *webView){

  ////////////////
  // load url
  ////////////////

  // read city ID
  static gchar *cityID; 
  cityID = readTextFile("cityID.txt", NULL);

  // url to load
  gchar *url = g_strconcat("https://openweathermap.org/city/", cityID, NULL);

  // load url 
  webkit_web_view_load_uri(webView, url);
  g_free (url);

  
  //////////////////////////
  // script to allow Cookies
  //////////////////////////

  // read script
  static gchar *allowCookiesScript;
  allowCookiesScript = readTextFile("allowCookies.js", NULL);

  // make param
  static OnceCbParamType param_allowCookies;
  param_allowCookies.webView = webView;
  param_allowCookies.script = allowCookiesScript;

  // call script after 1 min 30sec
  g_timeout_add_seconds (90, once_cb, &param_allowCookies);

  //////////////////////////
  // script to reload
  //////////////////////////

  // read script
  static gchar *relodeScript;
  relodeScript = readTextFile("relode.js", NULL);

  // concatinate URL
  relodeScript = g_strconcat("const myOpenWeatherURL = '", "https://openweathermap.org/city/", cityID, "'\n", relodeScript, NULL);
  g_warning("script: %s", relodeScript);

  // make param
  static OnceCbParamType reloadParam;
  reloadParam.webView = webView;
  reloadParam.script = relodeScript;

  // call once_cb every 5 min.
  //g_timeout_add (300000, repeated_cb, &reloadParam);
  //g_timeout_add_seconds (300, repeated_cb, &reloadParam);
  GThread *thread_refresh_site = g_thread_new("refresh_site thread", (gpointer)&refresh_site_every_5_minutes, &reloadParam);
}