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
	int wy = hCarte + 0.9 * (hWindow - 2*hCarte);
	int wx = hCarte + 0.9 * (wWindow - 2*hCarte);
	int h = hCarte + 150 + 0.1 * (hWindow - 2*hCarte - 150);
	int x=0;
	int y=0;
	switch ((8+dist_trigo)%4) {
	case 0:
		x = wx; y = hWindow-h; break;
	case 1:
		x = h; y = wy; break;
	case 2:
		x = hWindow-wx; y = h; break;
	case 3:
		x = wWindow-h; y = hWindow-wy; break;
	default: 
		break;
	}
	SDL_Rect rect = {x, y, 90, 90};
	SDL_RenderCopy(renderer, jeton, NULL, &rect);
}
void	GameView::render() {
	SDL_RenderPresent(renderer);
}

void	GameView::clear() {
	//204021
	SDL_SetRenderDrawColor(renderer, 0x20, 0x40, 0x33, 0xFF);
	SDL_RenderClear(renderer);
	SDL_Rect rect = {0, 0, wWindow, hWindow};
	SDL_RenderCopy(renderer, fond, NULL, &rect);
}

void	GameView::renderPaquet(Paquet paquet, int sx, int sy) {
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
		if (isInsideRectangle(sx, sy, l, h, std::max(chev*wCarte,(float)wCarte*(j==copy_paquet.size()-1)))) {
			renderCarte(carte, l, h-elevation);
		}
		else{
			renderCarte(carte, l, h);
		}
		i++;
	}
}


void GameView::renderRetournees(Paquet gauche, Paquet haut, Paquet droite) {
	float temp = 4/5.0 * hWindow / ( (float) ((haut.size()+1)*wCarte) );
	float chev = (float) std::min((float) 4/5, (float) temp );
	int i = 0;
	int wStart = (wWindow - (chev*wCarte * haut.size())) / 2;
	for (auto &carte : haut) {
		renderDosV(wStart + i*chev*wCarte, 0);
		i++;
	}
	i = 0;
	int hStart = (hWindow - (chev*wCarte * gauche.size())) / 2;
	for (auto &carte : gauche) {
		renderDosH(0, hStart + i*chev*wCarte);
		i++;
	}
	i = 0;
	hStart = (hWindow - (chev*wCarte * droite.size())) / 2;
	for (auto &carte : droite) {
		renderDosH(wWindow-hCarte, hStart + i * chev *wCarte);
		i++;
	}
}

void GameView::renderMenu(int x, int y) {
	int wIcone = (int) (hMenu / 4.0);
	int dec = wMenu / 10.0;
	SDL_Rect rect = {x, y, wMenu, hMenu};
	SDL_RenderCopy(renderer, menu, NULL, &rect);
	rect = {x+dec, y+dec, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[Couleur::Trefle], NULL, &rect);
	rect = {x+2*dec+wIcone, y+dec, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[Couleur::Coeur], NULL, &rect);
	rect = {x+dec, y+2*dec+wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[Couleur::Carreau], NULL, &rect);
	rect = {x+2*dec+wIcone, y+2*dec+wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[Couleur::Pique], NULL, &rect);
}

bool GameView::isInsideRectangle(int sx, int sy, int xcarte, int ycarte, int wcarte){
	return (sx >= xcarte && sx < xcarte+wcarte && sy >= ycarte);
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