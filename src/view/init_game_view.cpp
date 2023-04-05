#include "view.hpp"
#include <sstream>

Uint32 get_pixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16*)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32*)p;

    default:
        return 0;
    }
}

void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16*)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xFF;
            p[1] = (pixel >> 8) & 0xFF;
            p[2] = pixel & 0xFF;
        }
        else
        {
            p[0] = pixel & 0xFF;
            p[1] = (pixel >> 8) & 0xFF;
            p[2] = (pixel >> 16) & 0xFF;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;

    default:
        break;
    }
}

void gaussian_blur(SDL_Surface* surface, int radius) {
    SDL_Surface* tmp_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_BlitSurface(surface, NULL, tmp_surface, NULL);

    int passes = radius / 2;
    if (passes < 1) passes = 1;

    for (int i = 0; i < passes; i++)
    {
        int this_radius = (i + 1) * 2;

        float* kernel = new float[this_radius * 2 + 1];
        float sigma = (float)this_radius / 3.0f;
        float two_sigma_squared = 2.0f * sigma * sigma;
        float sqrt_two_pi_sigma = sqrtf(2.0f * M_PI * sigma);
        float kernel_sum = 0.0f;
        for (int j = -this_radius; j <= this_radius; j++)
        {
            float distance_squared = (float)(j * j);
            kernel[j + this_radius] = expf(-distance_squared / two_sigma_squared) / sqrt_two_pi_sigma;
            kernel_sum += kernel[j + this_radius];
        }

        for (int j = 0; j <= this_radius * 2; j++)
        {
            kernel[j] /= kernel_sum;
        }

        for (int y = 0; y < surface->h; y++)
        {
            for (int x = 0; x < surface->w; x++)
            {
                Uint8 r = 0, g = 0, b = 0, a = 0;

                for (int k = -this_radius; k <= this_radius; k++)
                {
                    int xx = x + k;
                    if (xx < 0) xx = 0;
                    if (xx >= surface->w) xx = surface->w - 1;

                    Uint32 pixel = get_pixel(tmp_surface, xx, y);
                    r += (Uint8)((pixel >> 24) & 0xFF) * kernel[k + this_radius];
                    g += (Uint8)((pixel >> 16) & 0xFF) * kernel[k + this_radius];
                    b += (Uint8)((pixel >> 8) & 0xFF) * kernel[k + this_radius];
                    a += (Uint8)(pixel & 0xFF) * kernel[k + this_radius];
                }

                Uint32 pixel = SDL_MapRGBA(surface->format, r, g, b, a);
                put_pixel(surface, x, y, pixel);
            }
        }

        delete[] kernel;
    }

    SDL_FreeSurface(tmp_surface);
}

void loadTexture(std::string ss, SDL_Renderer* renderer, SDL_Texture*& texture) {
    SDL_Surface* surface = IMG_Load(ss.c_str());
    if (surface == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void loadTexture_dim(std::string ss, SDL_Renderer* renderer, SDL_Texture*& texture, int width, int height, bool transp, int blur = 0) {
    SDL_Surface* surface = IMG_Load(ss.c_str());
    if (surface == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Surface* image_surface = SDL_CreateRGBSurface(0, width, height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);    
    if (image_surface == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Rect rect = { 0, 0, width, height };
    Uint32 color = SDL_MapRGBA(image_surface->format, 0, 0, 0, 0);
    SDL_FillRect(image_surface, &rect, color);
    SDL_SetSurfaceAlphaMod(surface, transp ? 255 : 150);
    SDL_Rect dest_rect = { 0, 0, width, height };
    SDL_BlitScaled(surface, NULL, image_surface, &dest_rect);
    if (blur>0) {
        gaussian_blur(image_surface, blur);
    }
    texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(surface);
}

void GameView::init() {
    window = create_window();
    renderer = create_renderer(window);
    SDL_GetWindowSize(window, &wWindow, &hWindow);
    wCarte = 150*1920 / wWindow;
    hCarte = 240*1920 / wWindow;
    wMenu = 480*1920 / wWindow;
    hMenu = 225*1920 / wWindow;
    elevation = 0.1 * hCarte;
    wIcone = (int) (hMenu / 5.5);
	dec = (int) ((hMenu-4*wIcone)/5);
    for (auto& atout : atouts) {
        std::stringstream ss;
        ss << "ressources/Autres/" << "icone_" << atout << ".png";
        loadTexture_dim(ss.str(), renderer, icones[{atout,true}], 200, 200, true);
        loadTexture_dim(ss.str(), renderer, icones[{atout,false}], 200, 200, false);
    }
    for (auto& couleur : couleurs) {
        for (auto& valeur : valeurs) {
            std::stringstream ss;
			ss << "ressources/" << couleur << "/" << valeur << ".png";
            loadTexture_dim(ss.str(), renderer, textures[valeur][couleur], 3*wCarte, 3*hCarte, true);
        }
    }
    for (int i=0; i<10; i++) {
        std::stringstream ss;
		ss << "ressources/Chiffres/" << (char) (i+'0') << ".png";
        loadTexture(ss.str(), renderer, chiffres[i]);
    }
    loadTexture_dim("ressources/Autres/menu.png", renderer, menu, wMenu, hMenu, true);
    loadTexture_dim("ressources/Chiffres/plus.png", renderer, plus[false], 60, 60, false);
    loadTexture_dim("ressources/Chiffres/moins.png", renderer, moins[false], 60, 60, false);
    loadTexture_dim("ressources/Chiffres/plus.png", renderer, plus[true], 60, 60, true);
    loadTexture_dim("ressources/Chiffres/moins.png", renderer, moins[true], 60, 60, true);
    loadTexture("ressources/Autres/encadrement.png", renderer, encadrement);
    loadTexture("ressources/Autres/dots.png", renderer, dots);
    loadTexture("ressources/Autres/carte_dosV.png", renderer, dos_carteV);
    loadTexture("ressources/Autres/carte_dosH.png", renderer, dos_carteH);
    loadTexture("ressources/Autres/tapis_vert.png", renderer, fond);
    loadTexture("ressources/Autres/Vline.png", renderer, Vline);
    loadTexture("ressources/Autres/Hline.png", renderer, Hline);
    loadTexture_dim("ressources/Autres/jeton1.png", renderer, jeton, 200, 200, true, 3);
    loadTexture("ressources/Autres/nous.png", renderer, nous);
    loadTexture("ressources/Autres/eux.png", renderer, eux);
    loadTexture("ressources/Autres/points_faits.png", renderer, points_faits);
    loadTexture("ressources/Autres/points_marques.png", renderer, points_marques);
    loadTexture("ressources/Autres/total.png", renderer, total);
}
