#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H


class ScreenCapture
{
protected:
    int m_ScreenWidth  = 0;
    int m_ScreenHeight = 0;
    unsigned char* m_ScreenData = 0;

public:
    ScreenCapture();
    ~ScreenCapture();

    int getWidth();
    int getHeight();

    int getBlue(int x, int y);
    int getGreen(int x, int y);
    int getRed(int x, int y);

    virtual void capture() = 0;
};

#endif // SCREENCAPTURE_H
