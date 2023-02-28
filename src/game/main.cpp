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
	game.createRandomPaquet();
    game.affichePaquetListe(game.paquet);
	game.distributionPaquet(game.who_cuts, 13);
    game.afficheAllPaquetsListe();
	game.next_enchere(game.who_plays, true);
	game.next_to_play();
	while (not game.next_enchere(game.who_plays, false)){
		game.next_to_play();
	}
	while (std::get<2>(game.current_enchere)==Atout::Passe){
		game.next_to_cut();
		game.next_to_play();
		game.fusionPaquets();
		game.current_enchere = {game.who_plays,0,Atout::Passe,false,false};
		std::cout << std::endl << "Pas d'enchere, redistribution ; " << game.who_cuts << " coupe. " << std::endl;
		game.distributionPaquet(game.who_cuts, 13);
		game.next_enchere(game.who_plays, true);
		game.next_to_play();
		while (not game.next_enchere(game.who_plays, false)){
			game.next_to_play();
		}
	}
	std::cout << std::endl << "Contrat : " << game.current_enchere << std::endl;

	for (int pli = 0; pli<8; pli++){
		game.joue_pli();
	}
	//loop(game);

	return 0;
}