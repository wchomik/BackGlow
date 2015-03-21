#ifndef AMBIENTPIXELS_H
#define AMBIENTPIXELS_H

#include <Windows.h>
#include <wingdi.h>
#include <QtSerialPort/QSerialPort>

#include "serial.h"

class BackGlow
{
private:
    Serial* SP;

    inline int PosB(int x, int y)
    {
        return ScreenData[4 * ((y*ScreenX) + x)];
    }

    inline int PosG(int x, int y)
    {
        return ScreenData[4 * ((y*ScreenX) + x) + 1];
    }

    inline int PosR(int x, int y)
    {
        return ScreenData[4 * ((y*ScreenX) + x) + 2];
    }

public:
    BackGlow(const char *port);
    ~BackGlow();

    void process();


    int leds  = 30;
    int depth = 50;
    float brightnes = 0.7f;
    float redIntensity   = 1.0f;
    float greenIntensity = 1.0f;
    float blueIntensity  = 1.0f;

    int ScreenX = 0;
    int ScreenY = 0;
    BYTE* ScreenData = 0;

    HDC hdcScreen;
    HDC hdcMem;
};

#endif // AMBIENTPIXELS_H
