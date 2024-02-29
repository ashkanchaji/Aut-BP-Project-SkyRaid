cmake_minimum_required(VERSION 3.26)
project(projectFile C)

set(CMAKE_C_STANDARD 11)

add_executable(projectFile main.c
        menu.c
        menu.h
        color.h
        game.h
        game.c
        board.c
        difficulty.c)
