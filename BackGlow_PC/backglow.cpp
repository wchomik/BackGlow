#include "backglow.h"
#include <cmath>
#include <emmintrin.h>

BackGlow::BackGlow(QString port) :
    m_Serial(port)
{
    m_ScreenWidth  = m_ScreenCap.getWidth();
    m_ScreenHeight = m_ScreenCap.getHeight();

    if (m_Serial.isReady()) {
        printf("Connected to serial port\n");
    } else {
        printf("Cannot connect to serial port!");
    }
}

BackGlow::~BackGlow()
{
    unsigned char buffer[1024];
    memset(buffer, 0, 1024);
    buffer[0] = (unsigned char)(m_leds);
    m_Serial.write((char *)buffer, m_leds * 3 + 1);
}

void BackGlow::process()
{
    static bool tableDone = false;
    static float gammaTable[256];
    if(!tableDone){
        for(int i = 0; i < 256; ++i){
            gammaTable[i] = pow(i / 256.0f, 2.2f) * 256;
        }
        tableDone = true;
    }

    m_ScreenCap.capture();

    float rgb[3 * MAXLEDS];
    memset(rgb, 0, sizeof(rgb));
    for(int y = 0; y < m_depth; ++y){
        for(int led = 0; led < m_leds; ++led){
            float r = 0, g = 0, b = 0;
            int xMax = m_ScreenWidth * (led + 1) / m_leds;
            for(int x = m_ScreenWidth * led / m_leds; x < xMax; ++x){
                int pixel = m_ScreenCap.getPixel(x, y);
                r += gammaTable[(pixel & 0xff0000) >> 16];
                g += gammaTable[(pixel & 0x00ff00) >> 8];
                b += gammaTable[(pixel & 0x0000ff)];
            }
            rgb[3 * led + 0] += r;
            rgb[3 * led + 1] += g;
            rgb[3 * led + 2] += b;
         }
    }

    unsigned char buffer[3 * MAXLEDS + 1];
    int i = 0;
    memset(buffer, 0, sizeof(buffer));
    buffer[i] = (unsigned char)(m_leds);
    for(int led = m_leds - 1; led >= 0; --led){
        buffer[++i] = (unsigned char)(rgb[led * 3 + 0] / ((m_ScreenWidth / m_leds) * m_depth) * m_brightnes * m_redIntensity);
        buffer[++i] = (unsigned char)(rgb[led * 3 + 1] / ((m_ScreenWidth / m_leds) * m_depth) * m_brightnes * m_greenIntensity);
        buffer[++i] = (unsigned char)(rgb[led * 3 + 2] / ((m_ScreenWidth / m_leds) * m_depth) * m_brightnes * m_blueIntensity);
    }

    m_Serial.write((char *)buffer, ++i);
}
