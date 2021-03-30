#pragma once


#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "SDL.h"
#include <string>


class Toolbar {
private:
	//std::string toolName;	//The name of the current tool
	//int currentToolIndex;	//The Index of the current tool

	SDL_Window* window;	//The window itself
	SDL_Renderer* renderer;	//What will render to this window

	SDL_Rect renderArea;	//What area this will render too
public:
	Toolbar(SDL_Window* window, SDL_Renderer* render) {

	}


};

#endif
