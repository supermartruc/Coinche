#include "game.hpp"
#include <SFML/Network.hpp>

int servmain(){

    bool connection_phase_done = false;
    bool all_connected = false;

    int nb_joueurs_max = 4;
    int nb_joueurs_connectes = 0;

    int port = 1234;
    
    std::vector<sf::TcpSocket*> NetJoueurs;
    std::vector<std::string> Pseudos;

    NetJoueurs.clear();
    Pseudos.clear();

    sf::TcpListener listener;
    listener.listen(port);
    sf::SocketSelector selector;
    selector.add(listener);

    if (listener.listen(port) == sf::Socket::Done){
        std::cout << "Serveur opérationnel" << std::endl << "IP adress : " << std::flush;
        system("curl ifconfig.me");
        std::cout << std::endl;
    }
    else{
        std::cout << "Serveur down..." << std::endl;
    }

    while (!connection_phase_done){
        if (selector.wait()){
            if (not all_connected && selector.isReady(listener)){
                sf::TcpSocket* socket_joueur = new sf::TcpSocket();
                listener.accept(*socket_joueur);
                selector.add(*socket_joueur);
                NetJoueurs.push_back(socket_joueur);

                std::string pseudo;
                sf::Packet pseudoPacket;
                if (socket_joueur->receive(pseudoPacket) == sf::Socket::Done){
                    pseudoPacket >> pseudo;
                    Pseudos.push_back(pseudo);
                }
                if (++nb_joueurs_connectes == nb_joueurs_max){
                    all_connected = true;
                }
            }
        }


    }
    return 0;

}