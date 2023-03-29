#ifndef CLIENT_HPP

#define CLIENT_HPP

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "carte.hpp"
#include "game.hpp"
#include "view.hpp"

void InitView(GameView &view, Timer &timer);
int ConnexionServer(sf::TcpSocket &client_socket, std::string ipaddress, int port, std::string &my_pseudo);
void getPseudoRole(std::vector<std::pair<std::string,Joueur>> &assoc_pseudo_role, sf::TcpSocket &client_socket, std::string my_pseudo, Joueur &my_role, int nb_joueurs, GameView &view);
void GetGameInfo(Joueur &who_deals, Joueur &who_speaks, Joueur &who_plays, bool &enchere_en_cours, sf::TcpSocket &client_socket, std::vector<Enchere> &all_encheres, Enchere &current_enchere, GameView &view, Joueur my_role, Paquet mesCartes, Timer &timer);
void SendEnchere(GameInfo &gameInfo);
void GetEnchere(Joueur my_role, int &pointclient, Atout &atoutclient, bool &coincheclient, bool &surcoincheclient, Enchere current_enchere, GameView &view, Timer &timer, Paquet mesCartes, Joueur who_deals);

int clientmain();

#endif