#ifndef CLIENT_HPP

#define CLIENT_HPP

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "carte.hpp"
#include "game.hpp"

typedef std::vector<sf::TcpSocket*> sockvec;

int clientmain();

#endif