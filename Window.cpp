#include "Window.h"


Window::Window(std::ostream &outputStream) :
    outputStream(outputStream)
    {}


void Window::refresh_text() {
    std::stringstream ss;
    if (gWin) {
        ss << "You won!";
    } else if (flag_mode) {
        ss << "You are in flag mode!" << " ";
        ss << "Entered coords in flag mode: ";
        if (!get_coords().empty()) {
            if (get_marked_coords().back().first)
                ss << get_marked_coords().back().first << " ";
            if (get_marked_coords().back().second)
                ss << get_marked_coords().back().second;
        }
    } else if (gLost) {
        ss << "You lost!";
    } else {
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
    buffer << ANSI_CLEAR << ANSI_COLOR_RESET;
    buffer << "\r";
    for (size_t i = 0; i < 4 * width + 1; ++i)
        buffer << "-";
    buffer << std::endl;
    for (size_t j = 0; j < height; ++j) {
        buffer << "\r";
        for (size_t i = 0; i < width; ++i) {
            buffer << "| ";
            switch (copy[j][i]) {
                case '1': buffer << COLOR_BLUE << copy[j][i]; break;
                case '2': buffer << COLOR_GREEN << copy[j][i]; break;
                case '3': buffer << COLOR_RED << copy[j][i]; break;
                case '4': buffer << COLOR_PURPLE << copy[j][i]; break;
                default: buffer << copy[j][i]; break;
            }
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

void Window::init_copy_grid() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copy[i][j] = '0';
        }
    }
}
void Window::init_bombs() {
    // Seed the random number generator with the current time
    std::srand(std::time(nullptr));
    int count = 0;
    while (count < bomb_count) {
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
    if (flag_mode){
        auto marked_point = marked_coords.at(marked_coords.size() - 1);
        copy[marked_point.first - 1][marked_point.second - 1] = 'F';
    } else {
        if (coords.empty()) {
            //TO-DO create separate method for copy grid init
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    copy[i][j] = '0';
                }
            }
        } else {
            auto point = coords.at(coords.size() - 1);//first - w; second - h
            switch (grid[point.first - 1][point.second - 1]) {
                case ' ':
                    open_one_cell_around(point);
                    break;
                case 'X':
                    //lost the game
                    gLost = true;
                    break;
                default:
                    open_number(point);
            }
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

void Window::add_marked_coordinates(size_t coord1, size_t coord2) {
    if (count == bomb_count) {
        gWin = true;
    }
    auto marked_coord_pair = std::make_pair(coord1, coord2);
    if (grid[marked_coord_pair.first - 1][marked_coord_pair.second - 1] == 'X'){
        count++;
    }
    marked_coords.push_back(marked_coord_pair);
}

void recursion() {

}


void Window::open_one_cell_around(std::pair<size_t, size_t>& point) {
//    for (int dy = -7; dy <= 7; dy++) {
//        for (int dx = -7; dx <= 7; dx++) {
//            int newX = static_cast<int>(point.second) - 1 + dx;
//            int newY = static_cast<int>(point.first) - 1 + dy;
//
//            if (newX >= 0 &&
//                newX < width &&
//                newY >= 0 &&
//                newY < height &&
//                grid[newY][newX] != 'X') {
//                copy[newY][newX] = grid[newY][newX];
//            }
//        }
//    }
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int newX = static_cast<int>(point.second) - 1 + dx;
            int newY = static_cast<int>(point.first) - 1 + dy;
            auto p = std::make_pair(static_cast<size_t>(newY), static_cast<size_t>(newX));
            if (newX >= 0 &&
                newX < width &&
                newY >= 0 &&
                newY < height &&
                grid[newY][newX] != 'X' &&
                !is_visited(p))
            {
                visited.push_back(p);
                copy[newY][newX] = grid[newY][newX];
                if (grid[newY][newX] != 'X' && grid[newY][newX] != ' ')
                    continue;
                newX++; newY++;
                auto newPoint = std::make_pair(static_cast<size_t>(newY), static_cast<size_t>(newX));
                open_one_cell_around(newPoint);
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

void Window::set_lost(bool lost) {
    gLost = lost;
}

bool Window::isFlagMode() const {
    return flag_mode;
}

void Window::setFlagMode(bool flagMode) {
    flag_mode = flagMode;
}

bool Window::isGWin() const {
    return gWin;
}

void Window::setGWin(bool gWin) {
    Window::gWin = gWin;
}

int Window::getCount() const {
    return count;
}

void Window::setCount(int count) {
    Window::count = count;
}

const std::vector<std::pair<size_t, size_t>> &Window::get_marked_coords() const {
    return marked_coords;
}

bool Window::is_visited(std::pair<size_t, size_t> point) {
    bool isVisited = false;
    for (auto& p : visited) {
        if (p.first == point.first && p.second == point.second) {
            isVisited = true;
        }
    }
    return isVisited;
}



