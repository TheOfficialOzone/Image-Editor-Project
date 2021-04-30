#pragma once


#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "SDL.h"
#include "Camera/Camera.h"
#include <string>
#include <iostream>


class Toolbar {
private:
	//std::string toolName;	//The name of the current tool
	//int currentToolIndex;	//The Index of the current tool
	SDL_Renderer* renderTo;	//What will render to this window

	ViewPort renderArea;	//What area this will render too
	SDL_Rect viewArea;	
public:
	Toolbar(SDL_Renderer* renderer) {
		this->renderTo = renderer;
	}

	///Gets where the Toolbar must be rendered
	void setRenderArea(ViewPort renderArea) {
		this->renderArea = renderArea;
		viewArea.x = (int)renderArea.x;
		viewArea.y = (int)renderArea.y;
		viewArea.w = (int)renderArea.w;
		viewArea.h = (int)renderArea.h;
	}

	void renderToolBar();	//Renders the Tool bar

};

#endif
