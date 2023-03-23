#ifndef SERVER_HPP

#define SERVER_HPP

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "carte.hpp"
#include "game.hpp"

typedef std::vector<sf::TcpSocket*> sockvec;

void ConnexionPhase(int nb_joueurs_max, sockvec &NetJoueurs, std::vector<std::string> &Pseudos, sf::TcpListener &listener, sf::SocketSelector selector);
void RoleDistribution(sockvec NetJoueurs, std::vector<Joueur> Roles, std::vector<std::string> Pseudos, int nb_joueurs_max, sf::SocketSelector &selector);
int AskCuts(Jeu &game, sockvec NetJoueurs);
int CreeEnvoiePaquet(Jeu &game, sockvec NetJoueurs);
void SendGameInfo(Jeu &game, sockvec NetJoueurs);


int servermain();

#endif