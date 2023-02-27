#include<iostream>
#include"game.hpp"

void	loop(Jeu game) {
	SDL_Event		event;
	bool			quit = false;
	GameView		view;

	SDL_Init(SDL_INIT_VIDEO);
	view.init();
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
			}
		}
	}
	SDL_Quit();
}

int main() {
	Jeu game;
	Paquet mypaquet = game.createRandomPaquet();
    game.affichePaquetListe(mypaquet);
	game.initPaquet(mypaquet, Joueur::Sud, 0);
    game.afficheAllPaquetsListe();
	//loop(game);

	return 0;
}