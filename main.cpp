#include "Window.h"

#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <future>
#include <filesystem>

namespace fs = std::filesystem;

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
        win = std::make_unique<Window>(std::cout);
    }

    void load_game() {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::unique_lock<std::mutex> lg(mutex);

        if (fs::exists("grids.json")) {
            cvar.notify_one();
            cvar.wait(lg);
            if (is_loaded)
                win->load_json();
            else {
                is_loaded = true;
                //delete "grids.json"
                fs::remove("grids.json");
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
    std::unique_ptr<Window> win;
    std::mutex mutex;
    std::condition_variable cvar;

    void handle_input(char c) {
        if (c == 'y') {
            is_loaded = true;
        } else if (c == 'n') {
            is_loaded = false;
        } else if (c == 'q') {
            //save game
            win->save_game("grids.json");
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
                    if (fs::exists("grids.json"))
                        fs::remove("grids.json");
                    set_quit(true);
                }
            } else {
                win->add_coordinates(digit1, digit2);
                win->calculate_grid(digit1, digit2);
                if (is_Lost()) {
                    if (fs::exists("grids.json"))
                        fs::remove("grids.json");
                    set_quit(true);
                }
            }
        }
    }
};

int main() {

    auto initThread = [](Game& g) {
        g.load_game();
    };

    auto outputThread = [](Game& g) {
        while (true) {
            g.output();
            if (g.get_quit()) {
                break;
            }
        }
    };

    auto inputThread = [](Game& g) {
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

    return 0;
}
