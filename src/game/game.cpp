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

std::ostream &operator<<(std::ostream &os, Paquet p) {
    os << "{";
	for (int i=0; i < p.size(); i++){
        os << p[i] << " ";
    }
    os << "}";
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

int joueurToInt(Joueur j){
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

Joueur intToJoueur(int i){
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

int atoutToInt(Atout a){
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

int couleurToInt(Couleur c){
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

std::string joueurToString(Joueur j){
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

Atout stringToAtout(std::string s){
    if (s=="Pique"){return Atout::Pique;}
    if (s=="Carreau"){return Atout::Carreau;}
    if (s=="Coeur"){return Atout::Coeur;}
    if (s=="Trefle"){return Atout::Trefle;}
    if (s=="Ta"){return Atout::Ta;}
    if (s=="Sa"){return Atout::Sa;}
    if (s=="Passe"){return Atout::Passe;}
return Atout::Pique;
}

Couleur stringToCouleur(std::string s){
    if (s=="Pique"){return Couleur::Pique;}
    if (s=="Carreau"){return Couleur::Carreau;}
    if (s=="Coeur"){return Couleur::Coeur;}
    if (s=="Trefle"){return Couleur::Trefle;}
return Couleur::Pique;
}

Valeur stringToValeur(std::string s){
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

Atout couleurToAtout(Couleur c){
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

Couleur atoutToCouleur(Atout a){
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

int carteToPoint(Carte c, Atout a){
    auto [val, coul] = c;
    bool is_atout=(a==Atout::Ta)||(atoutToInt(a)==couleurToInt(coul));
    if ((a==Atout::Ta)||(atoutToInt(a)==couleurToInt(coul))){is_atout = true;}
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

bool paquetContientCouleur(Paquet my_paquet, Couleur my_couleur){
    int n = my_paquet.size();
    for (int i=0; i<n; i++){
        if (std::get<1>(my_paquet[i])==my_couleur) {return true;}
    }
    return false;
}

bool auxCompareValeur(Valeur v1, Valeur v2, std::vector<Valeur> tab, int c){
    if (tab[c]==v1) {return true;} else {if (tab[c]==v2) {return false;} else {return auxCompareValeur(v1,v2,tab,c+1);}}
}

bool compareCarte(Carte c1, Carte c2, Couleur couleur_demandee,  Atout atout_actuel){
    if (std::get<1>(c1) == std::get<1>(c2)) {
        if (atout_actuel==Atout::Ta || couleurToAtout(std::get<1>(c1))==atout_actuel) {
            return auxCompareValeur(std::get<0>(c1),std::get<0>(c2),comp_atout,0);
        }
            return auxCompareValeur(std::get<0>(c1),std::get<0>(c2),comp_non_atout,0);
    }
    if(couleurToAtout(std::get<1>(c1)) == atout_actuel && couleurToAtout(std::get<1>(c2)) != atout_actuel) {return true;}
    if(couleurToAtout(std::get<1>(c2)) == atout_actuel && couleurToAtout(std::get<1>(c1)) != atout_actuel) {return false;}
    if(std::get<1>(c1) == couleur_demandee && std::get<1>(c2) != couleur_demandee) {return true;}
    if(std::get<1>(c2) == couleur_demandee && std::get<1>(c1) != couleur_demandee) {return false;}
    return auxCompareValeur(std::get<0>(c1),std::get<0>(c2),comp_non_atout,0);

}

Carte max_of_paquet(Paquet thepaquet, Couleur couleur_demandee, Atout atout_actuel){
    if (thepaquet.empty()){return Carte{Valeur::As,Couleur::Pique};}
    Carte provc = thepaquet[0];
    for (int i=1;i<thepaquet.size();i++){
        if (compareCarte(thepaquet[i],provc,couleur_demandee, atout_actuel)){
            provc = thepaquet[i];
        }
    }
    return provc;
}

bool est_valide_carte(Carte jcarte, Paquet jpaquet, Couleur couleur_demandee, Atout atout_actuel, Paquet pli_en_cours, Joueur self, Joueur first){

    auto [jval,jcoul] = jcarte;

    if (std::find(jpaquet.begin(),jpaquet.end(),jcarte) == jpaquet.end()){return false;}
    if (first == self){return true;}
    bool monte_a_atout = (compareCarte(jcarte,max_of_paquet(pli_en_cours,couleur_demandee,atout_actuel),couleur_demandee,atout_actuel) || 
        compareCarte(max_of_paquet(pli_en_cours,couleur_demandee,atout_actuel),max_of_paquet(jpaquet,couleur_demandee,atout_actuel),couleur_demandee,atout_actuel));
    if (jcoul == couleur_demandee){
        if (couleurToAtout(couleur_demandee) == atout_actuel || atout_actuel == Atout::Ta){
            return monte_a_atout;
        }
        return true;
    }
    if (paquetContientCouleur(jpaquet,couleur_demandee)){return false;}
	if (atout_actuel == Atout::Ta || atout_actuel == Atout::Sa){return true;}
    if (couleurToAtout(jcoul) == atout_actuel){return monte_a_atout;}
    if (paquetContientCouleur(jpaquet,atoutToCouleur(atout_actuel))){
        if (self == intToJoueur((joueurToInt(first)+1)%4)){return false;}
        if (self == intToJoueur((joueurToInt(first)+2)%4)){return (compareCarte(pli_en_cours[0],pli_en_cours[1],couleur_demandee,atout_actuel));}
        return (compareCarte(pli_en_cours[1],max_of_paquet(pli_en_cours,couleur_demandee,atout_actuel),couleur_demandee,atout_actuel));
    }
    return true;
}

Paquet tri_une_couleur(Paquet jpaquet, Couleur act_coul, Atout atout_actuel){
    int n = jpaquet.size();
    Paquet provpaquet = {};
    std::vector<Valeur> used;
    if (atout_actuel == Atout::Ta || atout_actuel == couleurToAtout(act_coul)){used = comp_atout;} else {used = comp_non_atout;}
    for (int i = 0; i < 8; i++){
        Carte lacarte = Carte {used[i], act_coul};
        if (std::find(jpaquet.begin(),jpaquet.end(),lacarte) != jpaquet.end()){
            provpaquet.push_back(lacarte);
        }
    }
    return provpaquet;
}

Paquet tri_paquet_affichage(Paquet jpaquet, Atout atout_actuel){
    std::vector<Couleur> les_couleurs_aux = {Couleur::Pique,Couleur::Carreau,Couleur::Trefle,Couleur::Coeur};
    std::vector<Couleur> les_couleurs = {};
    int n = jpaquet.size();
    if (atout_actuel == Atout::Sa || atout_actuel == Atout::Ta){
        les_couleurs = les_couleurs_aux;
    }
    else{
        les_couleurs.push_back(atoutToCouleur(atout_actuel));
        for (int i=0;i<4;i++){
            Couleur act_coul = les_couleurs_aux[i];
            if (act_coul != atoutToCouleur(atout_actuel)){
                les_couleurs.push_back(act_coul);
            } 
        }
    }
    Paquet les_piques = {}, les_carreaux = {}, les_coeurs = {}, les_trefles = {};
    for (int i = 0; i < n; i++){
        if (std::get<1>(jpaquet[i]) == Couleur::Pique) {les_piques.push_back(jpaquet[i]);}
        if (std::get<1>(jpaquet[i]) == Couleur::Carreau) {les_carreaux.push_back(jpaquet[i]);}
        if (std::get<1>(jpaquet[i]) == Couleur::Coeur) {les_coeurs.push_back(jpaquet[i]);}
        if (std::get<1>(jpaquet[i]) == Couleur::Trefle) {les_trefles.push_back(jpaquet[i]);}
    }
    Paquet les_new_piques = tri_une_couleur(les_piques,Couleur::Pique,atout_actuel);
    Paquet les_new_carreaux = tri_une_couleur(les_carreaux,Couleur::Carreau,atout_actuel);
    Paquet les_new_coeurs = tri_une_couleur(les_coeurs,Couleur::Coeur,atout_actuel);
    Paquet les_new_trefles = tri_une_couleur(les_trefles,Couleur::Trefle,atout_actuel);

    std::vector<Paquet> prov_fusion = {};
    for (int i = 0; i < 4; i++){
        if (les_couleurs[i]==Couleur::Pique){prov_fusion.push_back(les_new_piques);}
        if (les_couleurs[i]==Couleur::Carreau){prov_fusion.push_back(les_new_carreaux);}
        if (les_couleurs[i]==Couleur::Coeur){prov_fusion.push_back(les_new_coeurs);}
        if (les_couleurs[i]==Couleur::Trefle){prov_fusion.push_back(les_new_trefles);}
    }
    Paquet final_res = {};
    for (int i = 0; i < 4; i++){
        int taille = prov_fusion[i].size();
        for (int j = 0; j < taille; j++){
            final_res.push_back(prov_fusion[i][j]);
        }
    }
    return final_res;
}

void Jeu::createRandomPaquet(){
	Paquet provPaquet = {};
	for (auto c = couleurs.begin(); c != couleurs.end(); c++){
		for (auto v = valeurs.begin(); v != valeurs.end(); v++){
			provPaquet.push_back(std::tuple {*v,*c});
		}
	}
	std::shuffle (provPaquet.begin(), provPaquet.end(), gen);
	paquet = provPaquet;
}

int find_index_render(Carte c){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            if (c == Carte{valeurs[j], couleurs[i]}){
                return (4*i+j);
            }
        }
    }
    return -1;
}

void Jeu::comptePoints(){
    points_OE = 0;
    points_NS = 0;
    int n = defausseNS.size();
    int m = defausseOE.size();
    Atout atout_actuel = std::get<2>(current_enchere);
    if (n==0) {points_OE += 250;} else {if (m==0) {points_NS += 250;} else {

    for (int i=0;i<n;i++){
        points_NS += carteToPoint(defausseNS[i], atout_actuel);
    }
    for (int j=0;j<m;j++){
        points_OE += carteToPoint(defausseOE[j], atout_actuel);
    }
    }}
    if (atout_actuel != Atout::Sa && atout_actuel != Atout::Ta){
        Couleur couleur_actuelle = atoutToCouleur(atout_actuel);
        if ((std::find(defausseNS.begin(), defausseNS.end(), Carte(Valeur::Roi,couleur_actuelle)) != defausseNS.end()) && 
            (std::find(defausseNS.begin(), defausseNS.end(), Carte(Valeur::Dame,couleur_actuelle)) != defausseNS.end())){
                points_NS += 20;
            }
        if ((std::find(defausseOE.begin(), defausseOE.end(), Carte(Valeur::Roi,couleur_actuelle)) != defausseOE.end()) && 
            (std::find(defausseOE.begin(), defausseOE.end(), Carte(Valeur::Dame,couleur_actuelle)) != defausseOE.end())){
                points_OE += 20;
            }
    }
    if (dix_de_der_winner == Joueur::Nord || dix_de_der_winner == Joueur::Sud){points_NS += 10;}
    else {points_OE += 10;}
}

void Jeu::coupePaquet(int where_to_cut){ 
    /*Convention : couper APRES la carte d'index where_to_cut ; ex : wtc = 0 -> on place la carte du haut en bas..*/
    Paquet newPaquet;
    int n = Jeu::paquet.size();
    for (int i = 1+where_to_cut; i < n; i++){
        newPaquet.push_back(paquet[i]);
    }
    for (int i = 0; i < 1+where_to_cut; i++) {
        newPaquet.push_back(paquet[i]);
    }
    paquet = newPaquet;
}

void Jeu::distributionPaquet(Joueur who_cuts, int where_to_cut){
    int who_cuts_int = joueurToInt(who_cuts);
    coupePaquet(where_to_cut);
    for (int i=0;i<12;i++){
        allPaquets[(i/3+who_cuts_int+2)%4].push_back(paquet[i]);
    }
    for (int i=0;i<8;i++){
        allPaquets[(i/2+who_cuts_int+2)%4].push_back(paquet[i+12]);
    }
    for (int i=0;i<12;i++){
        allPaquets[(i/3+who_cuts_int+2)%4].push_back(paquet[i+20]);
    }
}

void Jeu::fusionPaquets(){
    paquet.clear();
    for (int i=0;i<32;i++){
        paquet.push_back(allPaquets[i/8][i%8]);
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
    if (who_bids == std::get<0>(current_enchere) && not first_enchere){
        return true;
    }
    else{
        Enchere what_bids = ask_enchere(who_bids);
        last_enchere[joueurToInt(who_bids)] = what_bids;
        if (std::get<1>(what_bids) > std::get<1>(current_enchere)){
            current_enchere = what_bids;
        }
        return false;
    }
}

Carte demande_carte(Joueur joueur){
    std::cout << "Jouez une carte, " << joueurToString(joueur) << " : " << std::endl << std::flush;
    std::string vs, cs, de; 
    std::cin >> vs >> de >> cs;
    return Carte{stringToValeur(vs), stringToCouleur(cs)};
}

void Jeu::joue_pli(){
    dernier_pli = pli_actuel;
    pli_actuel.clear();
    std::cout << std::endl << "A " << joueurToString(who_plays) << " de commencer : " << std::endl;
    Carte carte_jouee; Paquet jpaquet;
    for (int i = 0; i < 4; i++){
        afficheAllPaquetsListe();
        if (i != 0) {std::cout << std::endl << "A " << joueurToString(who_plays) << " de jouer : " << std::endl;}
        jpaquet = getPaquet(who_plays);
        carte_jouee = demande_carte(who_plays);
        while (true){
            std::cout << carte_jouee << jpaquet << pli_actuel << std::endl;
            std::cout << est_valide_carte(carte_jouee,jpaquet,couleur_demandee,atout_actuel,pli_actuel,who_plays,intToJoueur((joueurToInt(who_plays)-i+4)%4)) << std::endl;
            if (est_valide_carte(carte_jouee,jpaquet,couleur_demandee,atout_actuel,pli_actuel,who_plays,intToJoueur((joueurToInt(who_plays)-i+4)%4))){
                std::cout << "CEST BON" << std::endl;
                break;
            }
            else {std::cout << "CEST PAS BON" << std::endl; carte_jouee = demande_carte(who_plays);}
        }
        if (i==0) {couleur_demandee = std::get<1>(carte_jouee);}
        pli_actuel.push_back(carte_jouee);
        (allPaquets[joueurToInt(who_plays)]).erase(std::find(allPaquets[joueurToInt(who_plays)].begin(),allPaquets[joueurToInt(who_plays)].end(),carte_jouee));
        next_to_play();
    }
    Joueur winner_du_pli = Joueur::Nord;
    Carte carte_winneuse = max_of_paquet(pli_actuel,couleur_demandee,atout_actuel);
    for (int i = 0; i < 4; i++){
        if (pli_actuel[i] == carte_winneuse){ // astuce : avec les next_to_play(), who_plays vaut le joueur qui a iniitié le pli
            winner_du_pli = intToJoueur(i);
            if (winner_du_pli == Joueur::Nord || winner_du_pli == Joueur::Sud){
                for (int j=0; j < 4; j++){
                    defausseNS.push_back(pli_actuel[j]);
                }
                if (not allPaquets[0].size()){
                    dix_de_der_winner = Joueur::Nord;
                }
                break;
            }
            else{
                for (int j=0; j < 4; j++){
                    defausseOE.push_back(pli_actuel[j]);
                }
                if (not allPaquets[0].size()){
                    dix_de_der_winner = Joueur::Ouest;
                }
                break;
            }
        }
    }
    who_plays = winner_du_pli;
}

void Jeu::next_to_play(){
    Joueur provj = who_plays;
    who_plays = intToJoueur((1+joueurToInt(provj))%4);
}

void Jeu::next_to_cut(){
    Joueur provj = who_cuts;
    who_cuts = intToJoueur((1+joueurToInt(provj))%4);
}

void Jeu::affichePaquetListe(Paquet paquet){
    int n = paquet.size();
    std::cout << "[";
    for (int i=0; i < n-1; i++){
        std::cout << paquet[i] << " ; ";
    }

    if (n) {std::cout << paquet[n-1];}
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