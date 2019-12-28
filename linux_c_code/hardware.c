//
// Created by jstormes on 12/28/19.
//

#include "hardware.h"

int openPort(char *portName)
{
    int fd;

    fd = open(portName, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portName, strerror(errno));
        return -1;
    }

    setPortAttributes(fd);

    return fd;
}

int setPortAttributes(int fd)
{
    struct termios tty;

    int speed = B9600;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10; // Need to find an "optimal setting".

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int getChar(int fd)
{
    char readBuffer[1];
    int readLength = read(fd, readBuffer, 1);
    if (readLength==0) return 0;
    if (readLength>0) return readBuffer[0];
    return readLength;
}

int hardwareReadLine(int fd, char line[], int maxLineLength, int retries)
{
    int lineLength = 0;
    int retryCount = 0;

    int tmpChar;

    do {
        tmpChar = getChar(fd);
        if (tmpChar > 0) {

            if (tmpChar < 32) {
                if (lineLength > 0) {
                    line[lineLength] = (char) 0;
                    return lineLength;
                }
            } else {
                line[lineLength] = (char) tmpChar;
                lineLength++;
                if (lineLength > maxLineLength) {
                    line[lineLength] = (char) 0;
                    return -1;
                }
            }

        }
        else {
            usleep(1000);
            if (retryCount++ >= retries) {
                return -1;
            }
        }

    } while(1);

}

int hardwareWriteLine(int fd, char line[])
{
    int writeLength = write(fd, line, strlen(line));
    if (writeLength != strlen(line)) {
        printf("Error on serial port write: %d, %s\n", writeLength, strerror(errno));
    }

    writeLength = write(fd, "\n", 1);
    if (writeLength != 1) {
        printf("Error on serial port write: %d, %s\n", writeLength, strerror(errno));
    }

    return writeLength;
}

int hardwareReset(int fd)
{
    char line[1024];

    tcdrain(fd);
    for(int i=0; i<10; i++) {
        hardwareWriteLine(fd, "");
        hardwareReadLine(fd, line, sizeof(line), 1);
        if (strstr(line,"Ready")!=NULL) {
            break;
        }
    }
}

int ping(int fd, long l)
{
    char line[1024];

    sprintf(line, "ping %02X", (unsigned  int)l);
    hardwareWriteLine(fd, line);
    printf("ping sent\n");
    hardwareReadLine(fd, line, sizeof(line), 6);
    printf("%s\n",line);
    if (strstr(line, "ping") != NULL) {
        hardwareReadLine(fd, line, sizeof(line), 6);
        if (strstr(line, "Ready") != NULL) {
            return 0;
        }
    }

    return -1;
}
