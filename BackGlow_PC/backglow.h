#ifndef AMBIENTPIXELS_H
#define AMBIENTPIXELS_H

#include <QtSerialPort/QSerialPort>
#include <QString>

#include "serial_.h"
#include "screencapture_.h"
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class BackGlow
{
private:
    Serial m_Serial;
    ScreenCapture m_ScreenCap;

    int m_ScreenWidth;
    int m_ScreenHeight;
    static const int MAXLEDS = 30;
    cl::Context context;
    cl::Device device;
    cl::Kernel kernelPass1;
    cl::Kernel kernelPass2;
    cl::Image2D inputImage;
    cl::Buffer middleBuffer;
    cl::Buffer outputBuffer;
    cl::Buffer constantBuffer;
public:
    BackGlow(QString port);
    ~BackGlow();

    void process();

    int   m_leds           = 23;
    int   m_depth          = 50;
    float m_brightnes      = 0.7f;
    float m_redIntensity   = 1.0f;
    float m_greenIntensity = 1.0f;
    float m_blueIntensity  = 1.0f;
};

#endif // AMBIENTPIXELS_H
