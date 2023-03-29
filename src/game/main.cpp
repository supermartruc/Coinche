#include "view.hpp"
#include "server.hpp"
#include "client.hpp"
#include <algorithm>

void loop_testrender() {
	GameView view;
	Paquet mypaquet = {{Valeur::Valet, Couleur::Trefle}, {Valeur::Valet, Couleur::Carreau}, {Valeur::Valet, Couleur::Pique}, {Valeur::Valet, Couleur::Coeur}, {Valeur::Dix, Couleur::Coeur}, {Valeur::As, Couleur::Trefle}, {Valeur::Roi, Couleur::Trefle}, {Valeur::Dame, Couleur::Trefle}};
	std::vector<Enchere> encheres; 
	encheres.push_back({Joueur::Nord,100,Atout::Coeur,false,false});
	encheres.push_back({Joueur::Est,90,Atout::Coeur,false,false});
	encheres.push_back({Joueur::Sud,10,Atout::Trefle,false,false});
	encheres.push_back({Joueur::Ouest,10,Atout::Carreau,false,false});
	int annonce_temp = 80;
	bool quit = false;
	view.init();
	while (!quit) {
		quit = !view.handleEvents();
		view.render(Joueur::Sud, Joueur::Est, mypaquet, {8,8,8,8}, 80, annonce_temp, Joueur::Sud, encheres);
		SDL_Delay(1000/60);
	}
}

int		main(void) {

	// loop_testrender(); exit(0);

	std::cout << "Serveur ou client ? (s/c)" << std::endl;

	std::string instance_role;
	std::cin >> instance_role;

	if (instance_role == "s"){
		if (servermain()){
			exit(0);
		}
	}
	else if(instance_role == "c"){
		if (clientmain()){
			exit(0);
		}
	}
	else {
		std::cout << "Role invalide ! " << std::endl;
		exit(0);
	}
	return 0;
}