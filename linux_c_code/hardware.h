//
// Created by jstormes on 12/28/19.
//

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#ifndef LINUX_C_CODE_HARDWARE_H
#define LINUX_C_CODE_HARDWARE_H

int setPortAttributes(int fd);
int openPort(char *portName);
int hardwareReadLine(int fd, char line[], int maxLineLength, int retries);
int ping(int fd, long l);
int hardwareReset(int fd);

#endif //LINUX_C_CODE_HARDWARE_H
