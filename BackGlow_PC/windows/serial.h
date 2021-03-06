#ifndef SERIAL_H
#define SERIAL_H

#include <QString>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class Serial
{
private:
    HANDLE  serialHandle;
    bool    connected;

public:
    Serial(QString portName);
    virtual ~Serial();

    virtual int  read (char *buffer, unsigned int nbChar);
    virtual int write(char *buffer, unsigned int nbChar);
    virtual bool isReady();
};

#endif // SERIAL_H
