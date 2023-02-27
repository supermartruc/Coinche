#include "view.hpp"
#include <sstream>

void	GameView::init() {
	window = create_window();
	renderer = create_renderer(window);
	for (auto &couleur : couleurs) {
		for (auto &valeur : valeurs) {
			std::stringstream ss;
			ss << "ressources/" << couleur << "/" << valeur << ".png";
			SDL_Surface *surface = IMG_Load(ss.str().c_str());
			if (surface == nullptr) {
				std::cerr << "Error: " << SDL_GetError() << std::endl;
				exit(1);
			}
			textures[valeur][couleur] = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}
	}
}