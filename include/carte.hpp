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
};

typedef std::tuple<Valeur,Couleur> Carte;
typedef std::vector<Carte> Paquet;
typedef std::tuple<Joueur,int,Atout,bool,bool> Enchere; // Joueur, points, atout (ou passe), coinché, surcoinché.

std::ostream &operator<<(std::ostream &os, Valeur v);
std::ostream &operator<<(std::ostream &os, Couleur c);
std::ostream &operator<<(std::ostream &os, Carte c);

static std::vector<Couleur> couleurs {Couleur::Coeur, Couleur::Carreau, Couleur::Pique, Couleur::Trefle};
static std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};

#endif