#ifndef SERIAL
#define SERIAL
/*
class Serial_
{
public:
    virtual ~Serial_();
    virtual int read (char *buffer, unsigned int nbChar) = 0;
    virtual int write(char *buffer, unsigned int nbChar) = 0;
    virtual bool isReady() = 0;
};*/

#ifdef WIN32
#include "windows/serial.h"
#elif defined(__unix)
#include "linux/serial.h"
#endif

#endif // SERIAL

