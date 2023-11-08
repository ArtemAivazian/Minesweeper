#include "window.hpp"

#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <future>
#include <fstream>
#include <cstdio>
#include <cstdlib>


void set_raw(bool set) {
    if (set) {
        system("stty raw");  // enable raw
    } else {
        system("stty -raw"); // disable raw
    }
}

class Game {
public:
    explicit Game() : quit(false)
    {
        win = std::make_unique<window>(std::cout);
    }

    bool file_exists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }


    void delete_file(const std::string& filename) {
        std::remove(filename.c_str());
    }



    void load_game() {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::unique_lock<std::mutex> lg(mutex);

        if (file_exists("grid.txt") && file_exists("copy.txt")) {
            cvar.notify_one();
            cvar.wait(lg);
            if (is_loaded)
                win->load_txt();
            else {
                is_loaded = true;
                //delete files
                delete_file("grid.txt");
                delete_file("copy.txt");
                win->original_grid();
                win->init_copy_grid();
            }
        } else {
            is_loaded = true;
            win->original_grid();
            win->init_copy_grid();
        }
        cvar.notify_one();
    }

    void input() {
        std::unique_lock<std::mutex> lg(mutex);
        lg.unlock();

        if (quit)
            return;
        if (win->is_lost()) {
            return;
        }
        if (win->is_game_won()) {
            return;
        }

        char c;
        std::cin >> c;
        handle_input(c);
        cvar.notify_one();
    }


    void output() {
        std::unique_lock<std::mutex> ul(mutex);
        cvar.wait(ul);

        win->refresh_text(is_loaded);
        win->print_grid(is_loaded);
    }

    bool is_Lost(){
        return win->is_lost();
    }

    bool is_Win() {
        return win->is_game_won();
    }

    bool get_quit() const {
        return quit;
    }
    void set_quit(const bool qq) {
        quit = qq;
    }
private:
    bool quit;
    bool is_loaded = false;
    std::unique_ptr<window> win;
    std::mutex mutex;
    std::condition_variable cvar;

    void handle_input(char c) {
        if (c == 's'){
            //save game
            win->save_grid("grid.txt");
            win->save_copy("copy.txt");
        } else if (c == 'y') {
            is_loaded = true;
        } else if (c == 'n') {
            is_loaded = false;
        } else if (c == 'q') {
            set_quit(true);
        } else if (c == 'f' && win->is_flag_mode()) {
            win->set_flag_mode(false);
        } else if (c == 'f' && !win->is_flag_mode()){
            win->set_flag_mode(true);
        }else if (c >= '1' && std::cin.peek() == ' ') {
            int digit1 = c - '0';
            int digit2;
            std::cin >> digit2;
            if (win->is_flag_mode()) {
                win->calculate_marked_coordinates(digit1, digit2);
                if (is_Win()) {
                    if (file_exists("grid.txt") && file_exists("copy.txt")) {
                        delete_file("grid.txt");
                        delete_file("copy.txt");
                    }
                    set_quit(true);
                }
            } else {
                win->add_coordinates(digit1, digit2);
                win->calculate_grid(digit1, digit2);
                if (is_Lost()) {
                    if (file_exists("grid.txt") && file_exists("copy.txt")) {
                        delete_file("grid.txt");
                        delete_file("copy.txt");
                    }
                    set_quit(true);
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    //--help command implementation
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "--help") {
            std::cout << "Welcome to HELP page of the game Minesweeper!" << std::endl
                      << std::endl
                      << "Game tips:" << std::endl
                      << "1.How to enter coordinates of the point?" << std::endl
                      << "[row] [col]" << std::endl
                      << "2.How to switch mode to flag mode?" << std::endl
                      << "Enter 'f' from the keyboard" << std::endl
                      << "3.How to remove flag from the grid?" << std::endl
                      << "Enter coordinates of the point that you want to remove in flag mode from the keyboard" << std::endl
                      << "4.How to save game process" << std::endl
                      << "Enter 's' from the keyboard" << std::endl
                      << "5.How to quit the game?" << std::endl
                      << "Enter 'q' from the keyboard" << std::endl
                      << std::endl
                      << "Game rules:" << std::endl
                      << "You need to correctly mark all bombs by flags." << std::endl
                      << "If you select in non-flag mode coordinates that is bomb, you will lost" << std::endl;
            std::string start;
            std::cout << "Type 'start' to start game or 'exit' to exit: ";
            std::cin >> start;
            if (start == "start") {
                break;
            } else if (start == "exit") {
                return 0;
            }
        }
    }

    auto initThread = [](Game &g) {
        g.load_game();
    };

    auto outputThread = [](Game &g) {
        while (true) {
            g.output();
            if (g.get_quit()) {
                break;
            }
        }
    };

    auto inputThread = [](Game &g) {
        while (true) {
            g.input();
            if (g.get_quit()) {
                break;
            }
        }
    };

    set_raw(true);
    Game game;
    std::thread t1(initThread, std::ref(game));
    std::thread t2(outputThread, std::ref(game));
    std::thread t3(inputThread, std::ref(game));
    t1.join();
    t2.join();
    t3.join();
    set_raw(false);

}
