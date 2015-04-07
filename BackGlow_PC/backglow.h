#ifndef AMBIENTPIXELS_H
#define AMBIENTPIXELS_H

#include <QtSerialPort/QSerialPort>
#include <QString>

#include "serial_.h"
#include "screencapture_.h"

class BackGlow
{
private:
    Serial m_Serial;
    ScreenCapture m_ScreenCap;

    int m_ScreenWidth;
    int m_ScreenHeight;
    static const int MAXLEDS = 30;
public:
    BackGlow(QString port);
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
