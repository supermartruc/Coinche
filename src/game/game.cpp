#include"game.hpp"

std::mt19937 gen(std::random_device{}());

std::ostream &operator<<(std::ostream &os, Couleur c) {
	switch (c) {
		case Couleur::Pique:
			os << "Pique";
			break;
		case Couleur::Coeur:
			os << "Coeur";
			break;
		case Couleur::Trefle:
			os << "Trefle";
			break;
		case Couleur::Carreau:
			os << "Carreau";
			break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, Valeur v) {
	switch (v) {
		case Valeur::As:
			os << "As";
			break;
		case Valeur::Roi:
			os << "Roi";
			break;
		case Valeur::Dame:
			os << "Dame";
			break;
		case Valeur::Valet:
			os << "Valet";
			break;
		case Valeur::Dix:
			os << "Dix";
			break;
		case Valeur::Neuf:
			os << "Neuf";
			break;
		case Valeur::Huit:
			os << "Huit";
			break;
		case Valeur::Sept:
			os << "Sept";
			break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, Carte c) {
	auto [val, coul] = c;
	os << val << " de " << coul;
	return os;
}

int Jeu::joueurToInt(Joueur j){
    switch(j){
        case Joueur::Nord:
            return 0;
            break;
        case Joueur::Est:
            return 1;
            break;
        case Joueur::Sud:
            return 2;
            break;
        case Joueur::Ouest:
            return 3;
            break;
    }
    return -1;
}

Paquet Jeu::createRandomPaquet(){
	Paquet provPaquet = {};
	for (auto c = couleurs.begin(); c != couleurs.end(); c++){
		for (auto v = valeurs.begin(); v != valeurs.end(); v++){
			provPaquet.push_back(std::tuple {*v,*c});
		}
	}
	std::shuffle (provPaquet.begin(), provPaquet.end(), gen);
	return provPaquet;
}

Paquet Jeu::coupePaquet(Paquet last_game_paquet, int where_to_cut){ 
    /*Convention : couper APRES la carte d'index where_to_cut ; ex : wtc = 0 -> on place la carte du haut en bas..*/
    Paquet newPaquet;
    int n = last_game_paquet.size();
    for (int i = 1+where_to_cut; i < n; i++){
        newPaquet.push_back(last_game_paquet[i]);
    }
    for (int i = 0; i < 1+where_to_cut; i++) {
        newPaquet.push_back(last_game_paquet[i]);
    }
    return newPaquet;
}

void Jeu::initPaquet(Paquet last_game_paquet, Joueur who_cuts, int where_to_cut){
    int who_cuts_int = joueurToInt(who_cuts);
    Paquet paquet_coupe = coupePaquet(last_game_paquet, where_to_cut);
    for (int i=0;i<12;i++){
        allPaquets[(i/3+who_cuts_int+2)%4].push_back(paquet_coupe[i]);
    }
    for (int i=0;i<8;i++){
        allPaquets[(i/2+who_cuts_int+2)%4].push_back(paquet_coupe[i+12]);
    }
    for (int i=0;i<12;i++){
        allPaquets[(i/3+who_cuts_int+2)%4].push_back(paquet_coupe[i+20]);
    }
}

void Jeu::affichePaquetListe(Paquet paquet){
    int n = paquet.size();
    std::cout << "[";
    for (int i=0; i < n-1; i++){
        std::cout << paquet[i] << " ; ";
    }
    std::cout << paquet[n-1];
    std::cout << "]" << std::endl;
}

void Jeu::afficheAllPaquetsListe(){
    std::cout << std::endl << "Nord : ";
    affichePaquetListe(allPaquets[0]);
    std::cout << "Est : ";
    affichePaquetListe(allPaquets[1]);
    std::cout << "Sud : ";
    affichePaquetListe(allPaquets[2]);
    std::cout << "Ouest : ";
    affichePaquetListe(allPaquets[3]);
}