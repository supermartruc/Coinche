#include "view.hpp"

void	GameView::renderCarte(Carte carte, int x, int y) {
	auto [valeur, couleur] = carte;
	SDL_Rect rect = {x, y, wCarte, hCarte};
	SDL_RenderCopy(renderer, textures[valeur][couleur], NULL, &rect);
}

void	GameView::render() {
	SDL_RenderPresent(renderer);
}

void	GameView::clear() {
	//204021
	SDL_SetRenderDrawColor(renderer, 0x20, 0x40, 0x33, 0xFF);
	SDL_RenderClear(renderer);
}

void	GameView::renderPaquet(Paquet paquet) {
	int i = 0;
	int wStart = (wWindow - (wCarte * paquet.size())) / 2;
	for (auto &carte : paquet) {
		renderCarte(carte, wStart + (i * wCarte), 0);
		i++;
	}
}