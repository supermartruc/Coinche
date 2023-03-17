#include "server.hpp"

void ConnexionPhase(int nb_joueurs_max, sockvec &NetJoueurs, std::vector<std::string> &Pseudos, sf::TcpListener &listener, sf::SocketSelector selector){
    bool all_connected = false;
    int nb_joueurs_connectes = 0;
    while (!all_connected){
        if (selector.wait(sf::milliseconds(100))){
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
}

int servermain(){

    int nb_joueurs_max = 4;

    int port = 1234;
    
    sockvec NetJoueurs;
    std::vector<std::string> Pseudos;

    NetJoueurs.clear();
    Pseudos.clear();

    sf::TcpListener listener;
    listener.listen(port);
    sf::SocketSelector selector;
    selector.add(listener);

    if (listener.listen(port) == sf::Socket::Done){
        std::cout << "Serveur opérationnel !" << std::endl << "IP adress : " << std::flush;
        system("curl ifconfig.me");
        std::cout << std::endl;
    }
    else{
        std::cout << "Serveur down..." << std::endl;
    }

    ConnexionPhase(nb_joueurs_max, NetJoueurs, Pseudos, listener, selector);
    std::cout << "Everyone is connected ; here are the pseudos : " << std::endl;
    for (int i=0;i<nb_joueurs_max;i++){
        std::cout << "- " << Pseudos[i] << std::endl;
    }

    return 0;

}