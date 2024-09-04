#include <glib.h>
#include <unistd.h>
#include <stdio.h>
gchar *get_WebKitWebProces_pid(){

  // call pgrep
  GString *command_line = g_string_new ("");
  g_string_printf (command_line, "pgrep -lP %d", getpid());
  gchar *standard_output = NULL;
  gboolean result;
  GError *error = NULL;
  gint status;

  result = g_spawn_command_line_sync (
    command_line->str, // const gchar* command_line,
    &standard_output, // gchar** standard_output,
    NULL, // gchar** standard_error,
    &status, // gint* wait_status,
    &error// GError** error
  );
  // free command_line
  g_string_free (command_line, TRUE);

  if (!result){ // error handling
    g_print("err: %s\n", error->message);
    g_print("err: %d\n", error->code);
    g_print("status: %d\n", status);
    g_error_free (error);
    return NULL;
  } else {
    printf("standard_output: %s",standard_output);
    gchar **lines = g_strsplit(standard_output, "\n", 0);
    for (int i =0; i < sizeof(lines) / sizeof(gchar *); i++){
      gchar **line = g_strsplit(lines[i], " ", 0);
      if (g_strcmp0(g_strstrip(line[1]), "WebKitWebProces")){
        printf("match\n");
        return g_strstrip(line[0]);
      } else {
        printf("line[1]: %s", g_strstrip(line[1]));
      }
    }
    return NULL;
  }
}

void restrict_cpu_usage_of_WebKitWebProces(){
  gchar *my_pid = get_WebKitWebProces_pid();
  GString *command_line;
  if (my_pid != NULL){
    command_line = g_string_new ("");
    g_string_printf (command_line, "cpulimit -l 5 -p %s &", my_pid);
  } else {
    return;
  }
  gchar *standard_output = NULL;
  gboolean result;
  GError *error = NULL;
  gint status;

  result = g_spawn_command_line_sync (
    command_line->str, // const gchar* command_line,
    &standard_output, // gchar** standard_output,
    NULL, // gchar** standard_error,
    &status, // gint* wait_status,
    &error// GError** error
  );
  // free command_line
  g_string_free (command_line, TRUE);

  if (!result){ // error handling
    g_print("err: %s\n", error->message);
    g_print("err: %d\n", error->code);
    g_print("status: %d\n", status);
    g_error_free (error);
  }
}

gboolean cpu_restrict_cb(gpointer user_data){
  restrict_cpu_usage_of_WebKitWebProces();
  return FALSE;
}