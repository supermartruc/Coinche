#include "client.hpp"
#include"timer.hpp"

int ConnexionServer(sf::TcpSocket &client_socket, std::string ipaddress, int port, std::string &my_pseudo){
	if (client_socket.connect(ipaddress,port) == sf::Socket::Done){
		std::cout << "Connexion réussie ! " << std::endl;
		sf::Packet my_pseudoPacket;
		my_pseudoPacket << my_pseudo;
		client_socket.send(my_pseudoPacket);
		std::cout << "OK apres connexion serv" << std::endl;
		return 0;
	}
	else {
		std::cout << "Connexion échouée... " << std::endl;
		return 1;
	}
}


int GetGameInfo(GameInfo &gameInfo){
	int junk = 0;
	int InfoInt=0;

    sf::Packet InfoIntPacket;
    gameInfo.client_socket.setBlocking(false);
	if (gameInfo.client_socket.receive(InfoIntPacket) == sf::Socket::Done){
		gameInfo.client_socket.setBlocking(true);
	}
	else{
		return 1;
	}
    InfoIntPacket >> InfoInt;
    gameInfo.who_deals = intToJoueur((InfoInt/1000)%10);
    gameInfo.who_plays = intToJoueur((InfoInt/100)%10);
    gameInfo.who_speaks = intToJoueur((InfoInt/10)%10);
    gameInfo.enchere_en_cours = (bool)(InfoInt%10);
	gameInfo.ice_speak = gameInfo.myPlayer.myRole == gameInfo.who_speaks;

    std::string EnchereString;
    sf::Packet EnchereStringPacket;
    Joueur joueurRecup;
    int pointsRecup;
    Atout atoutRecup;
    bool coincheRecup, surcoincheRecup;

    for (int i=0; i<5; i++){
		gameInfo.client_socket.receive(EnchereStringPacket);
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
		if (i==0){
			gameInfo.current_enchere = Enchere {joueurRecup, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup};
			gameInfo.annonce_min = std::max(10 + pointsRecup, 80);
			gameInfo.annonce_temp = gameInfo.annonce_min;
		}
		else{gameInfo.all_encheres[i-1] = Enchere {joueurRecup, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup};}      
	}
	return 0;
}

void	enchereLoop(GameInfo& gameInfo) {
	bool		quit = false;
	std::string		InfoString = "";
	sf::Packet		InfoStringPacket;


	while (!quit) {
		quit = !gameInfo.view.handleEvents();
		gameInfo.view.render(gameInfo.myPlayer.myRole, gameInfo.who_deals, gameInfo.myPlayer.mesCartes, {8,8,8,8}, gameInfo.annonce_min, gameInfo.annonce_temp, gameInfo.who_speaks, gameInfo.all_encheres);
		SDL_Delay(1000/60);
		GetGameInfo(gameInfo);
		if (gameInfo.ice_speak && gameInfo.view.iconeToAtout() != Atout::Rien && gameInfo.view.mouse_click){
			InfoString = std::to_string(gameInfo.annonce_temp) + " " + std::to_string(atoutToInt(gameInfo.view.iconeToAtout())) + "00";
			InfoStringPacket.clear();
			InfoStringPacket << InfoString;
			gameInfo.client_socket.setBlocking(true);
			gameInfo.client_socket.send(InfoStringPacket);
			gameInfo.client_socket.setBlocking(false);
			gameInfo.ice_speak = false;
		}
		if (!gameInfo.enchere_en_cours){
			std::cout << "ENCHERE TERMINEES" << std::endl;
			break;
		}
	}
}

void	RecupereCartes(GameInfo& gameInfo) {
	gameInfo.myPlayer.mesCartes.clear();
	std::string									mesCartesString = "";
	sf::Packet									mesCartesStringPacket;
	gameInfo.client_socket.receive(mesCartesStringPacket);
	mesCartesStringPacket >> mesCartesString;
	
	std::string									UneCarteString = "";
	for (int i = 0; i < mesCartesString.size(); i++) {
		if (mesCartesString[i] == ' ') {
			gameInfo.myPlayer.mesCartes.push_back(intToCarte(std::stoi(UneCarteString)));
			UneCarteString = "";
		}
		else {
			UneCarteString += mesCartesString[i];
		}
	}
}

void	loopLobby(GameInfo& gameInfo) {
	bool										quit = false;
	sf::Packet									PseudoReceivePacket;
	sf::Packet									RoleReceivePacket;
	std::string									pseudoReceive;
	int											roleIntReceive;
	Joueur										roleReceive;
	bool										pseudoReceived = false;
	bool										roleReceived = false;
	int											nb_joueurs = 0;

	gameInfo.view.init();
	gameInfo.client_socket.setBlocking(false);
	while (!quit) {
		quit = !gameInfo.view.handleEvents();
		gameInfo.view.clear(true);
		SDL_Delay(1000/60);
		if (!pseudoReceived and gameInfo.client_socket.receive(PseudoReceivePacket) != sf::Socket::Done) {
			continue;
		}
		pseudoReceived = true;
		PseudoReceivePacket >> pseudoReceive;
		if (!roleReceived and gameInfo.client_socket.receive(RoleReceivePacket) != sf::Socket::Done) {
			continue;
		}
		roleReceived = true;
		RoleReceivePacket >> roleIntReceive;
		roleReceive = intToJoueur(roleIntReceive);
		std::cout << pseudoReceive << " : " << roleReceive << std::endl;
		if (pseudoReceive == gameInfo.myPlayer.myPseudo){
			gameInfo.myPlayer.myRole = roleReceive;
		}
		gameInfo.assoc_pseudo_role.push_back({pseudoReceive,roleReceive});
		nb_joueurs++;
		if (nb_joueurs == 4) {
			break;
		} else {
			pseudoReceived = false;
			roleReceived = false;
		}
	}
	gameInfo.ice_speak = gameInfo.who_speaks == gameInfo.myPlayer.myRole;
	gameInfo.client_socket.setBlocking(true);
	RecupereCartes(gameInfo);	// Temps instantané car le serv envoie tout d'un coup
	gameInfo.all_encheres = {{Joueur::Nord,10,Atout::Rien,false,false},
										{Joueur::Est,10,Atout::Rien,false,false},
										{Joueur::Sud,10,Atout::Rien,false,false},
										{Joueur::Ouest,10,Atout::Rien,false,false}};

}


int clientmain(){
	GameInfo	gameInfo;

	gameInfo.who_deals = Joueur::Ouest;
	gameInfo.who_speaks = Joueur::Nord;
	gameInfo.who_plays = Joueur::Nord;
	gameInfo.annonce_min = 80;
	gameInfo.annonce_temp = 80;
	gameInfo.ice_speak = false;
	gameInfo.taille_paquets = {8,8,8,8};

	int port = 1234;
	std::string ipaddress;

	std::cout << "Adresse IP : " << std::flush;
	std::cin >> ipaddress;
	std::cout << "Port : " << std::flush;
	std::cin >> port;


	std::cout << "Votre pseudo : ";
	std::cin >> gameInfo.myPlayer.myPseudo;
	std::cin.ignore();

	if (ConnexionServer(gameInfo.client_socket,ipaddress,port,gameInfo.myPlayer.myPseudo)){
		std::cerr << "Erreur de connexion au serveur." << std::endl;
		exit(1);
	}


	// getPseudoRole(assoc_pseudo_role, client_socket, my_pseudo, my_role, nb_joueurs, view);
	loopLobby(gameInfo);

	enchereLoop(gameInfo);

	SDL_Quit(); 

	return 42;

}