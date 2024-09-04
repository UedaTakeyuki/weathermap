#ifndef RESTRICTCPU_H
#define RESTRICTCPU_H

void restrict_cpu_usage_of_WebKitWebProces();
gboolean cpu_restrict_cb(gpointer user_data);

#endif /* RESTRICTCPU_H */