#include "serial.h"

Serial::Serial(const char *portName)
{
    this->connected    = false;
    this->serialHandle = CreateFile(reinterpret_cast<LPCWSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (this->serialHandle == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
            printf("Port %s is unavailable.\n", portName);
        } else {
            printf("Unknown Error");
        }
    } else {
        DCB commSettings = { 0 };

        if (!GetCommState(this->serialHandle, &commSettings)) {
            printf("Could not get communictaion settings!\n");
        } else {
            commSettings.BaudRate = CBR_9600;
            commSettings.ByteSize = 8;
            commSettings.StopBits = ONESTOPBIT;

            if (!SetCommState(serialHandle, &commSettings)) {
                printf("Could not change communictaion settings!\n");
            } else {
                this->connected = true;
            }
        }
    }
}

Serial::~Serial()
{
    if (this->connected) {
        this->connected = false;
        CloseHandle(this->serialHandle);
    }
}

int Serial::read(char *buffer, unsigned int nbChar)
{
    DWORD        bytesRead;
    unsigned int toRead;
    DWORD        errors;
    COMSTAT      status;

    ClearCommError(this->serialHandle, &errors, &status);

    if (status.cbInQue>0) {
        if (status.cbInQue>nbChar) {
            toRead = nbChar;
        } else {
            toRead = status.cbInQue;
        }

        if (ReadFile(this->serialHandle, buffer, toRead, &bytesRead, NULL) && bytesRead != 0) {
            return bytesRead;
        }

    }

    return -1;
}


int Serial::write(char *buffer, unsigned int nbChar)
{
    DWORD     bytesSend;
    DWORD     errors;
    COMSTAT   status;

    if (!WriteFile(this->serialHandle, (void *)buffer, nbChar, &bytesSend, 0)) {
        ClearCommError(this->serialHandle, &errors, &status);
        return -1;
    }

    return bytesSend;
}

bool Serial::isReady()
{
    return this->connected;
}
