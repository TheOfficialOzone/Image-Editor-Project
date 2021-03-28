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

	//double layerDrawStartX, layerDrawStartY;	Depreciated
	//double layerDrawEndX, layerDrawEndY;

	Camera myCam;
	ViewPort myPort;
public:
	//Makes the window the size inputed
	Window(int xSize, int ySize) {	
		SDL_CreateWindowAndRenderer(xSize, ySize, SDL_WINDOW_RESIZABLE, &window, &renderer);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		resetCamera();
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
	void getSize(int* width, int* height);	

	//Gets the size of the window while reffing to zoom
	void getRelativeZoomSize(int* width, int* height);

	//resets the camera's information
	void resetCamera();

	//Sets the amount of zoom there is
	void setCameraZoom(double nZoom);
	//Sets the amount of zoom there is
	void setCameraPos(double x, double y);

	//Gets a camera copy from the window
	Camera getCamera() {
		return myCam;
	}
	//Gets a Viewport copy from the window
	ViewPort getViewPort() {
		return myPort;
	}

};// Window Class

#endif