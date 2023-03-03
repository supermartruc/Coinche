#ifndef VIEW_HPP

# define VIEW_HPP

#include "carte.hpp"

#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window		*create_window();
SDL_Renderer	*create_renderer(SDL_Window *window);

class Animation {
	public:
		Animation() = default;
		Animation(SDL_Texture *texture, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd) : texture(texture), xStart(xStart), yStart(yStart), xEnd(xEnd), yEnd(yEnd), timeStart(timeStart), timeEnd(timeEnd), time(0), started(false), ended(false) {};
		~Animation() = default;
		void	init(SDL_Renderer *renderer, std::string path, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd);
		void	init(SDL_Texture *texture, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd);
		void	start();
		void	render(SDL_Renderer *renderer);
		void	update(int deltaTime);
		bool	isEnded() const;
	private:
		SDL_Texture	*texture;
		int			xStart;
		int			yStart;
		int			xEnd;
		int			yEnd;
		int			timeStart;
		int			timeEnd;
		int			time;
		bool		started;
		bool		ended;
};

class GameView {
	private:
		std::map<Valeur, std::map<Couleur, SDL_Texture*>>	textures;
		std::vector<Animation>								animations;
		SDL_Window		*window;
		SDL_Renderer	*renderer;
		SDL_Texture* 	dos_carteV;
		SDL_Texture* 	dos_carteH;
		SDL_Texture* 	fond;
		SDL_Texture* 	jeton;
		int				wCarte;
		int				hCarte;
		int				wWindow;
		int				hWindow;

	public:
		void	init();
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

		void	startAnimations();
		void	addAnimation(Animation animation);
		void	updateAnimations(int deltaTime);
		void	renderAnimations();
};

#endif