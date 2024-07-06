#include <glib.h>
#include "screensize.h" // RPi screen size
#include <stdio.h> // printf
#include <gtk/gtk.h> // GdkRectangle, gdk_monitor_get_workarea

struct width_height *getScreenSizeFromGDK(){
    // https://stackoverflow.com/a/53509211/11073131
    GdkDisplay *display = gdk_display_get_default();
    GdkWindow* gdkwindow = gdk_get_default_root_window ();
    GdkMonitor* monitor = gdk_display_get_monitor_at_window (display, gdkwindow);

    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_display_get_default()),
        &workarea);

/*
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_display_get_default()),
        &workarea);    
*/
    struct width_height *wh = g_malloc(sizeof(struct width_height));
    wh->width = workarea.width;
    wh->height = workarea.height + DESKTOP_MANAGER_HEIGHT;
    return wh;
}
