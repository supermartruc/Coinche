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
		Animation(SDL_Texture *texture, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd, Carte carte_anim) : texture(texture), xStart(xStart), yStart(yStart), xEnd(xEnd), yEnd(yEnd), timeStart(timeStart), timeEnd(timeEnd), time(0), started(false), ended(false) {};
		~Animation() = default;
		void	init(SDL_Renderer *renderer, std::string path, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd);
		void	init(SDL_Texture *texture, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd, Carte carte_anim);
		void	start();
		void	render(SDL_Renderer *renderer);
		void	update(int deltaTime);
		bool	isEnded() const;

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
		Carte		carte_anim;
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
		int 			elevation;

	public:
		void	init();
		void	renderCarte(Carte carte, int x, int y);
		void	renderPaquet(Paquet paquet, int sx, int sy);
		void	renderDealer(int dist_trigo);
		void	renderDosV(int x, int y);
		void	renderDosH(int x, int y);
		void	renderRetournees(Paquet haut, Paquet droite, Paquet gauche);
		void	render();
		void	clear();
		int		getWCarte() const;
		int		getHCarte() const;
		bool 	isInsideRectangle(int sx, int sy, int xcarte, int ycarte, int wcarte);

		void	startAnimations();
		void	addAnimation(Animation animation);
		void	updateAnimations(int deltaTime);
		void	renderAnimations();
};

#endif