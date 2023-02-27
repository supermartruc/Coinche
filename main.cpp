#include<iostream>
#include"game.hpp"

int main() {

    Jeu game;
    Paquet mypaquet = game.createPaquet();
    std::cout << mypaquet[0] << std::endl;
    return 0;
}