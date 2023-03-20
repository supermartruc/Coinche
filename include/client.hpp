#ifndef CLIENT_HPP

#define CLIENT_HPP

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "carte.hpp"
#include "game.hpp"

int ConnexionServer(sf::TcpSocket &client_socket, std::string ipaddress, int port, std::string &my_pseudo);
void getPseudoRole(std::vector<std::pair<std::string,Joueur>> &assoc_pseudo_role, sf::TcpSocket &client_socket, std::string my_pseudo, Joueur &my_role, int nb_joueurs);
void GetGameInfo(Joueur &who_deals, Joueur &who_speaks, Joueur &who_plays, bool &enchere_en_cours, sf::TcpSocket &client_socket, std::vector<Enchere> &all_encheres, Enchere &current_enchere);
void SendEnchere(int pointclient, Atout atoutclient, bool coincheclient, bool surcoincheclient, sf::TcpSocket &client_socket);
void GetEnchere(int &pointclient, Atout &atoutclient, bool &coincheclient, bool &surcoincheclient, Enchere current_enchere);

int clientmain();

#endif