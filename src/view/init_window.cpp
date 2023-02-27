//use SDL2 to create a window
#include "view.hpp"
#include <SDL2/SDL.h>

SDL_Window	*create_window() {
	SDL_Window *window = SDL_CreateWindow("Coinche", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == NULL) {
		SDL_Log("Erreur lors de la creation de la fenetre: %s", SDL_GetError());
		return NULL;
	}
	return window;
}

SDL_Renderer	*create_renderer(SDL_Window *window) {
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_Log("Erreur lors de la creation du renderer: %s", SDL_GetError());
		return NULL;
	}
	return renderer;
}