#pragma once
#include <iostream>
#include <random>
#include <sstream>

enum field_param {
    nothing,
    bomb,
    flag,
    number
};

class Window {
public:
    /*
     * window constructor
     */
    explicit Window(std::ostream &outputStream);

    /*
     *  print grid
     */
    void print_grid();

    /*
     *  calculate grid
     */
    void calculate_grid();


private:
    std::ostream &outputStream;
    static const int width = 9;
    static const int height = 9;
    char grid[width][height];
};




