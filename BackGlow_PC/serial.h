#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class Serial
{
private:
    HANDLE  serialHandle;
    bool    connected;

public:
    Serial(const char *portName);
    ~Serial();

    int  read (char *buffer, unsigned int nbChar);
    bool write(char *buffer, unsigned int nbChar);
    bool isReady();
};

#endif // SERIAL_H
