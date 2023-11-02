#pragma once
#include <iostream>
#include <random>
#include <sstream>
#include <utility>
#include <algorithm>
//save data
#include <fstream>
#include "json.hpp"

#define ANSI_CLEAR "\x1B[2J\x1B[H"  //clear terminal
#define COLOR_RED   "\x1B[91m"
#define COLOR_GREEN "\x1B[92m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_PURPLE "\x1B[35m"
#define ANSI_COLOR_RESET "\x1B[m"  //reset colors

using json = nlohmann::json;

class Window {
public:
    /*
     * window constructor
     */
    explicit Window(std::ostream &outputStream);
    /*
     *  print displayed grid to console
     *  is loaded - if it is false, will be printed init text
     */
    void print_grid(bool& is_loaded);
    /*
     * update text under the displayed grid
     * is loaded - if it is false, will be calculated init text
     */
    void refresh_text(bool& is_loaded);
    /*
     *  compute entered coords and update
     */
    void calculate_grid(int y, int x);
    /*
     * add coords of entered points in non-flag mode
     */
    void add_coordinates(size_t coord1, size_t coord2);
    /*
     * add marked point to vector or delete it, if it has been already marked
     */
    void calculate_marked_coordinates(size_t coord1, size_t coord2);
    /*
     * Initialize original grid:
     * 1. initialize each cell with '0'
     * 2. generate bombs randomly
     * 3. calculate count of bombs around the particular cell
     */
    void original_grid();
    /*
     * initialize displayed grid with '0'
     */
    void init_copy_grid();
    /*
     * save to JSON file both grids: original and displayed
     */
    void save_game(const std::string& file_name);

    /*
     * load grids data from json file "grids.json"
     */
    void load_json();
    //getters and setters

    const std::vector<std::pair<size_t, size_t>>& get_coords() const;

    const std::vector<std::pair<size_t, size_t>>& get_marked_coords() const;

    bool is_flag_mode() const;

    void set_flag_mode(bool mode);

    bool is_game_won() const;

    bool is_lost() const;

private:
    std::ostream &output_stream; //Output current reference (std::cout)
    static const int width = 9; //Default grid width
    static const int height = 9; //Default grid height
    int bomb_count = 8;//Max count of bombs that can be on the grid
    char grid[height][width]; //Original grid
    char copy[height][width]; //Displayed grid
    std::vector<std::pair<size_t, size_t>> coords; //vector of points, that was entered in non-flag mode
    std::vector<std::pair<size_t, size_t>> marked_coords; //vector of points, that was entered in flag mode
    std::string text; //displayed info text under the displayed grid
    bool defeat = false; //a flag that determines whether a player has lost
    bool victory = false; //a flag that determines whether a player has won
    bool flag_mode = false; //a flag that determines whether a player in a flag mode
    int count = 0; //count of correct marked flags
    std::vector<std::pair<size_t, size_t>> visited; //vector of points that has already opened
    //Timer
//    std::chrono::time_point<std::chrono::steady_clock> start, end;
//    std::chrono::duration<float> duration;


    //private methods

    //returns true, if point already visited
    bool is_visited(std::pair<size_t, size_t> point);
    /*
     * determines, if the entered marked point has already been marked
     * return true, if point has already been marked
     */
    bool already_marked(std::pair<size_t, size_t>& point);
    /*
     * removes flag from the cell
     */
    void remove_flag(std::pair<size_t, size_t>& point);
    /*
     * determines, if the entered point has already been entered
     * returns true, if the point has already been entered
     */
    bool already_entered(std::pair<size_t, size_t>& point);
    /*
     * calculate count of bombs around the particular cell
     */
    void calculate_bombs_near();
    /*
     * initialize each cell with '0' of original grid
     */
    void init_grid();
    /*
     * initialize bombs on original grid
     */
    void init_bombs();
    /*
     * show grid with selected point
     */
    void open_number(std::pair<size_t, size_t>& point);
    /*
     * opens one cell in each direction around cell that is empty and is being clicked in non-flag mode
     */
    void open_one_cell_around(std::pair<size_t, size_t>& point);
};




