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
        if (win->is_Lost())
            return;
        if (win->isGWin())
            return;

        if (once) {
            win->original_grid();
            win->init_copy_grid();
            once = false;
            cvar.notify_one();
        }
        if (win->isFlagMode()) {
            char c;
            std::cin >> c;
            if (c == 'q') {
                set_quit(true);
                cvar.notify_one();
            } else if (c == 'f'){
                win->setFlagMode(false);
                cvar.notify_one();
            } else if (c >= '1' && std::cin.peek() == ' ') {
                int digit1 = c - '0';
                int digit2;
                std::cin >> digit2;

                win->add_marked_coordinates(digit1, digit2);
                win->calculate_grid();

                if (is_Win()) {
                    set_quit(true);
                }
                cvar.notify_one();
            }
        }
        char c;
        std::cin >> c;
        if (c == 'q') {
            set_quit(true);
            cvar.notify_one();
        } else if (c == 'f'){
            win->setFlagMode(true);
            cvar.notify_one();
        } else if (c >= '1' && std::cin.peek() == ' ') {
            int digit1 = c - '0';
            int digit2;
            std::cin >> digit2;

            win->add_coordinates(digit1, digit2);
            win->calculate_grid();

            if (is_Lost()) {
                set_quit(true);
            }
            cvar.notify_one();
        }

    }

//    void compute() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(300));
//        std::unique_lock<std::mutex> ul(mutex);
////        mutex.lock();
//
//        win->calculate_grid();
//
//        std::cout << "Compute" << std::endl;
//
//        cvar.notify_one();
//    } //unlock

    void output() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::unique_lock<std::mutex> ul(mutex);
        cvar.wait(ul);

        win->refresh_text();
        win->print_grid();
    }

    bool is_Lost(){
        return win->is_Lost();
    }

    bool is_Win() {
        return win->isGWin();
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
    bool once = true;
};

int main() {
//    auto computeThread = [](Game& g) {
//        while (true) {
//            g.compute();
//            if (g.is_Lost() || g.get_quit()) {
//                break;
//            }
//        }
//    };
    auto outputThread = [](Game& g) {
        while (true) {
            g.output();
            if (g.is_Lost() || g.get_quit()) {
                break;
            }
        }
    };

    auto inputThread = [](Game& g) {
        while (true) {
            g.input();
            if (g.get_quit() || g.is_Lost()) {
                break;
            }
        }
    };


    set_raw(true);
    Game game;
//    std::thread t1(computeThread, std::ref(game));
    std::thread t3(inputThread, std::ref(game));
    std::thread t2(outputThread, std::ref(game));


//    t1.join();
    t2.join();
    t3.join();
    set_raw(false);

    return 0;
}
