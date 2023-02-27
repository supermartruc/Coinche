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
		int joueurToInt(Joueur j);
		Paquet createRandomPaquet();
		Paquet coupePaquet(Paquet last_game_paquet, int where_to_cut);
		void initPaquet(Paquet last_game_paquet, Joueur who_cuts, int where_to_cut);
		void affichePaquetListe(Paquet paquet);
		void afficheAllPaquetsListe();


	private:
		Paquet nordP, estP, sudP, ouestP, defausseNS, defausseOE, dernier_pli;
		std::vector<Paquet> allPaquets = {nordP,estP,sudP,ouestP};
};

#endif