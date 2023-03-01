#include "view.hpp"
#include <algorithm>

void 	GameView::DrawDisk(int x, int y, int radius) {
	SDL_SetRenderDrawColor(renderer, 235, 0, 0, 235);
	int sum_square = 1;
	for (int i=x-radius+1; i<x+radius; i++) {
		for (int j=y-radius+1; j<y+radius; j++) {
			sum_square = (i - x) * (i - x) + (j - y) * (j - y);
			if ( sum_square < radius * radius ) {
				if ( sum_square < radius * radius * 0.5 ) {
					SDL_SetRenderDrawColor(renderer, 235, 20, 20, 235);
				} else {
					SDL_SetRenderDrawColor(renderer, 235, 20, 20, ((float) (radius*radius-sum_square)/(radius*radius*0.5)) * 235);
				}
				SDL_RenderDrawPoint(renderer, i, j);
			}
		}
	}
}

void	GameView::renderCarte(Carte carte, int x, int y) {
	auto [valeur, couleur] = carte;
	SDL_Rect rect = {x, y, wCarte, hCarte};
	SDL_RenderCopy(renderer, textures[valeur][couleur], NULL, &rect);
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
	SDL_Rect rect = {x, y, 150, 150};
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

void	GameView::renderPaquet(Paquet paquet) {
	int i = 0;
	float temp = hWindow / ( (float) ((paquet.size()+0.5)*wCarte) );
	float chev = (float) std::min((float) 1, (float) (temp * 4/5.0) );
	int wStart = (wWindow - (chev*wCarte * paquet.size())) / 2;
	Paquet copy_paquet = tri_paquet_affichage(paquet, Atout::Sa);
	for (auto &carte : copy_paquet) {
		renderCarte(carte, wStart + (i * chev * wCarte), hWindow-hCarte);
		i++;
	}
}

void GameView::renderRetournees(Paquet gauche, Paquet haut, Paquet droite) {
	float temp = 4/5.0 * hWindow / ( (float) ((haut.size()+1)*wCarte) );
	float chev = (float) std::min((float) 4/5, (float) temp );
	std::cout << "chev = " << chev << "\n";
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