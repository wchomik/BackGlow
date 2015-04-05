#include "screencapturewin.h"

ScreenCaptureWin::ScreenCaptureWin()
{
    m_hdcScreen    = GetDC(NULL);
    m_hdcMem       = CreateCompatibleDC(m_hdcScreen);
    m_ScreenWidth  = GetDeviceCaps(m_hdcScreen, HORZRES);
    m_ScreenHeight = GetDeviceCaps(m_hdcScreen, VERTRES);

    if (m_ScreenData)
        free(m_ScreenData);
    m_ScreenData = (unsigned char*)malloc(4 * m_ScreenWidth * m_ScreenHeight);
    memset(m_ScreenData, 0, 4 * m_ScreenWidth * m_ScreenHeight);
}

ScreenCaptureWin::~ScreenCaptureWin()
{
    free(m_ScreenData);
    ReleaseDC(NULL, m_hdcScreen);
    DeleteDC(m_hdcMem);
}

void ScreenCaptureWin::capture()
{
    HBITMAP hBitmap = CreateCompatibleBitmap(m_hdcScreen, m_ScreenWidth, m_ScreenHeight);
    HGDIOBJ hOld    = SelectObject(m_hdcMem, hBitmap);
    BitBlt(m_hdcMem, 0, 0, m_ScreenWidth, m_ScreenHeight, m_hdcScreen, 0, 0, SRCCOPY);
    SelectObject(m_hdcMem, hOld);

    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes      = 1;
    bmi.biBitCount    = 32;
    bmi.biWidth       = m_ScreenWidth;
    bmi.biHeight      = -m_ScreenHeight;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage   = 0;

    GetDIBits(m_hdcMem, hBitmap, 0, m_ScreenHeight, m_ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    DeleteObject(hBitmap);
}

