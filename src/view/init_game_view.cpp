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

void GameView::init() {
    window = create_window();
    renderer = create_renderer(window);
    for (auto& couleur : couleurs) {
        for (auto& valeur : valeurs) {
            std::stringstream ss;
			ss << "ressources/" << couleur << "/" << valeur << ".png";
            loadTexture(ss.str(), renderer, textures[valeur][couleur]);
        }
    }
	std::string cV = "ressources/Autres/carte_dosV.png";
	std::string cH = "ressources/Autres/carte_dosH.png";
    std::string tapis = "ressources/Autres/tapis_vert.png";
    std::string jeton_str = "ressources/Autres/jeton1.png";
    loadTexture(cV, renderer, dos_carteV);
    loadTexture(cH, renderer, dos_carteH);
    loadTexture(tapis, renderer, fond);
    loadTexture(jeton_str, renderer, jeton);
    SDL_QueryTexture(textures[Valeur::As][Couleur::Coeur], nullptr, nullptr, &wCarte, &hCarte);
    wCarte *= 2;
    hCarte *= 2;
    SDL_GetWindowSize(window, &wWindow, &hWindow);
}
