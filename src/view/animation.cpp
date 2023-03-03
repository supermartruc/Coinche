#include "view.hpp"

void	Animation::init(SDL_Renderer *renderer, std::string path, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd, Carte carte_anim) {
	SDL_Surface	*tmp = IMG_Load(path.c_str());
	if (tmp == nullptr) {
		std::cerr << "Error: " << SDL_GetError() << std::endl;
		return;
	}
	this->texture = SDL_CreateTextureFromSurface(renderer, tmp);
	if (this->texture == nullptr) {
		std::cerr << "Error: " << SDL_GetError() << std::endl;
		return;
	}
	this->xStart = xStart;
	this->yStart = yStart;
	this->xEnd = xEnd;
	this->yEnd = yEnd;
	this->timeStart = timeStart;
	this->timeEnd = timeEnd;
	this->time = 0;
	this->carte_anim = carte_anim;
}

void	Animation::init(SDL_Texture *texture, int xStart, int yStart, int xEnd, int yEnd, int timeStart, int timeEnd) {
	this->texture = texture;
	this->xStart = xStart;
	this->yStart = yStart;
	this->xEnd = xEnd;
	this->yEnd = yEnd;
	this->timeStart = timeStart;
	this->timeEnd = timeEnd;
	this->time = 0;
	this->carte_anim = NULL;
}

void	Animation::start() {
	this->time = 0;
	this->started = true;
}

void	Animation::render(SDL_Renderer *renderer) {
	SDL_Rect	dst;
	dst.x = xStart + (xEnd - xStart) * time / (timeEnd - timeStart);
	dst.y = yStart + (yEnd - yStart) * time / (timeEnd - timeStart);
	SDL_QueryTexture(this->texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, this->texture, NULL, &dst);
}

void	Animation::update(int deltaTime) {
	if (!this->started || this->ended) {
		return;
	}
	this->time += deltaTime;
	if (this->time > this->timeEnd) {
		this->time = this->timeEnd;
		this->ended = true;
	}
}

bool	Animation::isEnded() const {
	return this->ended;
}
