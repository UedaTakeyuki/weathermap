#ifndef SCREENSIZE_H
#define SCREENSIZE_H

#include <glib.h>

#define DESKTOP_MANAGER_HEIGHT 32

// Width & Height struct
struct width_height {
    gint width;
    gint height;
};

struct width_height *getScreenSizeFromGDK();

#endif /* SCREENSIZE_H */
