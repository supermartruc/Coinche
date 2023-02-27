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
		std::vector<Couleur> couleurs {Couleur::Coeur, Couleur::Carreau, Couleur::Pique, Couleur::Trefle};
		std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
		Paquet createPaquet();
		void initPaquet(Paquet last_game_paquet);
		Paquet coupePaquet(Paquet last_game_paquet, int where_to_cut);

	private:
		std::vector<Couleur> couleurs {Couleur::Coeur, Couleur::Carreau, Couleur::Pique, Couleur::Trefle};
		std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
		Paquet nordP, sudP, ouestP, estP;
		std::vector<Paquet> allPaquets = {nordP,sudP,ouestP,estP};
};

#endif