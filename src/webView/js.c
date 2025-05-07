#include <glib.h>

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