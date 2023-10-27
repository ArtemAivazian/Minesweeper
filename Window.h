#pragma once
#include <iostream>
#include <random>
#include <sstream>
#include <utility>

#define ANSI_CLEAR "\x1B[2J\x1B[H"  // mazani terminalu

enum field_param {
    nothing,
    covered,
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

    void refresh_text();

    const std::vector<std::pair<size_t, size_t>>& get_coords() const;


private:
    std::ostream &outputStream;
    static const int width = 9;
    static const int height = 9;
    char grid[height][width];
    char copy[height][width];
    std::vector<std::pair<size_t, size_t>> coords;
    std::string text;
    bool once = true;
    bool gLost = false;
};




