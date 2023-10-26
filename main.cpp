#include "Window.h"

#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <future>

void set_raw(bool set) {
    if (set) {
        system("stty raw");  // enable raw
    } else {
        system("stty -raw"); // disable raw
    }
}

class Game {
public:
    /*
    * Konstruktor a destruktor tridy tic_tac_toe
    */
    explicit Game() :
            quit(false)
    {
        win = std::make_unique<Window>(std::cout);
    }

    void input() {
        std::unique_lock<std::mutex> lg(mutex);
        lg.unlock();
        if (quit)
            return;
        char c;
        std::cin >> c;
        if (c == 'q') {
            set_quit(true);
            cvar.notify_one();
        } else if (c >= '1' && std::cin.peek() == ' ') {
            int digit1 = c - '0';
            int digit2;
            std::cin >> digit2;
            //TO-DO check valid input
            win->add_coordinates(digit1, digit2);
            cvar.notify_one();
        }
    }

    void compute() {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::unique_lock<std::mutex> ul(mutex);
        win->calculate_grid();
        cvar.notify_one();
    }

    void output() {
        std::unique_lock<std::mutex> ul(mutex);
        cvar.wait(ul);
        win->print_grid();
    }

    bool is_Lost(){
        return win->is_Lost();
    }

    bool get_quit() const {
        return quit;
    }
    void set_quit(const bool qq) {
        quit = qq;
    }
private:
    bool quit;
    std::unique_ptr<Window> win;
    std::mutex mutex;
    std::condition_variable cvar;
};

int main() {
    auto computeThread = [](Game& g) {
        bool q = false;
        while (!q) {
            g.compute();
            if (g.is_Lost()) {
                g.set_quit(true);
            }
            q = g.get_quit();
        }
    };
    auto outputThread = [](Game& g) {
        bool q = false;
        while (!q) {
            g.output();
            if (g.is_Lost()) {
                g.set_quit(true);
            }
            q = g.get_quit();
        }
    };
    auto inputThread = [](Game& g) {
        bool q = false;
        while (!q) {
            g.input();
            if (g.is_Lost()) {
                g.set_quit(true);
            }
            q = g.get_quit();
        }
    };

    set_raw(true);
    Game game;
    std::thread t1(computeThread, std::ref(game));
    std::thread t2(outputThread, std::ref(game));
    std::thread t3(inputThread, std::ref(game));


    t1.join();
    t2.join();
    t3.join();
    set_raw(false);

    return 0;
}
