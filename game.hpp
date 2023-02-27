#ifndef GAME_HPP

#define GAME_HPP

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
    Coeur,
    Pique,
    Trefle,
    Carreau,
};

typedef std::tuple<Valeur,Couleur> Carte;
typedef std::vector<Carte> Paquet;

std::ostream &operator<<(std::ostream &os, Valeur v);
std::ostream &operator<<(std::ostream &os, Couleur c);
std::ostream &operator<<(std::ostream &os, Carte c);

class Jeu{
    public:
        Jeu() = default;
        ~Jeu() = default;
        std::vector<Couleur> couleurs {Couleur::Coeur, Couleur::Carreau, Couleur::Pique, Couleur::Trefle};
        std::vector<Valeur> valeurs {Valeur::As,Valeur::Roi,Valeur::Dame,Valeur::Valet,Valeur::Dix,Valeur::Neuf,Valeur::Huit,Valeur::Sept};
        Paquet createPaquet();

                
};

#endif