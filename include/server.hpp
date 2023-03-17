#ifndef SERVER_HPP

#define SERVER_HPP

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "carte.hpp"
#include "game.hpp"

typedef std::vector<sf::TcpSocket*> sockvec;

int servermain();

#endif