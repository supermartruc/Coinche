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

int AskCuts(Jeu game, sockvec NetJoueurs){
    return 0;
}

int CreeEnvoiePaquet(Jeu game, sockvec NetJoueurs){
    game.createRandomPaquet();
    game.distributionPaquet(Joueur::Sud, 1);
    for (int i=0; i < game.allPaquets.size(); i++){
        Paquet paquetJoueur = game.allPaquets[i];
        sf::TcpSocket *client_socket = NetJoueurs[i];
        std::string paquetString = "";
        sf::Packet paquetStringPacket;
        for (Carte carte : paquetJoueur){
            paquetString += std::to_string(carteToInt(carte)) + " ";
        }
        paquetStringPacket << paquetString;
        if (client_socket->send(paquetStringPacket) == sf::Socket::Done){
            std::cout << "Paquet bien envoyé à " << intToJoueur(i) << " : " << paquetString << std::endl;
        }
        else{
            std::cout << "Paquet NON envoyé à " << intToJoueur(i) << " : " << paquetString << std::endl;
            return 1;
        }
    }
    return 0;
}

void SendGameInfo(Jeu game, sockvec NetJoueurs){
    int InfoInt = (int)(game.enchere_en_cours) + 10*joueurToInt(game.who_speaks) + 100*joueurToInt(game.who_plays) + 1000*((joueurToInt(game.who_cuts)+1)%4);
    sf::Packet InfoIntPacket;
    InfoIntPacket << InfoInt;
    for (int i=0; i<NetJoueurs.size();i++){
        sf::TcpSocket *client_socket = NetJoueurs[i];
        if (client_socket->send(InfoIntPacket) == sf::Socket::Done){
        }
        else{
            std::cout << "Envoi échoué" << std::endl;
        }
    }
}


void PhaseEncheres(Jeu game, sockvec NetJoueurs){
    
}



int servermain(){

    Jeu game;
    std::vector<Joueur> Roles = {Joueur::Nord,Joueur::Est,Joueur::Sud,Joueur::Ouest};

    int nb_joueurs_max = 4;

    int port = 1234;
    std::cout << "Entrez le port : " <<std::flush;
    std::cin >> port;
    sockvec NetJoueurs; // Nord, Est, Sud, Ouest dans l'ordre
    std::vector<std::string> Pseudos;

    NetJoueurs.clear();
    Pseudos.clear();

    sf::TcpListener listener;
    listener.listen(port);
    sf::SocketSelector selector;
    selector.add(listener);

    if (listener.listen(port) == sf::Socket::Done){
        std::cout << "Serveur opérationnel !" << std::endl << "IP adress : " << sf::IpAddress::getLocalAddress() << std::endl 
        << "Port : " << port << std::endl;
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

    CreeEnvoiePaquet(game, NetJoueurs);

    game.enchere_en_cours = true;
    SendGameInfo(game, NetJoueurs);

    PhaseEncheres(game, NetJoueurs);

    selector.clear();
    NetJoueurs.clear();

    return 0;

}