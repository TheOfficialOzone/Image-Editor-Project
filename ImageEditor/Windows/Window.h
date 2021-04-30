#pragma once


#ifndef WINDOW_H
#define WINDOW_H

//SDL
#include "SDL.h"	//For rendering
#undef main	//NEcessary IDK why but SDL likes this

#include "Camera/Camera.h"
#include <iostream>
/*
	The main window of the program, 
	used for getting specific coordinates from the window

	Owen Zonneveld
*/

class Window {
private:
	SDL_Window* window;	//The window itself
	SDL_Renderer* renderer;	//What will render to this window

public:
	//Makes the window the size inputed with the appropriate Cameras
	Window(int xSize, int ySize, Camera newCam, ViewPort newPort) {	
		SDL_CreateWindowAndRenderer(xSize, ySize, SDL_WINDOW_RESIZABLE, &window, &renderer);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}//	Window Constructor

	//Makes the window the size inputed with a default Camera
	Window(int xSize, int ySize) {
		SDL_CreateWindowAndRenderer(xSize, ySize, SDL_WINDOW_RESIZABLE, &window, &renderer);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}//	Window Constructor


	~Window() {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;

		SDL_DestroyWindow(window);
		window = NULL;
	}//	Window Deconstructor

	SDL_Renderer* getRenderer();	//Gets the windows renderer

	//Clears the Window
	void clearWindow();
	//Renders the Window
	void render();	

	//Gets the size of the window
	void getSize(int* width, int* height) {
		SDL_GetWindowSize(window, width, height);
	}// getSize

	//Gets the size of the window while reffing to zoom

};// Window Class

#endif