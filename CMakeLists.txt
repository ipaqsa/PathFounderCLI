cmake_minimum_required(VERSION 3.22)
project(PathFounded C)

set(CMAKE_C_STANDARD 17)

add_executable(PathFounded main.c
        src/Engine.c src/Engine.h
        src/DS/queue_lib.h src/DS/queue_lib.c
        src/DS/list_lib.h src/DS/list_lib.c)
