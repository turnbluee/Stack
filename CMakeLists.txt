cmake_minimum_required(VERSION 3.28)
project(Stack C)

set(CMAKE_C_STANDARD 11)

add_executable(Stack main.c
        UserHeader.h
        FuncsHeader.h
        FuncsRealization.c
)
