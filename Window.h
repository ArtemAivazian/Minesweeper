#pragma once
#include <iostream>
#include <random>
#include <sstream>
#include <utility>

#define ANSI_CLEAR "\x1B[2J\x1B[H"  //clear terminal
#define COLOR_RED   "\x1B[91m"
#define COLOR_GREEN "\x1B[92m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_PURPLE "\x1B[35m"
#define ANSI_COLOR_RESET "\x1B[m"  //reset colors


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

    /*
     * calculate count of bombs near cell
     */
    void calculate_bombs_near();

    /*
     * initialize grid
     */
    void init_grid();

    /*
     * initialize bombs
     */
    void init_bombs();

    /*
     * show grid with coordinate selected
     */
    void open_number(std::pair<size_t, size_t>& point);

    /*
     *
     */
    void open_one_cell_around(std::pair<size_t, size_t>& point);

    void add_coordinates(size_t coord1, size_t coord2);

    void original_grid();

    bool is_Lost() const;
    void set_lost(bool lost);

    void refresh_text();

    const std::vector<std::pair<size_t, size_t>>& get_coords() const;
    void init_copy_grid();

    bool isFlagMode() const;

    void setFlagMode(bool flagMode);

    void add_marked_coordinates(size_t coord1, size_t coord2);


    bool isGWin() const;

    void setGWin(bool gWin);

    int getCount() const;

    void setCount(int count);

    const std::vector<std::pair<size_t, size_t>> &get_marked_coords() const;

    bool is_visited(std::pair<size_t, size_t> point);


private:
    std::ostream &outputStream;
    static const int width = 9;
    static const int height = 9;
    char grid[height][width];
    char copy[height][width];
    std::vector<std::pair<size_t, size_t>> coords;
    std::vector<std::pair<size_t, size_t>> marked_coords;
    std::string text;
    bool once = true;
    bool gLost = false;
    bool gWin = false;
    bool flag_mode = false;
    int count = 0;
    int bomb_count = 8;
    std::vector<std::pair<size_t, size_t>> visited;
};




