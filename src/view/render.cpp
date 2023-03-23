#include "view.hpp"
#include <algorithm>
#include <vector>

void	GameView::renderCarte(Carte carte, int x, int y) {
	auto [valeur, couleur] = carte;
	SDL_Rect rect = {x, y, wCarte, hCarte};
	SDL_RenderCopy(renderer, textures[valeur][couleur], NULL, &rect); 
	/*
	rect = {x - 1, y - 1, wCarte + 2, hCarte + 2};
	SDL_RenderDrawRect(renderer, &rect);
	rect = {x - 2, y - 2, wCarte + 4, hCarte + 4};
	SDL_RenderDrawRect(renderer, &rect);
	*/	
}

void 	GameView::renderDosV(int x, int y) {
	SDL_Rect rect = {x, y, wCarte, hCarte};
	SDL_RenderCopy(renderer, dos_carteV, NULL, &rect);
}

void 	GameView::renderDosH(int x, int y) {
	SDL_Rect rect = {x, y, hCarte, wCarte};
	SDL_RenderCopy(renderer, dos_carteH, NULL, &rect);
}

void	GameView::renderDealer(int dist_trigo) {
	int x=0;
	int y=0;
	switch ((4+dist_trigo)%4) {
	case 0:
		x = 3*wWindow/4; y = hWindow-21*hCarte/20-90; break;
	case 1:
		x = 21*hCarte/20; y = 5*hWindow/6; break;
	case 2:
		x = wWindow/4; y = 21*hCarte/20; break;
	case 3:
		x = wWindow-21*hCarte/20-90; y = hWindow/6; break;
	default: 
		break;
	}
	/*
	switch ((8+dist_trigo)%4) {
	case 0:
		x = wWindow-100; y = hWindow-hCarte-10; break;
	case 1:
		x = hCarte+10; y = hWindow-100; break;
	case 2:
		x = wWindow-100; y = hCarte+10; break;
	case 3:
		x = wWindow-hCarte-10; y = 100; break;
	default: 
		break;
	}*/
	SDL_Rect rect = {x, y, 90, 90};
	SDL_RenderCopy(renderer, jeton, NULL, &rect);
}

void	GameView::clear(bool refresh) {
	//204021
	while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;
				}
			}
			if (event.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&sx, &sy);
        	}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				mouse_pressed = true;
				mouse_click = false;
        	}
			if (event.type == SDL_MOUSEBUTTONUP) {
				if (mouse_pressed) {
					mouse_click = true;
				} else {
					mouse_click = false;
				}
				mouse_pressed = false;
        	}
		}
	if (quit){
		SDL_Quit();
		exit(0);
	}
	SDL_SetRenderDrawColor(renderer, 0x20, 0x40, 0x33, 0xFF);
	SDL_RenderClear(renderer);
	SDL_Rect rect = {0, 0, wWindow, hWindow};
	SDL_RenderCopy(renderer, fond, NULL, &rect);
	if (refresh) {
    	SDL_RenderPresent(renderer);
	}
}

void	GameView::renderPaquet(Paquet paquet) {
	int i = 0;
	float temp = hWindow / ( (float) ((paquet.size()+0.5)*wCarte) );
	float chev = (float) std::min((float) 1, (float) (temp * 4/5.0) );
	int wStart = (wWindow - (chev*wCarte * paquet.size())) / 2;
	Paquet copy_paquet = tri_paquet_affichage(paquet, Atout::Sa);
	Carte carte;
	for (int j=0; j < copy_paquet.size(); j++) {
		carte = copy_paquet[j];
		int l = wStart + (i * chev * wCarte);
		int h = hWindow-hCarte;
		if (isInsideRectangle( l, h, std::max(chev*wCarte,(float)wCarte*(j==copy_paquet.size()-1)))) {
			renderCarte(carte, l, h-elevation);
		}
		else{
			renderCarte(carte, l, h);
		}
		i++;
	}
}

void GameView::renderRetournees(int gauche, int haut, int droite) {
	float temp = 4/5.0 * hWindow / ( (float) ((haut+1)*wCarte) );
	float chev = (float) std::min((float) 4/5, (float) temp );
	float tempV = hWindow / ( (float) ((haut+0.5)*wCarte) );
	float chevV = (float) std::min((float) 1, (float) (tempV * 4/5.0) );
	int wStartV = (wWindow - (chevV*wCarte * haut)) / 2;
	int i = 0;
	int wStart = (wWindow - (chev*wCarte * haut)) / 2;
	while (haut--) {
		renderDosV(wStartV + i*chevV*wCarte, 0);
		i++;
	}
	i = 0;
	int hStart = (hWindow - (chev*wCarte * gauche)) / 2;
	while (gauche--) {
		renderDosH(0, hStart + i*chev*wCarte);
		i++;
	}
	i = 0;
	hStart = (hWindow - (chev*wCarte * droite)) / 2;
	while (droite--) {
		renderDosH(wWindow-hCarte, hStart + i * chev *wCarte);
		i++;
	}
}

void 	GameView::render_nombre(int nombre, int x, int y, int taille) {
	std::string nbr = std::to_string(nombre);
	for (int i=0; i<nbr.size(); i++) {
		SDL_Rect rect = {x+i*(3*taille/4), y, taille, taille};
		SDL_RenderCopy(renderer, chiffres[(int) (nbr[i]-'0')], NULL, &rect);
	}
}

bool GameView::isInsideRectangle(int xcarte, int ycarte, int wcarte){
	return (sx >= xcarte && sx < xcarte+wcarte && sy >= ycarte);
}

bool GameView::isInsideCarre(int x, int y, int c) {
	return (sx >= x && sx < x+c && sy >= y && sy < y+c);
}

Atout GameView::iconeToAtout() { //0, P, C, T, Coeur, TA, SA, Passe
	int x = (int) (3 / 8.0 * wWindow);
	int y = hWindow - (int) (2.1 * hCarte);
	if (isInsideCarre( x+dec, y+dec, 3*wIcone/4)) {
		return Atout::Trefle;
	} else if (isInsideCarre( x+dec+wIcone, y+dec+wIcone, 3*wIcone/4)) {
		return Atout::Pique;
	} else if (isInsideCarre( x+dec+wIcone, y+dec, 3*wIcone/4)) {
		return Atout::Coeur;
	} else if (isInsideCarre( x+dec, y+dec+wIcone, 3*wIcone/4)) {
		return Atout::Carreau;
	} else if (isInsideCarre( x+dec+wIcone, y+dec/2+2*wIcone, 3*wIcone/4)) {
		return Atout::Sa;
	} else if (isInsideCarre( x+dec, y+dec/2+2*wIcone, 3*wIcone/4)) {
		return Atout::Ta;
	} else if (isInsideCarre( x+dec/2+wIcone/2, y+dec/2+3*wIcone, 3*wIcone/4) || isInsideCarre( x+dec/2+5*wIcone/4, y+dec/2+3*wIcone, 3*wIcone/4)) {
		return Atout::Passe;

	} else {
		return Atout::Rien;
	}
}

std::pair<Atout, bool> GameView::pair_icone(Atout atout) {
	Atout atout_clicked = GameView::iconeToAtout();
	return {atout, atout==atout_clicked};
}

void GameView::renderMenu(int x, int y, int &annonce_temp, int annonce_min) {
	int wIcone = (int) (hMenu / 6.0);
	SDL_Rect rect = {x, y, wMenu, hMenu};
	SDL_RenderCopy(renderer, menu, NULL, &rect);

	rect = {x+dec, y+dec, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Trefle)], NULL, &rect);
	rect = {x+2*dec+wIcone, y+dec, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Coeur)], NULL, &rect);
	rect = {x+dec, y+2*dec+wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Carreau)], NULL, &rect);
	rect = {x+2*dec+wIcone, y+2*dec+wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Pique)], NULL, &rect);
	rect = {x+2*dec+wIcone, y+5*dec/2+2*wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Sa)], NULL, &rect);
	rect = {x+dec, y+5*dec/2+2*wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Ta)], NULL, &rect);
	rect = {x+dec/2+wIcone/2, y+3*dec+3*wIcone, dec+2*wIcone, 5*wIcone/6};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Passe)], NULL, &rect);

	bool is_moins = isInsideCarre( x + hCarte - 70, y+ hCarte/3, 50);
	bool is_plus = isInsideCarre( x + hCarte + 140, y+ hCarte/3, 50);
	render_nombre(annonce_temp, x + hCarte, y + hCarte/3, 50);
	rect = {x + hCarte - 70, y+ hCarte/3, 50, 50};
	SDL_RenderCopy(renderer, moins[is_moins], NULL, &rect);
	rect = {x + hCarte + 140, y+ hCarte/3, 50, 50};
	SDL_RenderCopy(renderer, plus[is_plus], NULL, &rect);
	if (is_plus && mouse_click) {
		annonce_temp = std::min(160, annonce_temp+10);
	}
	if (is_moins && mouse_click) {
		annonce_temp = std::max(annonce_min, annonce_temp-10);
	}
}

void	GameView::addAnimation(Animation animation) {
	animations.push_back(animation);
}

void	GameView::updateAnimations(int deltaTime) {
	for (auto &animation : animations) {
		animation.update(deltaTime);
	}
	animations.erase(std::remove_if(animations.begin(), animations.end(), [](Animation &animation) { return animation.isEnded(); }), animations.end());
}

void	GameView::renderAnimations() {
	for (auto &animation : animations) {
		animation.render(renderer);
	}
}

void	GameView::startAnimations() {
	for (auto &animation : animations) {
		animation.start();
	}
}
/*
void GameView::renderTexte(std::string text, int x, int y, int taille, SDL_Color color) {
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("ressources/Autres/the_nature.ttf", taille);
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect position = { 0, 0, surfaceMessage->w, surfaceMessage->h };
	SDL_RenderCopy(renderer, message, NULL, &position);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
	TTF_CloseFont(font);
	TTF_Quit();
}*/

void GameView::render(Joueur you, Joueur who_deals, Paquet mypaquet, std::vector<int> taille_paquets, Timer timer, int &annonce_temp, int annonce_min) {
	int int_you = joueurToInt(you);
	clear(false);
	renderPaquet(mypaquet);
	renderRetournees(taille_paquets[(int_you+1)%4], taille_paquets[(int_you+2)%4], taille_paquets[(int_you+3)%4]);
	renderDealer(joueurToInt(who_deals)-int_you);
	if (annonce_min != -1) {
		renderMenu((int) (3 / 8.0 * wWindow), hWindow - (int) (2.1 * hCarte), annonce_temp, annonce_min);
	}
	renderAnimations();
	SDL_Delay(std::max(0, 1000/60 - timer.get_ticks()));
	updateAnimations(timer.get_ticks());
	timer.start();
	SDL_RenderPresent(renderer);
}