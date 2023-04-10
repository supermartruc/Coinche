#ifndef VIEW_HPP

# define VIEW_HPP

#include "carte.hpp"
#include "timer.hpp"

#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window		*create_window();
SDL_Renderer	*create_renderer(SDL_Window *window);

class GameView {
	private:
		std::map<Valeur, std::map<Couleur, SDL_Texture*>>	textures;
		std::map<int, SDL_Texture*>							chiffres;			
		std::map<std::pair<Atout,bool>, SDL_Texture*>		icones;
		std::map<bool, SDL_Texture*> 						plus;
		std::map<bool, SDL_Texture*> 						moins;
		SDL_Window		*window;
		SDL_Texture* 	dos_carteV;
		SDL_Texture* 	dos_carteH;
		SDL_Texture* 	fond;
		SDL_Texture* 	jeton;
		SDL_Texture* 	menu;
		SDL_Texture* 	encadrement;
		SDL_Texture* 	dots;
		SDL_Texture* 	Hline;
		SDL_Texture* 	Vline;
		SDL_Texture* 	nous;
		SDL_Texture* 	eux;
		SDL_Texture* 	points_faits;
		SDL_Texture* 	points_marques;
		SDL_Texture* 	total;
		SDL_Texture* 	belote;
		SDL_Texture* 	rebelote;
		SDL_Event		event;
		int 			wMenu;
		int 			hMenu;
		int 			wIcone;
		int 			dec; // decalage des icones sur le menu
		int 			elevation;

	public:
		SDL_Renderer	*renderer;
		int				wCarte;
		int				hCarte;
		int				wWindow;
		int				hWindow;
		bool			mouse_pressed;
		bool			mouse_click;
		int 			sx;
		int 			sy;
		void	init();
		void	renderCarte(Carte carte, int x, int y);
		void	renderPaquet(Paquet paquet);
		void	renderDealer(int dist_trigo);
		void	renderDosV(int x, int y);
		void	renderDosH(int x, int y);
		void	renderRetournees(int gauche, int haut, int droite);
		void	renderMenu(int x, int y, int &annonce_temp, int annonce_min);
		void 	render_dots(int x, int y);
		void	render_une_annonce(int x, int y, int pointEnchere, Atout atoutEnchere);
		void	renderAnnonces(Joueur you, Joueur who_speaks, std::vector<Enchere> all_encheres);
		Atout	iconeToAtout();
		Carte 	clicToCarte(Paquet mypaquet_trie);
		void	render_nombre(int nombre, int x, int y, int taille);
		void	renderBelote(Joueur you, Joueur bel, int beloteur);
		std::pair <Atout, bool> pair_icone(Atout atout);
		//void	renderTexte(std::string text, int x, int y, int taille, SDL_Color color);
		void	clear(bool renderPresent = false);
		bool	handleEvents();
		int		getWCarte() const;
		int		getHCarte() const;
		bool 	isInsideRectangle( int xcarte, int ycarte, int wcarte);
		bool 	isInsideCarre( int x, int y, int c);
		void 	renderGlobalPoints(int new_nous, int new_eux, int nous_fait, int eux_fait, int nous_manche, int eux_manche);
		void 	renderCartesPli(Joueur you, Joueur who_starts, Paquet pli_en_cours);
		void 	renderEnchere(Joueur you, Joueur who_deals, Paquet mypaquet, int& annonce_temp, int annonce_min, Joueur who_speaks, std::vector<Enchere> all_encheres);
		void 	renderManche(Joueur you, Joueur who_deals, Paquet mypaquet, std::vector<int> taille_paquets, Enchere current_enchere, Joueur who_starts, Paquet pli_en_cours, int belote=0, Joueur beloteur=Joueur::Sud);
};

#endif