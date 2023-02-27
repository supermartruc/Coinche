#ifndef VIEW_HPP

# define VIEW_HPP

#include "carte.hpp"

#include <SDL2/SDL.h>

SDL_Window		*create_window();
SDL_Renderer	*create_renderer(SDL_Window *window);

#endif