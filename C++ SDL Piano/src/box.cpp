#include "SDL.h"
#include "position.h"

class box{
private:
	SDL_Rect shape;
public:

	SDL_Color color;

	void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); 
		SDL_RenderFillRect(renderer, &shape);
	}

	box() {}

	//constructor
	box(SDL_Color color_, int width_,  int height_, position location ) {
		color = color_;
		shape.h = height_;
		shape.w = width_;
		shape.x = location.posX;
		shape.y = location.posY;
	}

};