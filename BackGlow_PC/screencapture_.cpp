#include "screencapture_.h"

int ScreenCapture_::getBlue(int x, int y)
{
    return m_ScreenData[4 * ((y*m_ScreenWidth) + x)];
}

int ScreenCapture_::getGreen(int x, int y)
{
    return m_ScreenData[4 * ((y*m_ScreenWidth) + x) + 1];
}

int ScreenCapture_::getRed(int x, int y)
{
    return m_ScreenData[4 * ((y*m_ScreenWidth) + x) + 2];
}

