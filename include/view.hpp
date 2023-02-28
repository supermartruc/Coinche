#ifndef VIEW_HPP

# define VIEW_HPP

#include "carte.hpp"

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window		*create_window();
SDL_Renderer	*create_renderer(SDL_Window *window);

class GameView {
	private:
		std::map<Valeur, std::map<Couleur, SDL_Texture*>>	textures;
		SDL_Window		*window;
		SDL_Renderer	*renderer;
		int				wCarte;
		int				hCarte;
		int				wWindow;
		int				hWindow;

	public:
		void	init();
		void	renderCarte(Carte carte, int x, int y);
		void	renderPaquet(Paquet paquet);
		void	render();
		void	clear();
};

#endif