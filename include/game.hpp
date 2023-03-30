#ifndef GAME_HPP

#define GAME_HPP

#include <random>
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <string>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "carte.hpp"
#include "view.hpp"

struct MyPlayerInfo {
	Paquet		mesCartes;
	Joueur		myRole;
	std::string	myPseudo;
};

struct GameInfo {
	GameView									view;
	Joueur										who_deals;
	Joueur										who_speaks;
	Joueur										who_plays;
	Joueur										who_starts;
	bool										ice_speak;
	int											annonce_min;
	int											annonce_temp;
	bool										enchere_en_cours;
	bool										manche_terminee;
	bool										pli_termine;
	Paquet										pli_en_cours;
	std::vector<Enchere>						all_encheres;
	Enchere										current_enchere;
	std::vector<std::pair<std::string,Joueur>>	assoc_pseudo_role;
	std::vector<int>							taille_paquets;
	sf::TcpSocket 								client_socket;
	MyPlayerInfo								myPlayer;
};

class Jeu{
	public:
		Jeu() = default;
		~Jeu() = default;
		void createRandomPaquet();
		void coupePaquet(int where_to_cut);
		void distributionPaquet(Joueur who_cuts, int where_to_cut);
		void fusionPaquets();
		Enchere ask_enchere(Joueur who_bids);
		bool next_enchere(Joueur who_bids, bool first_enchere);
		bool paquetContientCouleur(Paquet my_paquet, Couleur my_couleur);
		void joue_pli();
		void comptePoints();
		void affichePaquetListe(Paquet paquet);
		void afficheAllPaquetsListe();
		Paquet getPaquet(Joueur j);

		Paquet paquet={}, nordP={}, estP={}, sudP={}, ouestP={}, defausseNS={}, defausseOE={}, dernier_pli={}, pli_actuel={};
		Couleur couleur_demandee = Couleur::Pique;
		Atout atout_actuel = Atout::Sa;
		std::vector<Paquet> allPaquets = {nordP,estP,sudP,ouestP};
		int tot_points_NS=0, tot_points_OE=0, points_NS=0, points_OE=0;
		Enchere current_enchere = {Joueur::Nord,10,Atout::Rien,false,false};
		std::vector<Enchere> all_enchere = {{Joueur::Nord,10,Atout::Rien,false,false},{Joueur::Est,10,Atout::Rien,false,false},{Joueur::Sud,10,Atout::Rien,false,false},{Joueur::Ouest,10,Atout::Rien,false,false}};
		Joueur who_cuts = Joueur::Sud, who_speaks = Joueur::Nord, who_plays = Joueur::Nord, dix_de_der_winner = Joueur::Nord;
		Joueur you = Joueur::Nord;
		bool enchere_en_cours = false;

	private:
		std::vector<Couleur> couleurs {Couleur::Pique, Couleur::Carreau, Couleur::Trefle, Couleur::Coeur};
		std::vector<Atout> atouts {Atout::Pique, Atout::Carreau, Atout::Trefle, Atout::Coeur, Atout::Passe, Atout::Ta, Atout::Sa};
		std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
};

#endif