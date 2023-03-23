#include "client.hpp"
#include"timer.hpp"

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

void GetGameInfo(Joueur &who_deals, Joueur &who_speaks, Joueur &who_plays, bool &enchere_en_cours, sf::TcpSocket &client_socket,
                std::vector<Enchere> &all_encheres, Enchere &current_enchere){
    int InfoInt=0;
    
    sf::Packet InfoIntPacket;
    if (client_socket.receive(InfoIntPacket) == sf::Socket::Done){
        InfoIntPacket >> InfoInt;
    }
    who_deals = intToJoueur((InfoInt/1000)%10);
    who_plays = intToJoueur((InfoInt/100)%10);
    who_speaks = intToJoueur((InfoInt/10)%10);
    enchere_en_cours = (bool)(InfoInt%10);

    std::string EnchereString;
    sf::Packet EnchereStringPacket;
    Joueur joueurRecup;
    int pointsRecup;
    Atout atoutRecup;
    bool coincheRecup, surcoincheRecup;

    for (int i=0; i<5; i++){
        if (client_socket.receive(EnchereStringPacket) == sf::Socket::Done) {
            EnchereStringPacket >> EnchereString;
            if (EnchereString[3] == ' '){
                joueurRecup = intToJoueur(std::stoi(std::string {EnchereString[0]}));
                pointsRecup = std::stoi(std::string {EnchereString[1], EnchereString[2]});
                atoutRecup = intToAtout(std::stoi(std::string {EnchereString[4]}));
                coincheRecup = bool(std::stoi(std::string {EnchereString[5]}));
                surcoincheRecup = bool(std::stoi(std::string {EnchereString[6]}));
            }
            else{
                joueurRecup = intToJoueur(std::stoi(std::string {EnchereString[0]}));
                pointsRecup = std::stoi(std::string {EnchereString[1], EnchereString[2], EnchereString[3]});
                atoutRecup = intToAtout(std::stoi(std::string {EnchereString[5]}));
                coincheRecup = (bool)(std::stoi(std::string {EnchereString[6]}));
                surcoincheRecup = (bool)(std::stoi(std::string {EnchereString[7]}));
            }
            if (i==0){current_enchere = Enchere {joueurRecup, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup};}
            else{all_encheres[i-1] = Enchere {joueurRecup, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup};}      
        }
    }
}

void GetEnchere(Joueur my_role, int &pointclient, Atout &atoutclient, bool &coincheclient, bool &surcoincheclient, Enchere current_enchere){
    bool enchere_not_valid = true;
    while (enchere_not_valid){
        std::cout << "Enchere en cours : " << current_enchere << std::endl;
        if (std::get<3>(current_enchere)){ //enchere deja coinche
            std::cout << "Surcoincher ? " << std::flush;
            std::cin >> surcoincheclient;
            if (surcoincheclient){
                break;
            }
        }
        if (std::get<2>(current_enchere) != Atout::Passe && std::get<2>(current_enchere) != Atout::Rien && std::get<0>(current_enchere) != intToJoueur((2+joueurToInt(my_role))%4)){
            std::cout << "Coincher ? " << std::flush;
            std::cin >> coincheclient;
            if (coincheclient){
                break;
            }
        }
        coincheclient = false;
        surcoincheclient = false;
        std::cout << "Atout : " << std::flush;
        std::string tempatout;
        std::cin >> tempatout;
        atoutclient = stringToAtout(tempatout);
        if (atoutclient == Atout::Passe){
            pointclient = 10;
            enchere_not_valid = false;
            break;
        }
        else{
            std::cout << "Points : " << std::flush;
            std::cin >> pointclient;
            if (pointclient%10 == 0 && pointclient >= 80 && pointclient <= 270 && pointclient >= 10 + std::get<1>(current_enchere)){
                enchere_not_valid = false;
                break;
            }
            
        }
        std::cout << "Enchere invalide, recommencez." << std::endl;
    }

}

void SendEnchere(int pointclient, Atout atoutclient, bool coincheclient, bool surcoincheclient, sf::TcpSocket &client_socket){
    sf::Packet EnchereStringPacket;
    std::string EnchereString = "";
    EnchereString += std::to_string(pointclient) + " " + std::to_string(atoutToInt(atoutclient)) + std::to_string((int)(coincheclient)) + std::to_string((int)(surcoincheclient));
    EnchereStringPacket << EnchereString;
    if (client_socket.send(EnchereStringPacket) == sf::Socket::Done){std::cout << "Paquet envoye enchere" << std::endl;}
    else {std::cout << "Erreur envoi enchere." << std::endl;}
}

int clientmain(){

	SDL_Event		event;
	bool			quit = false;
	bool 			mouse_pressed = false;
	GameView		view;
	Timer			timer;
	int 			sx;
	int				sy;
	int 			t = 100;



    int nb_joueurs = 4;
    Joueur my_role;

    Joueur who_deals = Joueur::Ouest;
    Joueur who_speaks = Joueur::Nord;
    Joueur who_plays = Joueur::Nord;

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

    std::vector<Enchere> all_encheres = {{Joueur::Nord,0,Atout::Rien,false,false},
                                         {Joueur::Est,0,Atout::Rien,false,false},
                                         {Joueur::Sud,0,Atout::Rien,false,false},
                                         {Joueur::Ouest,0,Atout::Rien,false,false}};

    Enchere current_enchere = {Joueur::Nord,0,Atout::Rien,false,false};    
    bool enchere_en_cours = true;

    Joueur joueurEnchere;
    int pointclient;
    Atout atoutclient;
    bool coincheclient;
    bool surcoincheclient;

    GetGameInfo(who_deals, who_speaks, who_plays, enchere_en_cours, client_socket, all_encheres, current_enchere);

    while (enchere_en_cours){ 
        if (who_speaks == my_role){
            GetEnchere(my_role, pointclient,atoutclient,coincheclient,surcoincheclient,current_enchere);
            SendEnchere(pointclient,atoutclient,coincheclient,surcoincheclient,client_socket);
        }
        GetGameInfo(who_deals, who_speaks, who_plays, enchere_en_cours, client_socket, all_encheres, current_enchere);
    }
    std::cout << "ENCHERE TERMINEE" << std::endl;

    return 0;
}