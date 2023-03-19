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
                    std::cout << pseudo << " vient de se connecter !" << std::endl << std::flush;
                }
                if (++nb_joueurs_connectes == nb_joueurs_max){
                    all_connected = true;
                    listener.close();
                    selector.remove(listener);
                }
            }
        }


    }
}

void RoleDistribution(sockvec NetJoueurs, std::vector<Joueur> Roles, std::vector<std::string> Pseudos, int nb_joueurs_max, sf::SocketSelector &selector){
    sf::Packet intRolePacket;
    sf::Packet pseudoPacket;
    for (int i=0; i < nb_joueurs_max; i++){
        intRolePacket.clear();
        pseudoPacket.clear();
        intRolePacket << i;
        pseudoPacket << Pseudos[i];
        for (int j=0; j < nb_joueurs_max; j++){
            sf::TcpSocket *client_socket = NetJoueurs[j];
            client_socket->send(pseudoPacket);
            client_socket->send(intRolePacket);
        }
    }
}


int servermain(){

    Jeu game;
    std::vector<Joueur> Roles = {Joueur::Nord,Joueur::Est,Joueur::Sud,Joueur::Ouest};

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
        std::cout << "Serveur opérationnel !" << std::endl << "IP adress : " << sf::IpAddress::getLocalAddress() << std::endl;
    }
    else{
        std::cout << "Serveur down..." << std::endl;
        return 1;
    }

    ConnexionPhase(nb_joueurs_max, NetJoueurs, Pseudos, listener, selector);

    std::cout << "Everyone is connected ; role distribution : " << std::endl;
    for (int i=0;i<nb_joueurs_max;i++){
        std::cout << "- " << Pseudos[i] <<" : " << Roles[i] << std::endl;
    }

    RoleDistribution(NetJoueurs, Roles, Pseudos, nb_joueurs_max, selector);

    return 0;

}