#include "client.hpp"

int ConnexionServer(sf::TcpSocket &client_socket, std::string ipaddress, int port, std::string &my_pseudo){
    if (client_socket.connect(ipaddress,port) == sf::Socket::Done){
        std::cout << "Connexion réussie ! " << std::endl;
        sf::Packet my_pseudoPacket;
        my_pseudoPacket << my_pseudo;
        client_socket.send(my_pseudoPacket);
        return 0;
    }
    else {
        std::cout << "Connexion échouée... " << std::endl;
        return 1;
    }
}

void getPseudoRole(std::vector<std::pair<std::string,Joueur>> &assoc_pseudo_role, sf::TcpSocket &client_socket, std::string my_pseudo, Joueur &my_role, int nb_joueurs){
    sf::Packet PseudoReceivePacket;
    sf::Packet RoleReceivePacket;
    std::string pseudoReceive;
    int roleIntReceive;
    Joueur roleReceive;

    for (int i = 0; i<nb_joueurs; i++){
        if (client_socket.receive(PseudoReceivePacket) == sf::Socket::Done){
            PseudoReceivePacket >> pseudoReceive;
            if (client_socket.receive(RoleReceivePacket) == sf::Socket::Done){
                RoleReceivePacket >> roleIntReceive;
                roleReceive = intToJoueur(roleIntReceive);
                if (pseudoReceive == my_pseudo){
                    my_role = roleReceive;
                }
            }
        }
        assoc_pseudo_role.push_back({pseudoReceive,roleReceive});
    }
}

void RecupereCartes(Paquet &mesCartes, sf::TcpSocket &client_socket){
    mesCartes.clear();
    std::string mesCartesString = "";
    sf::Packet mesCartesStringPacket;
    if (client_socket.receive(mesCartesStringPacket) == sf::Socket::Done){
        mesCartesStringPacket >> mesCartesString;
    }
    std::string UneCarteString = "";
    for (int i=0; i < mesCartesString.size(); i++){
        if (mesCartesString[i] == ' '){
            mesCartes.push_back(intToCarte(std::stoi(UneCarteString)));
            UneCarteString = "";
        }
        else{
            UneCarteString += mesCartesString[i];
        }
    }
}

int clientmain(){

    int nb_joueurs = 4;
    Joueur my_role;

    std::vector<std::pair<std::string,Joueur>> assoc_pseudo_role;

    int port = 1234;
    std::string ipaddress;

    std::cout << "Adresse IP : " << std::flush;
    std::cin >> ipaddress;
    std::cout << "Port : " << std::flush;
    std::cin >> port;


    std::string my_pseudo;
    std::cout << "Votre pseudo : ";
    std::cin >> my_pseudo;
    std::cin.ignore();

    sf::TcpSocket client_socket;
    
    Paquet mesCartes = {};

    if (ConnexionServer(client_socket,ipaddress,port,my_pseudo)){
        return 1;
    }
    getPseudoRole(assoc_pseudo_role, client_socket, my_pseudo, my_role, nb_joueurs);

    std::cout << "Joueurs : " << std::endl;


    for (int i=0; i < nb_joueurs; i++){
        std::string paffiche = std::get<0>(assoc_pseudo_role[i]);
        Joueur jaffiche = std::get<1>(assoc_pseudo_role[i]);
        std::cout << " - " << jaffiche << " : " << paffiche;
        if (paffiche == my_pseudo){std::cout << " (vous)";}
        std::cout << std::endl;
    }

    RecupereCartes(mesCartes, client_socket);

    std::cout << std::endl << "Mes cartes : " << mesCartes << std::endl;

    while (true){

    }

    return 0;
}