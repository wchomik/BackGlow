#include "backglow.h"
#include <cmath>

BackGlow::BackGlow(const char * port)
{
    hdcScreen = GetDC(NULL);
    hdcMem    = CreateCompatibleDC(hdcScreen);

    ScreenX = GetDeviceCaps(hdcScreen, HORZRES);
    ScreenY = GetDeviceCaps(hdcScreen, VERTRES);

    if (ScreenData)
        free(ScreenData);
    ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);

    SP = new Serial(port);
    if (SP->isReady()) {
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
    buffer[i++] = (unsigned char)(leds * 3 + 1);
    for (int led = leds - 1; led >= 0; led--) {
        buffer[i++] = 0;
        buffer[i++] = 0;
        buffer[i++] = 0;
    }
    SP->write((char *)buffer, i);

    delete SP;
    free(ScreenData);
    ReleaseDC(NULL, hdcScreen);
    DeleteDC(hdcMem);
}

void BackGlow::process()
{
    static bool tableDone = false;
    static float gammaTable[256];
    if(!tableDone){
        for(int i = 0; i < 256; ++i){
            gammaTable[i] = pow(i / 256.0f, 2.2f) * 256;
            /*gammaTable[i] = i / 256.0f;
            gammaTable[i] = gammaTable[i] < 0.04045 ?
                                gammaTable[i] / 12.92f :
                                powf((gammaTable[i] + 0.055) / 1.055, 2.4f);*/
        }
        tableDone = true;
    }
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, ScreenX, ScreenY);
    HGDIOBJ hOld    = SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hdcScreen, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hOld);

    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = ScreenX;
    bmi.biHeight = -ScreenY;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

    GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    DeleteObject(hBitmap);

    unsigned char buffer[1024];
    int i = 0;
    memset(buffer, 0, 1024);
    buffer[i++] = (unsigned char)(leds * 3 + 1);
    for (int led = leds - 1; led >= 0; led--) {
        int nextLed = led + 1;
        int nextX = nextLed * (ScreenX / leds);
        float r = 0, g = 0, b = 0;
        for (int x = led * (ScreenX / leds); x < nextX; x++) {
            for (int y = 0; y < depth; y++){
                r += gammaTable[PosR(x, y)];
                g += gammaTable[PosG(x, y)],
                b += gammaTable[PosB(x, y)];
            }
        }
        buffer[i++] = (unsigned char)((r / ((ScreenX / leds) * depth)) * brightnes * redIntensity);
        buffer[i++] = (unsigned char)((g / ((ScreenX / leds) * depth)) * brightnes * greenIntensity);
        buffer[i++] = (unsigned char)((b / ((ScreenX / leds) * depth)) * brightnes * blueIntensity);
    }

    SP->write((char *)buffer, i);
}
