#ifndef SCREENCAPTUREWIN_H
#define SCREENCAPTUREWIN_H
#include <Windows.h>
#include <wingdi.h>

#include "screencapture.h"

class ScreenCaptureWin : public ScreenCapture
{
private:
    HDC m_hdcScreen;
    HDC m_hdcMem;

public:
    ScreenCaptureWin();
    ~ScreenCaptureWin();

    virtual void capture();
};

#endif // SCREENCAPTUREWIN_H
