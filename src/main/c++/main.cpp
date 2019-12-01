#include <iostream>

int main () {
    std::ofstream log;
    log.open("retro-manager.log", std::ios::out | std::ios::trunc );
    return 0;
}
