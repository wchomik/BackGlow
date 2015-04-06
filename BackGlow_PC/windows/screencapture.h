#ifndef SCREENCAPTUREWIN_H
#define SCREENCAPTUREWIN_H

#include <Windows.h>
#include <wingdi.h>

#include "../screencapture_.h"

class ScreenCapture : public ScreenCapture_
{
private:
    HDC m_hdcScreen;
    HDC m_hdcMem;

public:
    ScreenCapture();
    virtual ~ScreenCapture();

    void capture();
};

#endif // SCREENCAPTUREWIN_H
