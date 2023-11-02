#include "Window.h"


Window::Window(std::ostream &outputStream) :
        output_stream(outputStream),
        start(std::chrono::high_resolution_clock::now())
    {}



void Window::refresh_text() {
    std::stringstream ss;
    if (victory) {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        ss << "You won! Your time: " << duration.count();
    } else if (flag_mode) {
        ss << "You are in flag mode!" << " ";
        ss << "Entered coords in flag mode: ";
        if (!get_marked_coords().empty()) {
            if (get_marked_coords().back().first)
                ss << get_marked_coords().back().first << " ";
            if (get_marked_coords().back().second)
                ss << get_marked_coords().back().second;
        }
    } else if (defeat) {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        ss << "You lost! Your time: " << duration.count();
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
                case '1': buffer << COLOR_BLUE << copy[j][i] << ANSI_COLOR_RESET; break;
                case '2': buffer << COLOR_GREEN << copy[j][i] << ANSI_COLOR_RESET; break;
                case '3': buffer << COLOR_RED << copy[j][i] << ANSI_COLOR_RESET; break;
                case '4': buffer << COLOR_PURPLE << copy[j][i] << ANSI_COLOR_RESET; break;
                default: buffer << copy[j][i]; break;
            }
            buffer << " ";
        }
        buffer << "|";
//        if (j == height / 2)
//            buffer << timer;
        buffer << "\n\r";
        for (size_t i = 0; i < 4 * width + 1; ++i)
            buffer << "-";
        buffer << std::endl;
    }
    buffer << "\r" << std::noskipws << text;
    output_stream << buffer.str() << std::endl;
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

void Window::original_grid() {
    init_grid();
    init_bombs();
    calculate_bombs_near();
}

bool Window::already_entered(std::pair<size_t, size_t>& point) {
    bool already= false;
    for (auto& p : coords) {
        if (p.first == point.first && p.second == point.second) {
            already = true;
        }
    }
    return already;
}
void Window::add_coordinates(size_t coord1, size_t coord2) {
    auto coord_pair = std::make_pair(coord1, coord2);
    //TO-DO filter added points
    //if already been entered
    //no ' ' and number
    if (!already_entered(coord_pair)) {
        if (copy[coord_pair.first - 1][coord_pair.second - 1] == 'F') {
            remove_flag(coord_pair);
            if (grid[coord_pair.first -1 ][coord_pair.second - 1] == 'X') {
                count--;
            }
        } else if (copy[coord_pair.first - 1][coord_pair.second - 1] == '0') {
            coords.push_back(coord_pair);
        }
    }
}

void Window::calculate_grid(int y, int x) {

    auto point = std::make_pair(static_cast<size_t>(y - 1), static_cast<size_t>(x - 1));//first - w; second - h
    switch (grid[y - 1][x - 1]) {
        case ' ':
            open_one_cell_around(point);
            break;
        case 'X':
            //lost the game
            defeat = true;
            break;
        default:
            open_number(point);
    }
}

bool Window::already_marked(std::pair<size_t, size_t>& point) {
    bool is_marked = false;
    for (auto& p : marked_coords) {
        if (p.first == point.first && p.second == point.second) {
            is_marked = true;
        }
    }
    return is_marked;
}

void Window::remove_flag(std::pair<size_t, size_t>& point) {
    //remove point from marked_coords vector
    auto newEnd = std::remove(marked_coords.begin(), marked_coords.end(), point);
    marked_coords.erase(newEnd, marked_coords.end());
    //replace cell with '0'
    copy[point.first - 1][point.second - 1] = '0';
}

void Window::calculate_marked_coordinates(size_t coord1, size_t coord2) {
    auto marked_coord_pair = std::make_pair(coord1, coord2);
    if (copy[coord1 - 1][coord2 - 1] == '0' || copy[coord1 - 1][coord2 - 1] == 'F') {
        if (already_marked(marked_coord_pair)) {
            remove_flag(marked_coord_pair);
            if (grid[marked_coord_pair.first - 1][marked_coord_pair.second - 1] == 'X') {
                count--;
            }
        } else {
            if (grid[marked_coord_pair.first - 1][marked_coord_pair.second - 1] == 'X') {
                count++;
            }
            if (count == bomb_count) {
                victory = true;
            }
            marked_coords.push_back(marked_coord_pair);
            copy[coord1 - 1][coord2 - 1] = 'F';
        }
    }
}


void Window::open_one_cell_around(std::pair<size_t, size_t>& point) {
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

bool Window::is_visited(std::pair<size_t, size_t> point) {
    bool isVisited = false;
    for (auto& p : visited) {
        if (p.first == point.first && p.second == point.second) {
            isVisited = true;
        }
    }
    return isVisited;
}

void Window::open_number(std::pair<size_t, size_t>& point) {
    copy[point.first][point.second] = grid[point.first][point.second];
}

bool Window::is_lost() const {
    return defeat;
}

const std::vector<std::pair<size_t, size_t>> &Window::get_coords() const {
    return coords;
}

bool Window::is_flag_mode() const {
    return flag_mode;
}

void Window::set_flag_mode(bool mode) {
    flag_mode = mode;
}


bool Window::is_game_won() const {
    return victory;
}

const std::vector<std::pair<size_t, size_t>> &Window::get_marked_coords() const {
    return marked_coords;
}



