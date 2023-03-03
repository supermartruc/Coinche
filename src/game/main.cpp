#include"game.hpp"
#include"timer.hpp"

void	loop(Jeu game) {
	SDL_Event		event;
	bool			quit = false;
	GameView		view;
	Timer			timer;

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
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
			}
		}
		view.clear();
		int you = joueurToInt(game.you);
		Joueur haut = intToJoueur((you+2)%4);
		Joueur gauche = intToJoueur((you+1)%4);
		Joueur droite = intToJoueur((you+3)%4);
		view.renderPaquet(game.getPaquet(game.you));
		view.renderRetournees(game.getPaquet(gauche), game.getPaquet(haut), game.getPaquet(droite));
		view.renderDealer(you - (1+joueurToInt(game.who_cuts)));
		view.renderAnimations();
		view.render();
		SDL_Delay(std::max(0, 1000/60 - timer.get_ticks()));
		view.updateAnimations(timer.get_ticks());
		timer.start();
	}
	SDL_Quit();
}

int 	main() {
	Jeu game;
	game.createRandomPaquet();
    game.affichePaquetListe(game.paquet);
	game.distributionPaquet(game.who_cuts, 13);
	
	loop(game);
	// game.affichePaquetListe(tri_paquet_affichage(game.allPaquets[2],Atout::Sa));
	//std::cout << compareCarte(Carte{Valeur::Valet,Couleur::Trefle},Carte{Valeur::Huit,Couleur::Trefle},Couleur::Trefle,Atout::Sa) << std::endl;

    game.afficheAllPaquetsListe();
	Joueur first_player = game.who_plays;
	game.next_enchere(game.who_plays, true);
	game.next_to_play();
	while (not game.next_enchere(game.who_plays, false)){
		game.next_to_play();
	}
	while (std::get<2>(game.current_enchere)==Atout::Passe){
		game.next_to_cut();
		game.next_to_play();
		first_player = game.who_plays;
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
	game.atout_actuel = std::get<2>(game.current_enchere);
	game.who_plays = first_player;
	for (int pli = 0; pli<8; pli++){
		game.joue_pli();
	}
	return 0;
}