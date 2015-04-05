#include "screencapture.h"

ScreenCapture::ScreenCapture()
{

}

ScreenCapture::~ScreenCapture()
{

}

int ScreenCapture::getWidth()
{
    return m_ScreenWidth;
}

int ScreenCapture::getHeight()
{
    return m_ScreenHeight;
}

int ScreenCapture::getBlue(int x, int y)
{
    return m_ScreenData[4 * ((y*m_ScreenWidth) + x)];
}

int ScreenCapture::getGreen(int x, int y)
{
    return m_ScreenData[4 * ((y*m_ScreenWidth) + x) + 1];
}

int ScreenCapture::getRed(int x, int y)
{
    return m_ScreenData[4 * ((y*m_ScreenWidth) + x) + 2];
}

