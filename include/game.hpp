#ifndef GAME_HPP

#define GAME_HPP

#include <random>
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <string>

#include "carte.hpp"
#include "view.hpp"

class Jeu{
	public:
		Jeu() = default;
		~Jeu() = default;
		int joueurToInt(Joueur j);
		int atoutToInt(Atout a);
		int couleurToInt(Couleur c);
		std::string joueurToString(Joueur j);
		Atout stringToAtout(std::string s);
		Atout couleurToAtout(Couleur c);
		Couleur atoutToCouleur(Atout a);
		Couleur stringToCouleur(std::string s);
		Valeur stringToValeur(std::string s);
		Joueur intToJoueur(int i);
		int carteToPoint(Carte c, Atout a);
		bool auxCompareValeur(Valeur v1, Valeur v2, std::vector<Valeur> tab, int c);
		bool compareCarte(Carte c1, Carte c2, Atout atout_actuel, Couleur couleur_demandee);
		void createRandomPaquet();
		void coupePaquet(int where_to_cut);
		void distributionPaquet(Joueur who_cuts, int where_to_cut);
		void fusionPaquets();
		Enchere ask_enchere(Joueur who_bids);
		bool next_enchere(Joueur who_bids, bool first_enchere);
		Carte pose_carte(bool premier_a_jouer);
		void joue_pli();
		void next_to_play();
		void next_to_cut();
		void comptePoints();
		void affichePaquetListe(Paquet paquet);
		void afficheAllPaquetsListe();
		Paquet getPaquet(Joueur j);

		Paquet paquet={}, nordP={}, estP={}, sudP={}, ouestP={}, defausseNS={}, defausseOE={}, dernier_pli={}, pli_actuel={};
		Couleur couleur_demandee = Couleur::Pique;
		std::vector<Paquet> allPaquets = {nordP,estP,sudP,ouestP};
		int tot_points_NS=0, tot_points_OE=0, points_NS=0, points_OE=0;
		Enchere current_enchere = {Joueur::Nord,0,Atout::Passe,false,false};
		std::vector<Enchere> last_enchere = {{Joueur::Nord,0,Atout::Passe,false,false},{Joueur::Est,0,Atout::Passe,false,false},{Joueur::Sud,0,Atout::Passe,false,false},{Joueur::Ouest,0,Atout::Passe,false,false}};
		Joueur who_cuts = Joueur::Sud, who_plays = Joueur::Nord;
		Joueur you = Joueur::Sud;

	private:
		std::vector<Couleur> couleurs {Couleur::Coeur, Couleur::Carreau, Couleur::Pique, Couleur::Trefle};
		std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
		std::vector<Valeur> comp_non_atout = {Valeur::As,Valeur::Dix,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
		std::vector<Valeur> comp_atout = {Valeur::Valet,Valeur::Neuf,Valeur::As,Valeur::Dix,Valeur::Roi,Valeur::Dame,Valeur::Huit,Valeur::Sept};
};

#endif