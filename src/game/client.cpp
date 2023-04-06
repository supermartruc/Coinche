#include"client.hpp"
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

int GetGameInfoManche(GameInfo &gameInfo){
	std::string InfoString="";

	sf::Packet InfoStringPacket;
    gameInfo.client_socket.setBlocking(false);
	if (gameInfo.client_socket.receive(InfoStringPacket) == sf::Socket::Done){
		gameInfo.client_socket.setBlocking(true);
	}
	else{
		return 1;
	}
	InfoStringPacket >> InfoString;
	gameInfo.who_starts = intToJoueur(std::stoi(std::string {InfoString[0]}));
	gameInfo.who_plays = intToJoueur(std::stoi(std::string {InfoString[1]}));
	if (gameInfo.who_starts == gameInfo.who_plays){gameInfo.pli_en_cours = {};}
	if (InfoString[2] != '4'){
		gameInfo.pli_en_cours.push_back(intToCarte(std::stoi(std::string {InfoString[2], InfoString[3]})));
	}
	gameInfo.pli_termine = (bool)std::stoi(std::string {InfoString[4]});
	gameInfo.manche_terminee = (bool)std::stoi(std::string {InfoString[5]});
	if (!gameInfo.pli_en_cours.empty()) {gameInfo.couleur_demandee = std::get<1>(gameInfo.pli_en_cours[0]);} 

	std::cout << "Infos traitees correctement : " << InfoString << std::endl;

	return 0;
}

int GetGameInfoPoints(GameInfo &gameInfo){
	std::string InfoString;
	sf::Packet InfoStringPacket;
	InfoStringPacket.clear();

	gameInfo.client_socket.setBlocking(false);
	if (gameInfo.client_socket.receive(InfoStringPacket) == sf::Socket::Done){
		gameInfo.client_socket.setBlocking(true);
	}
	else{
		return 1;
	}
	InfoStringPacket >> InfoString;

	gameInfo.tot_points_NS = std::stoi(InfoString.substr(0,4));
	gameInfo.points_NS_fait = std::stoi(InfoString.substr(4,4));
	gameInfo.points_NS_marque = std::stoi(InfoString.substr(8,4));
	gameInfo.tot_points_OE = std::stoi(InfoString.substr(12,4));
	gameInfo.points_OE_fait = std::stoi(InfoString.substr(16,4));
	gameInfo.points_OE_marque = std::stoi(InfoString.substr(20,4));

	std::cout << "Points faits NS : " << gameInfo.points_NS_fait << std::endl;
	std::cout << "Points totaux NS : " << gameInfo.tot_points_NS << std::endl;
	std::cout << "Points marqués NS : " << gameInfo.points_NS_marque << std::endl;
	std::cout << "Points faits OE : " << gameInfo.points_OE_fait << std::endl;
	std::cout << "Points totaux OE : " << gameInfo.tot_points_OE << std::endl;
	std::cout << "Points marqués OE : " << gameInfo.points_OE_marque << std::endl;

	return 0;
}

int GetGameInfo(GameInfo &gameInfo){
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
			if (pointsRecup >= 180){
			gameInfo.annonce_min = 252;
			}
			else{
				gameInfo.annonce_min = std::max(10 + pointsRecup, 80);
			}
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
		gameInfo.view.renderEnchere(gameInfo.myPlayer.myRole, gameInfo.who_deals, gameInfo.myPlayer.mesCartes, gameInfo.annonce_temp, gameInfo.annonce_min, gameInfo.who_speaks, gameInfo.all_encheres);
		SDL_Delay(1000/60);
		GetGameInfo(gameInfo);
		gameInfo.myPlayer.mesCartes = tri_paquet_affichage(gameInfo.myPlayer.mesCartes, std::get<2>(gameInfo.current_enchere));

		if (gameInfo.ice_speak && gameInfo.view.iconeToAtout() != Atout::Rien && gameInfo.view.mouse_click){
			std::string pointenvoi;
			if (gameInfo.view.iconeToAtout() == Atout::Passe){pointenvoi = "10";}
			else{pointenvoi = std::to_string(gameInfo.annonce_temp);}
			InfoString = pointenvoi + " " + std::to_string(atoutToInt(gameInfo.view.iconeToAtout())) + "00";
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
	if (quit){exit(0);}
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
		if (quit){exit(0);}
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

void	mancheLoop(GameInfo& gameInfo){
	bool 	quit = false;
	int carteInt;
	sf::Packet	carteIntPacket;
	gameInfo.pli_termine = false;
	gameInfo.pli_en_cours = {};

	while (!quit){
		quit = !gameInfo.view.handleEvents();
		if (quit){exit(0);}
		GetGameInfoManche(gameInfo);
		if (gameInfo.manche_terminee){
			std::cout << "MANCHE TERMINEE !" << std::endl;
			break;
		}
		gameInfo.view.renderManche(gameInfo.myPlayer.myRole, gameInfo.who_deals, gameInfo.myPlayer.mesCartes, gameInfo.taille_paquets, gameInfo.current_enchere, gameInfo.who_starts, gameInfo.pli_en_cours);
		SDL_Delay(1000/60);
		if (!gameInfo.pli_termine && gameInfo.who_plays == gameInfo.myPlayer.myRole && std::get<0>(gameInfo.view.clicToCarte(gameInfo.myPlayer.mesCartes)) != Valeur::Rien && gameInfo.view.mouse_click){
			if (!est_valide_carte(gameInfo.view.clicToCarte(gameInfo.myPlayer.mesCartes), gameInfo.myPlayer.mesCartes, gameInfo.couleur_demandee, gameInfo.atout_actuel, gameInfo.pli_en_cours, gameInfo.myPlayer.myRole, gameInfo.who_starts)){
				std::cout << "Carte invalide : " << gameInfo.view.clicToCarte(gameInfo.myPlayer.mesCartes) << std::endl;
				continue;
			}
			carteInt = carteToInt(gameInfo.view.clicToCarte(gameInfo.myPlayer.mesCartes));
			carteIntPacket.clear();
			carteIntPacket << carteInt;
			gameInfo.client_socket.setBlocking(true);
			gameInfo.client_socket.send(carteIntPacket);
			gameInfo.client_socket.setBlocking(false);
			gameInfo.who_plays = intToJoueur((1+joueurToInt(gameInfo.who_plays))%4);
			//std::cout << "Jai essaye d'envoyer le : " << intToCarte(carteInt) << std::endl;
			for (int i=0;i<gameInfo.myPlayer.mesCartes.size();i++){
				if (carteInt == carteToInt(gameInfo.myPlayer.mesCartes[i])){
					gameInfo.myPlayer.mesCartes.erase(gameInfo.myPlayer.mesCartes.begin()+i);
				}
			}
		}
	}
}

int clientmain(){
	GameInfo	gameInfo;

	gameInfo.couleur_demandee = Couleur::Pique;
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

	gameInfo.myPlayer.mesCartes = tri_paquet_affichage(gameInfo.myPlayer.mesCartes, Atout::Sa);

	enchereLoop(gameInfo);

	gameInfo.atout_actuel = std::get<2>(gameInfo.current_enchere);

	gameInfo.myPlayer.mesCartes = tri_paquet_affichage(gameInfo.myPlayer.mesCartes, std::get<2>(gameInfo.current_enchere));

	if (gameInfo.atout_actuel == Atout::Passe){
		exit(0);
	}

	mancheLoop(gameInfo);

	bool quit=0;
	while (GetGameInfoPoints(gameInfo)){
		quit = !gameInfo.view.handleEvents();
		if (quit){exit(0);}
	}

	while (true){
		quit = !gameInfo.view.handleEvents();
		if (quit){exit(0);}
		if (gameInfo.myPlayer.myRole == Joueur::Nord || gameInfo.myPlayer.myRole == Joueur::Sud){
			gameInfo.view.renderGlobalPoints(gameInfo.tot_points_NS, gameInfo.tot_points_OE, gameInfo.points_NS_fait, gameInfo.points_OE_fait, gameInfo.points_NS_marque, gameInfo.points_OE_marque);
		}
		else{
			gameInfo.view.renderGlobalPoints(gameInfo.tot_points_OE, gameInfo.tot_points_NS, gameInfo.points_OE_fait, gameInfo.points_NS_fait, gameInfo.points_OE_marque, gameInfo.points_NS_marque);
		}
		SDL_Delay(1000/60);
	}


	SDL_Quit(); 

	return 42;

}