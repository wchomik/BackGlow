#ifndef SCREENCAPTUREWIN_H
#define SCREENCAPTUREWIN_H

#include <Windows.h>
#include <wingdi.h>

#include "screencapture.h"

class ScreenCapture : public ScreenCapture_
{
private:
    HDC m_hdcScreen;
    HDC m_hdcMem;

public:
    ScreenCaptureWin();
    virtual ~ScreenCaptureWin();

    void capture();
};

#endif // SCREENCAPTUREWIN_H
