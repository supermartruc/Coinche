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
		SDL_Texture* dos_carteV;
		SDL_Texture* dos_carteH;
		SDL_Texture* fond;
		SDL_Texture* jeton;
		int				wCarte;
		int				hCarte;
		int				wWindow;
		int				hWindow;

	public:
		void	init();
		void 	DrawDisk(int x, int y, int radius);
		void	renderCarte(Carte carte, int x, int y);
		void	renderPaquet(Paquet paquet);
		void	renderDealer(int dist_trigo);
		void	renderDosV(int x, int y);
		void	renderDosH(int x, int y);
		void	renderRetournees(Paquet haut, Paquet droite, Paquet gauche);
		void	render();
		void	clear();
		int		getWCarte() const;
		int		getHCarte() const;
};

#endif