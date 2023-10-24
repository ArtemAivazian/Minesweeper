#include "Window.h"

Window::Window(std::ostream &outputStream)  :
        outputStream(outputStream)
        { }



void Window::print_grid() {
    std::stringstream buffer;
    buffer << "\r";
    for (size_t i = 0; i < 4 * width + 1; ++i)
        buffer << "-";
    buffer << std::endl;
    for (size_t j = 0; j < height; ++j) {
        buffer << "\r";
        for (size_t i = 0; i < width; ++i) {
            buffer << "| ";
            buffer << grid[j][i];
            buffer << " ";
        }
        buffer << "|";
        buffer << "\n\r";
        for (size_t i = 0; i < 4 * width + 1; ++i)
            buffer << "-";
        buffer << std::endl;
    }
    buffer << "\r" << std::noskipws << "hello";
    outputStream << buffer.str() << std::endl;
}

void Window::calculate_grid() {
    // Seed the random number generator with the current time
    std::srand(std::time(nullptr));

    //init
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = '0';
        }
    }

    //init bombs
    int count = 0;
    while (count < 8) {
        int i = std::rand() % height;
        int j = std::rand() % width;
        if (grid[i][j] == '0') {
            grid[i][j] = 'X';
            count++;
        }
    }

    //calculate count of bombs near in a particular cell
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == 'X') {
                continue;
            }
            int countBombs = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    int newX = x + dx;
                    int newY = y + dy;

                    if (newX >= 0 &&
                        newX < width &&
                        newY >= 0 &&
                        newY < height &&
                        grid[newY][newX] == 'X') {
                        countBombs++;
                    }
                }
            }
            grid[y][x] = '0' + countBombs;
        }
    }
}

