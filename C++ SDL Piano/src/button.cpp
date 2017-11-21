#include "box.h"
#include "position.h"

class button: private position {
private:
	box* shape;
	SDL_Color clickedColor;
	SDL_Color hoverColor;
	SDL_Color defaultColor;

	float hitBoxX;
	float hitBoxY;

	bool isHoverd(SDL_Renderer* renderer) {
		int x;
		int y; 
		SDL_GetMouseState(&x, &y);

		if (x > posX && x < posX + hitBoxX &&
			y > posY && y < posY + hitBoxY
			) {
			return true;
		}
		return false;
	}

public:

	bool isClicked(SDL_Renderer* rendererClickedOn) {
		
		SDL_PumpEvents();
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && isHoverd(rendererClickedOn)) {
			return true;
		}
		return false;
	}

	void draw(SDL_Renderer* thingToDrawTO) {
		SDL_Color setColor;

		if (isClicked(thingToDrawTO)) {
			setColor = clickedColor;
		}
		else if (isHoverd(thingToDrawTO)) {
			setColor = hoverColor;
		}
		else {
			setColor = defaultColor;
		}

		shape->color = setColor;
		shape->render(thingToDrawTO);
	}

	button() {}
	
	//constructor
	button(SDL_Color clickedColor_,	SDL_Color hoverColor_, SDL_Color defaultColor_, float buttonWidth, float buttonHeight, position location) {
		clickedColor = clickedColor_;
		hoverColor = hoverColor_;
		defaultColor = defaultColor_;
		shape = new box(defaultColor_,buttonWidth,buttonHeight,location);

		hitBoxX = buttonWidth;
		hitBoxY = buttonHeight;

		posX = location.posX;
		posY = location.posY;
	}

	~button() {
		//delete shape;
	}
};