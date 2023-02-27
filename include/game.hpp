#ifndef GAME_HPP

#define GAME_HPP

#include <random>
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

#include "carte.hpp"
#include "view.hpp"

class Jeu{
	public:
		Jeu() = default;
		~Jeu() = default;
		Paquet createPaquet();

	private:
		Paquet nordP, sudP, ouestP, estP;	
};

#endif