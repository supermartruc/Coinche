#include"game.hpp"
#include"timer.hpp"
#include"server.hpp"
#include"client.hpp"

void	loop(Jeu game) {
	SDL_Event		event;
	bool			quit = false;
	bool 			mouse_pressed = false;
	char 			key_pressed = '/';
	GameView		view;
	Timer			timer;
	int 			sx;
	int				sy;
	int 			sotg = 0;
	int 			t = 100;

	SDL_Init(SDL_INIT_VIDEO);
	view.init();
	timer.start();
	view.startAnimations();
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_p:
						key_pressed = 'p';
						break;
					case SDLK_c:
						key_pressed = 'c';
						break;
					case SDLK_k:
						key_pressed = 'k';
						break;
					case SDLK_t:
						key_pressed = 't';
						break;
					case SDLK_s:
						key_pressed = 's';
						break;
					case SDLK_a:
						key_pressed = 'a';
						break;
					case SDLK_SPACE:
						key_pressed = ' ';
						break;
				}
			}

			if (event.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&sx, &sy);
        	}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				mouse_pressed = true;
        	}
			if (event.type == SDL_MOUSEBUTTONUP) {
				mouse_pressed = false;
        	}
		}
		t = std::max(0,t-1);
		int you = joueurToInt(game.you);
		Joueur haut = intToJoueur((you+2)%4);
		Joueur gauche = intToJoueur((you+1)%4);
		Joueur droite = intToJoueur((you+3)%4);
		view.clear();
		view.renderPaquet(game.getPaquet(game.you), sx, sy);
		view.renderRetournees(game.getPaquet(gauche), game.getPaquet(haut), game.getPaquet(droite));
		view.renderDealer(you - (1+joueurToInt(game.who_cuts)));
		view.renderMenu((int) (3 / 8.0 * view.wWindow), view.hWindow - (int) (2.1 * view.hCarte), sx, sy);
		view.renderAnimations();
		view.render();
		SDL_Delay(std::max(0, 1000/60 - timer.get_ticks()));
		view.updateAnimations(timer.get_ticks());
		timer.start();

		switch (sotg)
		{
		case 0: // Phase des enchères
			if (game.you == game.who_plays && key_pressed != '/' && t == 0){
				int points = std::get<1>(game.current_enchere);
				Joueur who_bids = std::get<0>(game.current_enchere);
				if (key_pressed == ' '){
					t = 100;
					key_pressed = '/';
					game.who_plays = intToJoueur((1+joueurToInt(game.who_plays))%4);
					game.you = game.who_plays;
					if (who_bids == game.you){
						sotg = 1;
					}
					break;
				}
				else{
					game.current_enchere = Enchere {game.you,std::max(80,points+10),keyToAtout(key_pressed),false,false};
					t = 100;
					key_pressed = '/';
					game.who_plays = intToJoueur((1+joueurToInt(game.who_plays))%4);
					game.you = game.who_plays;
					break;
				}
			}
			else{
				break;
			}
		
		case 1: // Phase de jeu 
			std::cout << game.current_enchere << std::endl;
			quit = true;
			break;

		default:
			break;
		}
	}
	SDL_Quit();
}

int main() {
	
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
	
	Jeu game;
	game.createRandomPaquet();
	game.distributionPaquet(Joueur::Sud, 16);

	loop(game);
	
	return 0;
}