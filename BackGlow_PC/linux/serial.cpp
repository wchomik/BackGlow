#include "serial.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

void Serial::configure(int speed, int parity, bool blocking){
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        fprintf(stderr, "error %d from tcgetattr\n", errno);
        return;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        fprintf(stderr, "error %d from tcsetattr\n", errno);
        return;
    }

    tty.c_cc[VMIN]  = blocking ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        fprintf(stderr, "error %d setting term attributes\n", errno);

    isOk = true;
}

Serial::Serial(QString portName){
    char name[256] = "/dev/";
    strcpy(name + 5, portName.toUtf8());
    fd = open(name, O_RDWR | O_NOCTTY | O_SYNC);
    if(fd == 0) {
        fprintf(stderr, "%s", "Unable to open port.\n");
        return;
    };
    configure(B115200, 0, true);
}

Serial::~Serial(){
    close(fd);
}

int Serial::read(char *buffer, unsigned int nbChar){
    return (int) ::read(fd, buffer, nbChar);
}

int Serial::write(char *buffer, unsigned int nbChar){
    return (int) ::write(fd, buffer, nbChar);
}
