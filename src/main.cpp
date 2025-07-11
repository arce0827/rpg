#include "core/GameEngine.hpp"
#include <iostream>

int main() {
    try{
        GameEngine game;
        game.run();
    }
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}