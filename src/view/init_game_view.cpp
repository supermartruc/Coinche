#include "view.hpp"
#include <sstream>

void loadTexture(std::string ss, SDL_Renderer* renderer, SDL_Texture*& texture) {
    SDL_Surface* surface = IMG_Load(ss.c_str());
    if (surface == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void loadTexture_dim(std::string ss, SDL_Renderer* renderer, SDL_Texture*& texture, int width, int height) {
    SDL_Surface* surface = IMG_Load(ss.c_str());
    if (surface == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Surface* image_surface = SDL_CreateRGBSurface(0, width, height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);    if (image_surface == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Rect rect = { 0, 0, width, height };
    Uint32 color = SDL_MapRGBA(image_surface->format, 0, 0, 0, 0);
    SDL_FillRect(image_surface, &rect, color);
    SDL_Rect dest_rect = { 0, 0, width, height };
    SDL_BlitScaled(surface, NULL, image_surface, &dest_rect);
    texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(surface);
}

void GameView::init() {
    window = create_window();
    renderer = create_renderer(window);
    wCarte = 150;
    hCarte = 240;
    wMenu = hCarte;
    hMenu = wCarte;
    elevation = 0.1 * hCarte;
    wIcone = (int) (hMenu / 4.0);
	dec = (int) (wMenu / 10.0);
    loadTexture_dim("ressources/Autres/menu.png", renderer, menu, wMenu, hMenu);
    for (auto& couleur : couleurs) {
        std::stringstream ss;
        ss << "ressources/Autres/" << "icone_" << couleur << ".png";
        loadTexture_dim(ss.str(), renderer, icones[couleur], 200, 200);
        for (auto& valeur : valeurs) {
            std::stringstream ss;
			ss << "ressources/" << couleur << "/" << valeur << ".png";
            loadTexture_dim(ss.str(), renderer, textures[valeur][couleur], wCarte, hCarte);
        }
    }
	std::string cV = "ressources/Trefle/Dos.png"; //carte_dosV.png";
	std::string cH = "ressources/Autres/carte_dosH.png";
    std::string tapis = "ressources/Autres/tapis_vert.png";
    std::string jeton_str = "ressources/Autres/jeton1.png";
    loadTexture(cV, renderer, dos_carteV);
    loadTexture(cH, renderer, dos_carteH);
    loadTexture(tapis, renderer, fond);
    loadTexture_dim(jeton_str, renderer, jeton, 90, 90);
    addAnimation({dos_carteV, 0, 0, 500, 500, 100, 5000, Carte{Valeur::Valet, Couleur::Pique}});
    // SDL_QueryTexture(textures[Valeur::As][Couleur::Coeur], nullptr, nullptr, &wCarte, &hCarte);
    SDL_GetWindowSize(window, &wWindow, &hWindow);
    addAnimation({jeton, 0, 0, wWindow, 1000, 1000, 5000, Carte{Valeur::Valet, Couleur::Pique}});
}
