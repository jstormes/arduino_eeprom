cmake_minimum_required(VERSION 3.15)
project(linux_c_code C)

set(CMAKE_C_STANDARD 99)

add_executable(linux_c_code main.c hardware.c hardware.h)