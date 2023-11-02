cmake_minimum_required(VERSION 3.25)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)

add_executable(Minesweeper main.cpp Window.h Window.cpp json.hpp)
