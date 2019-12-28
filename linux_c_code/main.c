#include <stdio.h>

#include "hardware.h"

int main() {
    int fd = openPort("/dev/ttyACM1");
    hardwareReset(fd);

    ping(fd,1);

    sleep(10);

    ping(fd, 0);


    return 0;
}
