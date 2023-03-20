#ifndef CARTES_HPP

#define CARTES_HPP

#include <random>
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>


enum class Joueur{
	Nord,
	Est,
	Sud,
	Ouest,
};

enum class Valeur{
	As,
	Roi,
	Dame,
	Valet,
	Dix,
	Neuf,
	Huit,
	Sept,
};

enum class Couleur{
	Pique,
	Carreau,
	Coeur,
	Trefle,
};

enum class Atout{
	Pique,
	Carreau,
	Coeur,
	Trefle,
	Ta,
	Sa,
	Passe,
	Rien,
};

typedef std::tuple<Valeur,Couleur> Carte;
typedef std::vector<Carte> Paquet;
typedef std::tuple<Joueur,int,Atout,bool,bool> Enchere; // Joueur, points, atout (ou passe), coinché, surcoinché.

std::ostream &operator<<(std::ostream &os, Valeur v);
std::ostream &operator<<(std::ostream &os, Couleur c);
std::ostream &operator<<(std::ostream &os, Carte c);
std::ostream &operator<<(std::ostream &os, Paquet p);
std::ostream &operator<<(std::ostream &os, Joueur j);
std::ostream &operator<<(std::ostream &os, Atout a);
std::ostream &operator<<(std::ostream &os, Enchere e);
int joueurToInt(Joueur j);
int atoutToInt(Atout a);
int couleurToInt(Couleur c);
int valeurToInt(Valeur v);
int carteToInt(Carte carte);
std::string joueurToString(Joueur j);
Atout stringToAtout(std::string s);
Atout couleurToAtout(Couleur c);
Atout keyToAtout(char c);
Couleur intToCouleur(int couleurInt);
Couleur atoutToCouleur(Atout a);
Couleur stringToCouleur(std::string s);
Valeur intToValeur(int valeurInt);
Valeur stringToValeur(std::string s);
Carte intToCarte(int carteInt);
Joueur intToJoueur(int i);
int carteToPoint(Carte c, Atout a);
bool auxCompareValeur(Valeur v1, Valeur v2, std::vector<Valeur> tab, int c);
bool compareCarte(Carte c1, Carte c2, Couleur couleur_demande, Atout atout_actuel);
Carte max_of_paquet(Paquet thepaquet, Couleur couleur_demandee, Atout atout_actuel);
bool paquetContientCouleur(Paquet my_paquet, Couleur my_couleur);
bool est_valide_carte(Carte jcarte, Paquet jpaquet, Couleur couleur_demandee, Atout atout_actuel, Paquet pli_en_cours, Joueur self, Joueur first);
Paquet tri_une_couleur(Paquet jpaquet, Couleur act_coul, Atout atout_actuel);
Paquet tri_paquet_affichage(Paquet jpaquet, Atout atout_actuel);
int find_index_render(Carte c);

static std::vector<Atout> atouts {Atout::Pique, Atout::Carreau, Atout::Trefle, Atout::Coeur, Atout::Passe, Atout::Ta, Atout::Sa};
static std::vector<Couleur> couleurs {Couleur::Pique, Couleur::Carreau, Couleur::Trefle, Couleur::Coeur};
static std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
static std::vector<Valeur> comp_non_atout = {Valeur::As,Valeur::Dix,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
static std::vector<Valeur> comp_atout = {Valeur::Valet,Valeur::Neuf,Valeur::As,Valeur::Dix,Valeur::Roi,Valeur::Dame,Valeur::Huit,Valeur::Sept};

#endif