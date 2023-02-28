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

std::ostream &operator<<(std::ostream &os, Joueur j){
	switch (j) {
		case Joueur::Nord:
			os << "Nord";
			break;
		case Joueur::Est:
			os << "Est";
			break;
		case Joueur::Sud:
			os << "Sud";
			break;
		case Joueur::Ouest:
			os << "Ouest";
			break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, Atout a){
	switch (a) {
		case Atout::Pique:
			os << "Pique";
			break;
		case Atout::Carreau:
			os << "Carreau";
			break;
		case Atout::Coeur:
			os << "Coeur";
			break;
		case Atout::Trefle:
			os << "Trefle";
			break;
		case Atout::Sa:
			os << "Sans atout";
			break;
		case Atout::Ta:
			os << "Tout atout";
			break;
		case Atout::Passe:
			os << "Passe";
			break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, Enchere e){
	auto [j,p,a,c,sc] = e;
    os << j << " : " << p << " " << a;
    if (sc) {os << "sur";} else {if (c) {os << "coinché";}}
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

std::string Jeu::joueurToString(Joueur j){
    switch(j){
        case Joueur::Nord:
            return "Nord";
            break;
        case Joueur::Est:
            return "Est";
            break;
        case Joueur::Sud:
            return "Sud";
            break;
        case Joueur::Ouest:
            return "Ouest";
            break;
    }
    return "Error";
}

Atout Jeu::stringToAtout(std::string s){
    if (s=="Pique"){return Atout::Pique;}
    if (s=="Carreau"){return Atout::Carreau;}
    if (s=="Coeur"){return Atout::Coeur;}
    if (s=="Trefle"){return Atout::Trefle;}
    if (s=="Ta"){return Atout::Ta;}
    if (s=="Sa"){return Atout::Sa;}
    if (s=="Passe"){return Atout::Passe;}
return Atout::Pique;
}

Couleur Jeu::stringToCouleur(std::string s){
    if (s=="Pique"){return Couleur::Pique;}
    if (s=="Carreau"){return Couleur::Carreau;}
    if (s=="Coeur"){return Couleur::Coeur;}
    if (s=="Trefle"){return Couleur::Trefle;}
return Couleur::Pique;
}

Valeur Jeu::stringToValeur(std::string s){
    if (s=="As"){return Valeur::As;}
    if (s=="Dix"){return Valeur::Dix;}
    if (s=="Roi"){return Valeur::Roi;}
    if (s=="Dame"){return Valeur::Dame;}
    if (s=="Valet"){return Valeur::Valet;}
    if (s=="Neuf"){return Valeur::Neuf;}
    if (s=="Huit"){return Valeur::Huit;}
    if (s=="Sept"){return Valeur::Sept;}
return Valeur::As;
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
    if ((a==Atout::Ta)||(atoutToInt(a)==couleurToInt(coul))){
        is_atout = true;
    }
    switch (val){
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
        case Valeur::As:
            if (a==Atout::Sa){return 19;} else {return 11;}
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

bool Jeu::auxCompareValeur(Valeur v1, Valeur v2, std::vector<Valeur> tab, int c){
    if (tab[c]==v1) {return true;} else {if (tab[c]==v2) {return false;} else {return auxCompareValeur(v1,v2,tab,c+1);}}
}

bool Jeu::compareCarte(Carte c1, Carte c2, Atout atout_actuel, Couleur couleur_demandee){
    if (std::get<1>(c1) == std::get<1>(c2)) {
        if (atout_actuel==Atout::Ta || couleurToAtout(std::get<1>(c1))==atout_actuel) {
            return auxCompareValeur(std::get<0>(c1),std::get<0>(c2),Jeu::comp_atout,0);
        }
            return auxCompareValeur(std::get<0>(c1),std::get<0>(c2),Jeu::comp_non_atout,0);
    }
    if(couleurToAtout(std::get<1>(c1)) == atout_actuel && couleurToAtout(std::get<1>(c2)) != atout_actuel) {return true;}
    if(couleurToAtout(std::get<1>(c2)) == atout_actuel && couleurToAtout(std::get<1>(c1)) != atout_actuel) {return false;}
    if(std::get<1>(c1) == couleur_demandee && std::get<1>(c2) != couleur_demandee) {return true;}
    if(std::get<1>(c2) == couleur_demandee && std::get<1>(c1) != couleur_demandee) {return false;}
    return auxCompareValeur(std::get<0>(c1),std::get<0>(c2),Jeu::comp_non_atout,0);

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
    if (n==0) {Jeu::points_OE += 250;} else {if (m==0) {Jeu::points_NS += 250;} else {

    for (int i=0;i<n;i++){
        Jeu::points_NS += carteToPoint(Jeu::defausseNS[i], atout_actuel);
    }
    for (int j=0;j<m;j++){
        Jeu::points_OE += carteToPoint(Jeu::defausseOE[j], atout_actuel);
    }
    }}
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

void Jeu::fusionPaquets(){
    Jeu::paquet.clear();
    for (int i=0;i<32;i++){
        Jeu::paquet.push_back(allPaquets[i/8][i%8]);
    }
}

Enchere Jeu::ask_enchere(Joueur who_bids){
    std::string satout;
    Atout atout;
    int points=0;
    auto [j,p,a,c,sc] = Jeu::current_enchere;
    std::cout << std::endl << "Au tour de " << joueurToString(who_bids) << " de parler : " << std::endl;
    std::cout << "Enchere en cours : " << "de " << j << ", " << p << " " << a << std::endl;
    std::cout << "Atout ? : ";
    std::cin >> satout;
    atout = stringToAtout(satout);
    if (atout==Atout::Passe) {Enchere my_enchere = {who_bids,0,Atout::Passe,false,false}; return my_enchere;}
    else {
        std::cout << std::endl << "Points ? : ";
        std::cin >> points;
        if (points%10 || points < 10 + p || points < 80 || points > 250) {
            std::cout << std::endl << "Points invalides" << std::flush;
            return ask_enchere(who_bids);
        }
        else {
            Enchere my_enchere = {who_bids,points,atout,false,false};
            return my_enchere;
        }
    }
}

bool Jeu::next_enchere(Joueur who_bids, bool first_enchere){   
    if (who_bids == std::get<0>(Jeu::current_enchere) && not first_enchere){
        return true;
    }
    else{
        Enchere what_bids = ask_enchere(who_bids);
        Jeu::last_enchere[joueurToInt(who_bids)] = what_bids;
        if (std::get<1>(what_bids) > std::get<1>(Jeu::current_enchere)){
            Jeu::current_enchere = what_bids;
        }
        return false;
    }
}

Carte Jeu::pose_carte(bool premier_a_jouer){
    std::string sval_posee; std::string scoul_posee; Valeur val_posee; Couleur coul_posee; std::string de;
    bool legal_move=true;
    do {
        std::cout << std::endl << Jeu::who_plays << " : ";
        std::cin >> sval_posee >> de >> scoul_posee;
        val_posee = stringToValeur(sval_posee);
        coul_posee = stringToCouleur(scoul_posee);
    } while (true);

}

void Jeu::joue_pli(){
    Jeu::pli_actuel.clear();
    for (int player = 0; player < 4; player++){
        Jeu::pli_actuel.push_back(pose_carte(not player));
        if (not player) {Jeu::couleur_demandee = std::get<1>(Jeu::pli_actuel[0]);}
        next_to_play();
    }
    Jeu::dernier_pli = Jeu::pli_actuel;
}

void Jeu::next_to_play(){
    Joueur provj = Jeu::who_plays;
    Jeu::who_plays = intToJoueur((1+joueurToInt(provj))%4);
}

void Jeu::next_to_cut(){
    Joueur provj = Jeu::who_cuts;
    Jeu::who_cuts = intToJoueur((1+joueurToInt(provj))%4);
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

Paquet Jeu::getPaquet(Joueur j){
	return allPaquets[joueurToInt(j)];
}