#include "backglow.h"
#include <cmath>

BackGlow::BackGlow(const char * port)
{
    m_ScreenWidth  = m_ScreenCap.getWidth();
    m_ScreenHeight = m_ScreenCap.getHeight();

    m_Serial = new Serial(port);
    if (m_Serial->isReady()) {
        printf("Connected to serial port\n");
    } else {
        printf("Cannot connect to serial port!");
    }
}

BackGlow::~BackGlow()
{
    unsigned char buffer[1024];
    int i = 0;
    memset(buffer, 0, 1024);
    buffer[i++] = (unsigned char)(m_leds * 3 + 1);
    for (int led = m_leds - 1; led >= 0; led--) {
        buffer[i++] = 0;
        buffer[i++] = 0;
        buffer[i++] = 0;
    }
    m_Serial->write((char *)buffer, i);

    delete m_Serial;
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

    unsigned char buffer[1024];
    int i = 0;
    memset(buffer, 0, 1024);
    buffer[i++] = (unsigned char)(m_leds * 3 + 1);
    for (int led = m_leds - 1; led >= 0; led--) {
        int nextLed = led + 1;
        int nextX = nextLed * (m_ScreenWidth / m_leds);
        float r = 0, g = 0, b = 0;
        for (int x = led * (m_ScreenWidth / m_leds); x < nextX; x++) {
            for (int y = 0; y < m_depth; y++){
                r += gammaTable[m_ScreenCap.getRed(x, y)];
                g += gammaTable[m_ScreenCap.getGreen(x, y)],
                b += gammaTable[m_ScreenCap.getBlue(x, y)];
            }
        }
        buffer[i++] = (unsigned char)((r / ((m_ScreenWidth / m_leds) * m_depth)) * m_brightnes * m_redIntensity);
        buffer[i++] = (unsigned char)((g / ((m_ScreenWidth / m_leds) * m_depth)) * m_brightnes * m_greenIntensity);
        buffer[i++] = (unsigned char)((b / ((m_ScreenWidth / m_leds) * m_depth)) * m_brightnes * m_blueIntensity);
    }

    m_Serial->write((char *)buffer, i);
}
