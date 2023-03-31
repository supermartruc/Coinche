#include "server.hpp"

void ConnexionPhase(int nb_joueurs_max, sockvec &NetJoueurs, std::vector<std::string> &Pseudos, sf::TcpListener &listener, sf::SocketSelector selector){
	bool all_connected = false;
	int nb_joueurs_connectes = 0;
	while (!all_connected){
		if (selector.wait(sf::milliseconds(100))){
			if (selector.isReady(listener)){
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
					std::cout << "Tous les joueurs sont connectés !" << std::endl << std::flush;
					all_connected = true;
					listener.close();
					selector.remove(listener);
					return;
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

int AskCuts(Jeu &game, sockvec NetJoueurs){
	return 16;
}

int CreeEnvoiePaquet(Jeu &game, sockvec NetJoueurs){
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

void SendGameInfo(Jeu &game, sockvec NetJoueurs){
	int InfoInt = (int)(game.enchere_en_cours) + 10*joueurToInt(game.who_speaks) + 100*joueurToInt(game.who_plays) + 1000*((joueurToInt(game.who_cuts)+1)%4);
	sf::Packet InfoIntPacket;
	InfoIntPacket << InfoInt;
	for (int i=0; i<NetJoueurs.size();i++){
		sf::TcpSocket *client_socket = NetJoueurs[i];
		if (client_socket->send(InfoIntPacket) == sf::Socket::Done){}
		else{std::cout << "Envoi échoué" << std::endl;}
	}

	std::string EnchereString;
	sf::Packet EnchereStringPacket;
	Joueur joueurEnchere;
	int pointEnchere;
	Atout atoutEnchere;
	bool coincheEnchere;
	bool surcoincheEnchere;
	for (int i=0; i<5; i++){
		if (i==0){
			joueurEnchere = std::get<0>(game.current_enchere);
			pointEnchere = std::get<1>(game.current_enchere);
			atoutEnchere = std::get<2>(game.current_enchere);
			coincheEnchere = std::get<3>(game.current_enchere);
			surcoincheEnchere = std::get<4>(game.current_enchere);
		}
		else{
			joueurEnchere = std::get<0>(game.all_enchere[i-1]);
			pointEnchere = std::get<1>(game.all_enchere[i-1]);
			atoutEnchere = std::get<2>(game.all_enchere[i-1]);
			coincheEnchere = std::get<3>(game.all_enchere[i-1]);
			surcoincheEnchere = std::get<4>(game.all_enchere[i-1]);
		}
		EnchereString = "";
		EnchereStringPacket.clear();
		EnchereString += std::to_string(joueurToInt(joueurEnchere)) +
						std::to_string(pointEnchere) + " " + 
						std::to_string(atoutToInt(atoutEnchere)) + 
						std::to_string((int)(coincheEnchere)) + 
						std::to_string((int)(surcoincheEnchere));
		EnchereStringPacket << EnchereString;
		for (int j=0; j < 4; j++){
			sf::TcpSocket *client_socket = NetJoueurs[j];
			if (client_socket->send(EnchereStringPacket) == sf::Socket::Done){}
			else{std::cout << "Envoi échoué enchere" << std::endl;}

		}
		
	}
	
}

void PhaseEncheres(Jeu &game, sockvec NetJoueurs, sf::SocketSelector &selector){
	sf::TcpSocket *client_socket = NetJoueurs[joueurToInt(game.who_speaks)];
	std::cout << "En attente de l'enchere de " << game.who_speaks << std::endl;

	std::cout << "On va receptionner l'enchere de " << game.who_speaks << std::endl;
	sf::Packet EnchereStringPacket;
	EnchereStringPacket.clear();
	std::string EnchereString = "";
	int pointsRecup;
	Atout atoutRecup;
	bool coincheRecup, surcoincheRecup;
	if (client_socket->receive(EnchereStringPacket) == sf::Socket::Done) {
		EnchereStringPacket >> EnchereString;
		std::cout << "Enchere recuperee : elle vaut " << EnchereString << std::endl;
		if (EnchereString[2] == ' '){
			std::cout << "case blank" << std::endl;
			pointsRecup = std::stoi(std::string {EnchereString[0], EnchereString[1]});
			atoutRecup = intToAtout(std::stoi(std::string {EnchereString[3]}));
			coincheRecup = bool(std::stoi(std::string {EnchereString[4]}));
			surcoincheRecup = bool(std::stoi(std::string {EnchereString[5]}));
		}
		else{
			std::cout << "case not blank" << std::endl;
			pointsRecup = std::stoi(std::string {EnchereString[0], EnchereString[1], EnchereString[2]});
			atoutRecup = intToAtout(std::stoi(std::string {EnchereString[4]}));
			coincheRecup = (bool)(std::stoi(std::string {EnchereString[5]}));
			surcoincheRecup = (bool)(std::stoi(std::string {EnchereString[6]}));
		}
		std::cout << "Enchere recuperee : " << Enchere {game.who_speaks, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup} << std::endl;
		if (std::get<2>(game.current_enchere) == Atout::Rien || atoutRecup != Atout::Passe){
			game.current_enchere = Enchere {game.who_speaks, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup};
		}
		game.all_enchere[joueurToInt(game.who_speaks)] = Enchere {game.who_speaks, pointsRecup, atoutRecup, coincheRecup, surcoincheRecup};
	}
	else{
		std::cout << "Erreur recep enchere" << std::endl;
	}
	game.who_speaks = intToJoueur((1+joueurToInt(game.who_speaks))%4);
	
}

void SendGameInfoPli(Jeu &game, sockvec NetJoueurs, bool ajoutcarte, bool finmanche){
	std::string InfoString;
	sf::Packet 	InfoStringPacket;

	std::string carteEnvoi = "";
	if (!ajoutcarte){carteEnvoi = "40";}
	else{
		if (carteToInt(game.pli_actuel[game.pli_actuel.size()-1]) <= 9){
			carteEnvoi = "0" + std::to_string (carteToInt(game.pli_actuel[game.pli_actuel.size()-1]));
		}
		else{
			carteEnvoi = std::to_string (carteToInt(game.pli_actuel[game.pli_actuel.size()-1]));
		}
	}

	InfoString = std::to_string(joueurToInt(game.who_starts)) 
				+ std::to_string(joueurToInt(game.who_plays))
				+ carteEnvoi
				+ std::to_string (!game.pli_en_cours)
				+ std::to_string(finmanche);
	InfoStringPacket.clear();
	InfoStringPacket << InfoString;
	for (int i=0; i<NetJoueurs.size();i++){
		sf::TcpSocket *client_socket = NetJoueurs[i];
		if (client_socket->send(InfoStringPacket) == sf::Socket::Done){}
		else{std::cout << "Envoi échoué" << std::endl;}
	}

}

int servermain(){

	Jeu game;
	std::vector<Joueur> Roles = {Joueur::Nord,Joueur::Est,Joueur::Sud,Joueur::Ouest};

	int nb_joueurs_max = 4;

	int port = 1234;
	std::cout << "Entrez le port : " << std::flush;
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

	while (game.enchere_en_cours){
		std::cout << "current enchere : " << game.current_enchere << std::endl;
		for (int i=0; i<4; i++){
			std::cout << game.all_enchere[i] << std::endl;
		}
		PhaseEncheres(game, NetJoueurs, selector);
		if(std::get<1>(game.current_enchere) == 252 || (game.who_speaks == std::get<0>(game.current_enchere) && std::get<2>(game.all_enchere[(joueurToInt(game.who_speaks)+3)%4]) == Atout::Passe)){
			game.enchere_en_cours = false;
		}
		SendGameInfo(game, NetJoueurs);

	}
	std::cout << "ENCHERE TERMINEE" << std::endl;

	std::cout << "Enchere gagnante : " << game.current_enchere << std::endl;

	game.who_starts = game.who_speaks;

	for (int i=0; i<8; i++){
		game.who_plays = game.who_starts;
		SendGameInfoPli(game, NetJoueurs, false, false);

	}


	selector.clear();
	NetJoueurs.clear();

	return 0;

}