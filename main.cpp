#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include "Window.h"
#include <ctime>
#include <cstdlib>


using namespace std;


int main() {

    Window window{cout};
    window.calculate_grid();
    window.print_grid();

    return 0;
}
