#ifndef SERIALLINUX
#define SERIALLINUX

#include <QString>

class Serial
{
private:
    int fd;
    void configure(int speed, int parity, bool blocking);
    bool isOk;
public:
    Serial(QString portName);
    ~Serial();
    int read (char *buffer, unsigned int nbChar);
    int write(char *buffer, unsigned int nbChar);
    bool isReady(){ return isOk; }
};

#endif // SERIALLINUX

