#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

class ScreenCapture_
{
protected:
    int m_ScreenWidth  = 0;
    int m_ScreenHeight = 0;
    unsigned char* m_ScreenData = 0;

public:
    virtual ~ScreenCapture_(){}

    int getWidth(){ return m_ScreenWidth; }
    int getHeight(){ return m_ScreenHeight; }

    int getBlue(int x, int y);
    int getGreen(int x, int y);
    int getRed(int x, int y);
};

#ifdef WIN32
#include "windows/screencapture.h"
#elif defined(__unix)
#include "linux/screencapture.h"
#endif


#endif // SCREENCAPTURE_H
