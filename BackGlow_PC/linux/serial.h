#ifndef SERIALLINUX
#define SERIALLINUX

#ifdef __unix
#include "serial.h"

class Serial
{
private:
    int fd;
    void configure(int speed, int parity, bool blocking);
    bool isOk;
public:
    Serial(const char* portName);
    ~Serial();
    int read (char *buffer, unsigned int nbChar);
    int write(char *buffer, unsigned int nbChar);
    bool isReady(){ return isOk; }
};

#define SERIAL_CLASS SerialLinux

#endif
#endif // SERIALLINUX

