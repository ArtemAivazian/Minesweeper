#include "Window.h"


Window::Window(std::ostream &outputStream) :
    outputStream(outputStream)
    {}


void Window::refresh_text() {
    std::stringstream ss;
    if (gLost)
        ss << "You lost!";
    else {
        ss << "Entered coords: ";
        if (!get_coords().empty()) {
            if (get_coords().back().first)
                ss << get_coords().back().first << " ";
            if (get_coords().back().second)
                ss << get_coords().back().second;
        }
    }
    text = ss.str();
}


void Window::print_grid() {
    std::stringstream buffer;
    buffer << ANSI_CLEAR;
    buffer << "\r";
    for (size_t i = 0; i < 4 * width + 1; ++i)
        buffer << "-";
    buffer << std::endl;
    for (size_t j = 0; j < height; ++j) {
        buffer << "\r";
        for (size_t i = 0; i < width; ++i) {
            buffer << "| ";
            buffer << copy[j][i];
            buffer << " ";
        }
        buffer << "|";
        buffer << "\n\r";
        for (size_t i = 0; i < 4 * width + 1; ++i)
            buffer << "-";
        buffer << std::endl;
    }
    buffer << "\r" << std::noskipws << text;
    outputStream << buffer.str() << std::endl;
}

void Window::init_grid() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = '0';
        }
    }
}
void Window::init_bombs() {
    // Seed the random number generator with the current time
    std::srand(std::time(nullptr));
    int count = 0;
    while (count < 8) {
        int i = std::rand() % height;
        int j = std::rand() % width;
        if (grid[i][j] == '0') {
            grid[i][j] = 'X';
            count++;
        }
    }
}
void Window::calculate_bombs_near() {
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
            grid[y][x] = (countBombs == 0) ? ' ' : ('0' + countBombs);
        }
    }
}

void Window::calculate_grid() {
    if (coords.empty()) {
        //TO-DO create separate method for copy grid init
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                copy[i][j] = '0';
            }
        }
    } else {
        if (once) {
            original_grid();
            //TO-DO create separate method for copy grid init
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    copy[i][j] = '0';
                }
            }
            once = false;
        }
        auto point = coords.at(coords.size() - 1);//first - w; second - h
        switch (grid[point.first - 1][point.second - 1]) {
            case ' ':
                open_one_cell_around(point);
            break;
            case 'X':
                //lost the game
                gLost = true;
            case 'F':
                //FLAG TO-DO
            break;
            default:
                open_number(point);
        }
    }

}

void Window::original_grid() {
    init_grid();
    init_bombs();
    calculate_bombs_near();
}

void Window::add_coordinates(size_t coord1, size_t coord2) {
    auto coord_pair = std::make_pair(coord1, coord2);
    coords.push_back(coord_pair);
}

void Window::open_one_cell_around(std::pair<size_t, size_t> &point) {
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int newX = static_cast<int>(point.second) - 1 + dx;
            int newY = static_cast<int>(point.first) - 1 + dy;

            if (newX >= 0 &&
                newX < width &&
                newY >= 0 &&
                newY < height &&
                grid[newY][newX] != 'X') {
                copy[newY][newX] = grid[newY][newX];
            }
        }
    }
}

void Window::open_number(std::pair<size_t, size_t>& point) {
    copy[point.first - 1][point.second - 1] = grid[point.first - 1][point.second - 1];
}

bool Window::is_Lost() const {
    return gLost;
}

const std::vector<std::pair<size_t, size_t>> &Window::get_coords() const {
    return coords;
}



