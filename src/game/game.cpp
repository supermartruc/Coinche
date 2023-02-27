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

Joueur Jeu::intToJoueur(int i){
    switch(i){
        case 0:
            return Joueur::Nord;
            break;
        case 1:
            return Joueur::Est;
            break;
        case 2:
            return Joueur::Sud;
            break;
        case 3:
            return Joueur::Ouest;
            break;
    }
    return Joueur::Nord;
}

int Jeu::atoutToInt(Atout a){
    switch(a){
        case Atout::Pique:
            return 0;
            break;
        case Atout::Carreau:
            return 1;
            break;
        case Atout::Coeur:
            return 2;
            break;
        case Atout::Trefle:
            return 3;
            break;
        case Atout::Ta:
            return 4;
            break;
        case Atout::Sa:
            return 5;
            break;
        case Atout::Passe:
            return 6;
            break;
    }
    return -1;
}

int Jeu::couleurToInt(Couleur c){
    switch(c){
        case Couleur::Pique:
            return 0;
            break;
        case Couleur::Carreau:
            return 1;
            break;
        case Couleur::Coeur:
            return 2;
            break;
        case Couleur::Trefle:
            return 3;
            break;
    }
    return -1;
}

Atout Jeu::couleurToAtout(Couleur c){
    switch(c){
        case Couleur::Pique:
            return Atout::Pique;
            break;
        case Couleur::Carreau:
            return Atout::Carreau;
            break;
        case Couleur::Coeur:
            return Atout::Coeur;
            break;
        case Couleur::Trefle:
            return Atout::Trefle;
            break;
    }
    return Atout::Passe;
}

Couleur Jeu::atoutToCouleur(Atout a){
    switch(a){
        case Atout::Pique:
            return Couleur::Pique;
            break;
        case Atout::Carreau:
            return Couleur::Carreau;
            break;
        case Atout::Coeur:
            return Couleur::Coeur;
            break;
        case Atout::Trefle:
            return Couleur::Trefle;
            break;
        default:
            return Couleur::Pique;
            break;
    }
    return Couleur::Pique;
}

int Jeu::carteToPoint(Carte c, Atout a){
    auto [val, coul] = c;
    bool is_atout=false;
    if ((a==Atout::Sa)||(atoutToInt(a)==couleurToInt(coul))){
        is_atout = true;
    }
    switch (val){
        case Valeur::As:
            return 11;
            break;
        case Valeur::Dix:
            return 10;
            break;
        case Valeur::Roi:
            return 4;
            break;
        case Valeur::Dame:
            return 3;
            break;
        case Valeur::Huit:
            return 0;
            break;
        case Valeur::Sept:
            return 0;
            break;
        case Valeur::Valet:
            if (is_atout) {return 20;} else {return 2;}
            break;
        case Valeur::Neuf:
            if (is_atout) {return 14;} else {return 0;}
            break;
    }
    return -1;
}

void Jeu::createRandomPaquet(){
	Paquet provPaquet = {};
	for (auto c = couleurs.begin(); c != couleurs.end(); c++){
		for (auto v = valeurs.begin(); v != valeurs.end(); v++){
			provPaquet.push_back(std::tuple {*v,*c});
		}
	}
	std::shuffle (provPaquet.begin(), provPaquet.end(), gen);
	Jeu::paquet = provPaquet;
}

void Jeu::comptePoints(){
    Jeu::points_OE = 0;
    Jeu::points_NS = 0;
    int n = Jeu::defausseNS.size();
    int m = Jeu::defausseOE.size();
    Atout atout_actuel = std::get<2>(Jeu::current_enchere);

    for (int i=0;i<n;i++){
        Jeu::points_NS += carteToPoint(Jeu::defausseNS[i], atout_actuel);
    }
    for (int j=0;j<m;j++){
        Jeu::points_OE += carteToPoint(Jeu::defausseOE[j], atout_actuel);
    }
    if (atout_actuel != Atout::Sa && atout_actuel != Atout::Ta){
        Couleur couleur_actuelle = atoutToCouleur(atout_actuel);
        if ((std::find(Jeu::defausseNS.begin(), Jeu::defausseNS.end(), Carte(Valeur::Roi,couleur_actuelle)) != Jeu::defausseNS.end()) && 
            (std::find(Jeu::defausseNS.begin(), Jeu::defausseNS.end(), Carte(Valeur::Dame,couleur_actuelle)) != Jeu::defausseNS.end())){
                Jeu::points_NS += 20;
            }
        if ((std::find(Jeu::defausseOE.begin(), Jeu::defausseOE.end(), Carte(Valeur::Roi,couleur_actuelle)) != Jeu::defausseOE.end()) && 
            (std::find(Jeu::defausseOE.begin(), Jeu::defausseOE.end(), Carte(Valeur::Dame,couleur_actuelle)) != Jeu::defausseOE.end())){
                Jeu::points_OE += 20;
            }
    }
}

void Jeu::coupePaquet(int where_to_cut){ 
    /*Convention : couper APRES la carte d'index where_to_cut ; ex : wtc = 0 -> on place la carte du haut en bas..*/
    Paquet newPaquet;
    int n = Jeu::paquet.size();
    for (int i = 1+where_to_cut; i < n; i++){
        newPaquet.push_back(Jeu::paquet[i]);
    }
    for (int i = 0; i < 1+where_to_cut; i++) {
        newPaquet.push_back(Jeu::paquet[i]);
    }
    Jeu::paquet = newPaquet;
}

void Jeu::distributionPaquet(Joueur who_cuts, int where_to_cut){
    int who_cuts_int = joueurToInt(who_cuts);
    coupePaquet(where_to_cut);
    for (int i=0;i<12;i++){
        allPaquets[(i/3+who_cuts_int+2)%4].push_back(Jeu::paquet[i]);
    }
    for (int i=0;i<8;i++){
        allPaquets[(i/2+who_cuts_int+2)%4].push_back(Jeu::paquet[i+12]);
    }
    for (int i=0;i<12;i++){
        allPaquets[(i/3+who_cuts_int+2)%4].push_back(Jeu::paquet[i+20]);
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