cmake_minimum_required(VERSION 3.25)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)

add_executable(Minesweeper main.cpp window.hpp window.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Minesweeper Threads::Threads)