#include "client.hpp"

int clientmain(){

    int port = 1234;
    std::string ipaddress;

    std::cout << "Adresse IP : " << std::flush;
    std::cin >> ipaddress;

    std::string client_pseudo;
    std::cout << "Votre pseudo : ";
    std::cin >> client_pseudo;
    std::cin.ignore();

    sf::TcpSocket client_socket;

    if (client_socket.connect(ipaddress,port) == sf::Socket::Done){
        std::cout << "Connexion réussie ! " << std::endl;
        sf::Packet client_pseudoPacket;
        client_pseudoPacket << client_pseudo;
        client_socket.send(client_pseudoPacket);
    }

    while (true){

    }

    return 0;
}