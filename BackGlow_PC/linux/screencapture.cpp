#include "screencapture.h"

#include <stdio.h>

ScreenCapture::ScreenCapture() :
    xDisplay(nullptr),
    xRootWindow(0),
    xImage(nullptr),
    isOk(false)
{
    xDisplay = XOpenDisplay(NULL);
    if(xDisplay == nullptr){
        fputs("Unable to grab screen.\n", stderr);
        return;
    }

    xRootWindow = XDefaultRootWindow(xDisplay);
    if(xRootWindow == 0){
        fputs("Unable to grab screen.\n", stderr);
        return;
    }


    XWindowAttributes windowAttr;

    XGetWindowAttributes(xDisplay, xRootWindow, &windowAttr);
    m_ScreenWidth = windowAttr.width;
    m_ScreenHeight = windowAttr.height;

    isOk = true;
}

void ScreenCapture::capture(){
    if(xImage != nullptr){
        XDestroyImage(xImage);
    }
    xImage = XGetImage(xDisplay,xRootWindow, 0,0 , m_ScreenWidth,m_ScreenHeight,AllPlanes, ZPixmap);
    m_ScreenData = (unsigned char *) xImage->data;
}

ScreenCapture::~ScreenCapture(){

}
