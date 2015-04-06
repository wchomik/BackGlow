#ifndef SCREENCAPTURELINUX_H
#define SCREENCAPTURELINUX_H

#include "../screencapture_.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class ScreenCapture : public ScreenCapture_{
private:
    Display * xDisplay;
    Window xRootWindow;
    XImage * xImage;
    bool isOk;
public:
    ScreenCapture();
    ~ScreenCapture();
    void capture();
};

#endif // SCREENCAPTURELINUX_H

