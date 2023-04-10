#include "view.hpp"
#include <algorithm>
#include <vector>

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
	SDL_Rect rect = {x, y, 90, 90};
	SDL_RenderCopy(renderer, jeton, NULL, &rect);
}

void	GameView::clear(bool renderPresent) {
	SDL_SetRenderDrawColor(renderer, 0x20, 0x40, 0x33, 0xFF);
	SDL_RenderClear(renderer);
	SDL_Rect rect = {0, 0, wWindow, hWindow};
	SDL_RenderCopy(renderer, fond, NULL, &rect);
	if (renderPresent) {
		SDL_RenderPresent(renderer);
	}
}

bool	GameView::handleEvents() {
	mouse_click = false;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
		if (event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					return false;
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
	return true;
}

void	GameView::renderPaquet(Paquet paquet) {
	int i = 0;
	float temp = hWindow / ( (float) ((paquet.size()+0.5)*wCarte) );
	float chev = (float) std::min((float) 1, (float) (temp * 4/5.0) );
	int wStart = (wWindow - (chev*wCarte * paquet.size())) / 2;
	Paquet copy_paquet = {}; for (Carte carte : paquet){copy_paquet.push_back(carte);}
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
	int shift = nbr.size() < 3 ? 10 : -10;
	SDL_Rect rect;
	for (int i=0; i<nbr.size(); i++) {
		rect = {x+shift+i*(3*taille/4), y, taille, taille};
		SDL_RenderCopy(renderer, chiffres[(int) (nbr[i]-'0')], NULL, &rect);
	}
}

void 	GameView::renderBelote(Joueur you, Joueur bel, bool re) {
	int player = (joueurToInt(bel)-joueurToInt(you)+4)%4;
	int x, y;
	if (player%2 == 0) {
		x = wWindow/2 + 2*(player==2)*wCarte - 2*(player==0)*wCarte - wCarte/2;
		y = player==0 ? hWindow - (hCarte + elevation + 3*wCarte/5) : hCarte + elevation + wCarte/5;
	} else {
		x = player == 3 ? hCarte + elevation/2 : wWindow - (hCarte + elevation + wCarte);
		y = player == 1 ? 3*hWindow/4 : hWindow/4;
	}
	SDL_Rect rect = {x-10, y, wCarte+30, 2*wCarte/5};
	SDL_RenderCopy(renderer, encadrement, NULL, &rect);
	rect = {x, y, wCarte+10, 2*wCarte/5};
	SDL_RenderCopy(renderer, re ? rebelote : belote, NULL, &rect);
}

bool GameView::isInsideRectangle(int xcarte, int ycarte, int wcarte){
	return (sx >= xcarte && sx < xcarte+wcarte && sy >= ycarte);
}

bool GameView::isInsideCarre(int x, int y, int c) {
	return (sx >= x && sx < x+c && sy >= y && sy < y+c);
}

Atout GameView::iconeToAtout() { //0, P, C, T, Coeur, TA, SA, Passe
	int x = dec + (int) (3 / 8.0 * wWindow);
	int y = hWindow - (int) (elevation+hCarte+hMenu);
	if (isInsideCarre( x+dec, y+dec, wIcone)) {
		return Atout::Sa;
	} else if (isInsideCarre( x+3*dec+wIcone, y+dec, wIcone)) {
		return Atout::Ta;
	} else if (isInsideCarre( x+dec, y+2*dec+wIcone, wIcone)) {
		return Atout::Carreau;
	} else if (isInsideCarre( x+3*dec+wIcone, y+2*dec+wIcone, wIcone)) {
		return Atout::Pique;
	} else if (isInsideCarre( x+3*dec+wIcone, y+3*dec+2*wIcone, wIcone)) {
		return Atout::Coeur;
	} else if (isInsideCarre( x+dec, y+3*dec+2*wIcone, wIcone)) {
		return Atout::Trefle;
	} else if (isInsideCarre( x+dec, y+4*dec+3*wIcone, wIcone) || isInsideCarre( x+dec+wIcone, y+4*dec+3*wIcone, wIcone)) {
		return Atout::Passe;

	} else {
		return Atout::Rien;
	}
}

Carte GameView::clicToCarte(Paquet mypaquet_trie) { // il faudrait que les paquets soient initialisés triés
	Carte select= {Valeur::Rien, Couleur::Pique};
	int i = 0;
	float temp = hWindow / ( (float) ((mypaquet_trie.size()+0.5)*wCarte) );
	float chev = (float) std::min((float) 1, (float) (temp * 4/5.0) );
	int wStart = (wWindow - (chev*wCarte * mypaquet_trie.size())) / 2;
	Carte carte;
	for (int j=0; j < mypaquet_trie.size(); j++) {
		carte = mypaquet_trie[j];
		int l = wStart + (i * chev * wCarte);
		int h = hWindow-hCarte;
		if (isInsideRectangle( l, h, std::max(chev*wCarte,(float)wCarte*(j==mypaquet_trie.size()-1)))) {
			return carte;
		}
		else{
			renderCarte(carte, l, h);
		}
		i++;
	}
	return select;
}

std::pair<Atout, bool> GameView::pair_icone(Atout atout) {
	Atout atout_clicked = GameView::iconeToAtout();
	return {atout, atout==atout_clicked};
}

void GameView::renderMenu(int x, int y, int &annonce_temp, int annonce_min) {
	SDL_Rect rect = {x, y, wMenu, hMenu};
	SDL_RenderCopy(renderer, menu, NULL, &rect);
	render_nombre(0, x, y, 5);
	x = x + dec;
	rect = {x+dec, y+dec, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Sa)], NULL, &rect);
	rect = {x+3*dec+wIcone, y+dec, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Ta)], NULL, &rect);
	rect = {x+dec, y+2*dec+wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Carreau)], NULL, &rect);
	rect = {x+3*dec+wIcone, y+2*dec+wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Pique)], NULL, &rect);
	rect = {x+3*dec+wIcone, y+3*dec+2*wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Coeur)], NULL, &rect);
	rect = {x+dec, y+3*dec+2*wIcone, wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Trefle)], NULL, &rect);
	rect = {x+2*dec, y+4*dec+3*wIcone, 2*wIcone, wIcone};
	SDL_RenderCopy(renderer, icones[pair_icone(Atout::Passe)], NULL, &rect);

	bool is_moins = isInsideCarre( x + hCarte - 70, y+ hCarte/3, 50);
	bool is_plus = isInsideCarre( x + hCarte + 140, y+ hCarte/3, 50);
	render_nombre(annonce_temp, x + hCarte, y + hCarte/3, 50);
	rect = {x + hCarte - 70, y+ hCarte/3, 50, 50};
	SDL_RenderCopy(renderer, moins[is_moins], NULL, &rect);
	rect = {x + hCarte + 140, y+ hCarte/3, 50, 50};
	SDL_RenderCopy(renderer, plus[is_plus], NULL, &rect);
	if (is_plus && mouse_click) {
		if (annonce_temp >= 180) {
			annonce_temp = 252;
		} else {
			annonce_temp = std::min(180, annonce_temp+10);
		}
	}
	if (is_moins && mouse_click) {
		if (annonce_temp == 252) {
			annonce_temp = std::max(180, annonce_min);
		} else {
			annonce_temp = std::max(annonce_min, annonce_temp-10);
		}
	}
}

void GameView::render_dots(int x, int y) {
	SDL_Rect rect = {x+12, y+5, 9*wCarte/10, wCarte/4};
	SDL_RenderCopy(renderer, dots, NULL, &rect);
	rect = {x, y-5, wCarte+5, 3*wCarte/8};
	SDL_RenderCopy(renderer, encadrement, NULL, &rect);
}

void GameView::render_une_annonce(int x, int y, int pointEnchere, Atout atoutEnchere) {
	SDL_Rect rect;
	if (atoutEnchere == Atout::Rien) {
		return;
	} else if (atoutEnchere == Atout::Passe) {
		rect = {x, y-5, wCarte+5, 3*wCarte/8};
		SDL_RenderCopy(renderer, icones[{Atout::Passe, true}], NULL, &rect);
	} else {
		if (atoutEnchere != Atout::Rien) {
			rect = {x, y-5, wCarte+5, 2*wCarte/5};
			SDL_RenderCopy(renderer, encadrement, NULL, &rect);
			render_nombre(pointEnchere, x+10, y+5, wCarte/4);
		}
		int ch2 = (pointEnchere < 100) ? 10 : 0;
		rect = {x+wCarte-50-ch2, y+5, wCarte/4, wCarte/4};
		SDL_RenderCopy(renderer, icones[{atoutEnchere,false}], NULL, &rect);
	}
}

void GameView::renderAnnonces(Joueur you, Joueur who_speaks, std::vector<Enchere> all_encheres) {
	int x, y;
	int player;
	for (auto &t : all_encheres) {
		player = joueurToInt(std::get<0>(t))-joueurToInt(you)+4;
		if (player%2 == 0) {
			x = wWindow/2 + 2*(player%4==0)*wCarte - 2*(player%4!=0)*wCarte - wCarte/2;
			y = player%4==0 ? hWindow - (hCarte + elevation/2 + 50) : hCarte + elevation/2;
		} else {
			x = player%4 == 1 ? hCarte + elevation/2 : wWindow - (hCarte + elevation + wCarte);
			y = hWindow/2;
		}
		if (std::get<0>(t) == who_speaks) { 
			render_dots(x, y);
		} else {
			render_une_annonce(x, y, std::get<1>(t), std::get<2>(t));
		}
	}
}

void GameView::renderCartesPli(Joueur you, Joueur who_starts, Paquet pli_en_cours) {
	int esp = 5; // expacement entre le cente et une carte placée
	int p = (joueurToInt(who_starts)-joueurToInt(you)+4)%4;
	for (auto& carte : pli_en_cours) {
		renderCarte(carte, wWindow/2-wCarte/2 + (p==3)*(wCarte+esp) - (p==1)*(wCarte+esp), hWindow/2-hCarte/2 + (p==0 ? 1 : 0 )*(hCarte/2+esp) - (p==2 ? 1 : 0)*(hCarte/2+esp));
		p = (1+p)%4;
	}
}

void GameView::renderGlobalPoints(int new_nous, int new_eux, int nous_fait, int eux_fait, int nous_manche, int eux_manche) {
	clear();
	int xg = 3*wWindow/8;
	int xd = 5*wWindow/8;
	int y0 = hWindow/8;
	int marge = wWindow/8;
	// lignes
	SDL_Rect rect = {wWindow/2, y0, 10, 5*hWindow/8};
	SDL_RenderCopy(renderer, Vline, NULL, &rect);
	rect = {wWindow/4, y0+hWindow/8, wWindow/2, 10};	
	SDL_RenderCopy(renderer, Hline, NULL, &rect);
	// nous eux
	y0 += hWindow/16;
	rect = {xg-20, y0, 80, 30};	
	SDL_RenderCopy(renderer, nous, NULL, &rect);
	rect = {xd-20, y0, 60, 30};	
	SDL_RenderCopy(renderer, eux, NULL, &rect);
	
	y0 += hWindow / 8;
	rect = {marge, y0, 50, 20};	
	SDL_RenderCopy(renderer, total, NULL, &rect);
	render_nombre(new_nous-nous_manche, xg, y0, 20);
	render_nombre(new_eux-eux_manche, xd, y0, 20);

	y0 += hWindow / 8;
	rect = {marge, y0, 120, 20};	
	SDL_RenderCopy(renderer, points_faits, NULL, &rect);
	render_nombre(nous_fait, xg, y0, 20);
	render_nombre(eux_fait, xd, y0, 20);

	y0 += hWindow / 8;
	rect = {marge, y0, 170, 25};	
	SDL_RenderCopy(renderer, points_marques, NULL, &rect);
	render_nombre(nous_manche, xg, y0, 20);
	render_nombre(eux_manche, xd, y0, 20);

	y0 += hWindow / 8;
	rect = {marge, y0, 50, 20};	
	SDL_RenderCopy(renderer, total, NULL, &rect);
	render_nombre(new_nous, xg, y0, 20);
	render_nombre(new_eux, xd, y0, 20);
	SDL_RenderPresent(renderer);
}

void GameView::renderEnchere(Joueur you, Joueur who_deals, Paquet mypaquet, int& annonce_temp, int annonce_min, Joueur who_speaks, std::vector<Enchere> all_encheres) {
	clear();
	renderPaquet(mypaquet);
	renderRetournees(8, 8, 8);
	renderDealer(joueurToInt(who_deals)-joueurToInt(you));
	renderAnnonces(you, who_speaks, all_encheres);
	if (you == who_speaks) {
		renderMenu((int) (3 / 8.0 * wWindow), hWindow - (int) (elevation+hCarte+hMenu), annonce_temp, annonce_min);
	}
	SDL_RenderPresent(renderer);
}

void GameView::renderManche(Joueur you, Joueur who_deals, Paquet mypaquet, std::vector<int> taille_paquets, Enchere current_enchere, Joueur who_starts, Paquet pli_en_cours) {
	int int_you = joueurToInt(you);
	clear();
	renderPaquet(mypaquet);
	renderRetournees(taille_paquets[(int_you+1)%4], taille_paquets[(int_you+2)%4], taille_paquets[(int_you+3)%4]);
	renderDealer(joueurToInt(who_deals)-int_you);
	renderAnnonces(you, intToJoueur(1 + joueurToInt(std::get<0>(current_enchere))%4), {current_enchere});
	renderCartesPli(you, who_starts, pli_en_cours);
	renderBelote(you, you, true); // c'est faux le 2e joeur doit être celui qui annonce la belote, true si rebelote
	/*if (belote) {
		renderBelote(you, you, false); // c'est faux le 2e joeur doit être celui qui annonce la belote, true si rebelote
	}*/
	SDL_RenderPresent(renderer);
}