#ifndef AMBIENTPIXELS_H
#define AMBIENTPIXELS_H

#include <QtSerialPort/QSerialPort>

#ifdef _WIN32
#include "screencapturewin.h"
#define SCREEN_CAP ScreenCaptureWin
#endif

#include "serial.h"

class BackGlow
{
private:
    Serial* m_Serial;
    SCREEN_CAP m_ScreenCap;

    int m_ScreenWidth;
    int m_ScreenHeight;

public:
    BackGlow(const char *port);
    ~BackGlow();

    void process();

    int   m_leds           = 30;
    int   m_depth          = 50;
    float m_brightnes      = 0.7f;
    float m_redIntensity   = 1.0f;
    float m_greenIntensity = 1.0f;
    float m_blueIntensity  = 1.0f;
};

#endif // AMBIENTPIXELS_H
