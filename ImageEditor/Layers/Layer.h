#pragma once


#ifndef LAYER_H
#define LAYER_H


//SDL
#include "SDL.h"	//For rendering
#undef main	//NEcessary IDK why but SDL likes this

#include "SDL_image.h"	//For loading images

#include "Windows/Window.h"	//For rendering to


#include <string>
#include <iostream>
/*
	Where you can edit your images
	Allows for multiple layers

	Owen Zonneveld
*/

class Layer {
private:
	int alpha;	//The alpha of the layer
	int width, height;	//Size of the texture

	Window* myWindow;	//So it can get window size
	SDL_Renderer* renderTo;	//Feed in a renderer and the layer may render to it

	SDL_Surface* surface;	//Stores the information of the surface
	SDL_Texture* texture;	//For rendering

	SDL_Rect sourceArea;
	SDL_Rect renderArea;	//What area this will render too

	//double xScale, yScale;	//For the future :P
public:

	/*
		Gets a window to render to & an image to load as a surface
	*/
	Layer(Window* myWindow, std::string image) {
		this->myWindow = myWindow;
		renderTo = myWindow->getRenderer();	//Now points to the renderer
		

		int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		int initted = IMG_Init(imgFlags);
		if ((initted & imgFlags) != imgFlags)
			std::cout << "SDL_Image Not Initialized";

		surface = SDL_ConvertSurfaceFormat(IMG_Load(image.c_str()), SDL_PIXELFORMAT_RGBA32, 0);
		texture = SDL_CreateTextureFromSurface(renderTo, surface);
		
		alpha = SDL_ALPHA_OPAQUE;	//Defaults the alpha to Opaque

		init();
	}//	Layer Constructor
	/*
		Gets a window to render to & the size of the new surface
	*/
	Layer(Window* myWindow, int layerWidth, int layerHeight) {
		this->myWindow = myWindow;
		renderTo = myWindow->getRenderer();	//Now points to the renderer

		surface = SDL_CreateRGBSurfaceWithFormat(0, layerWidth, layerHeight, 32, SDL_PIXELFORMAT_RGBA32);
		texture = SDL_CreateTextureFromSurface(renderTo, surface);	

		init();
	}

	~Layer() {
		SDL_FreeSurface(surface);	//Destroys the surface
		surface = NULL;
		SDL_DestroyTexture(texture);	//Destroys the texture
		texture = NULL;
		renderTo = NULL;	//renderTo becomes empty
	}//	Layer Deconstructor

	void init() {
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);	//Gets size of texture

		renderArea.x = 0;
		renderArea.y = 0;
		renderArea.w = width;
		renderArea.h = height;

		sourceArea.x = 0;
		sourceArea.y = 0;
		sourceArea.w = width;
		sourceArea.h = height;
	}

	//Changes 1 pixel in the area
	void changePixel(SDL_Color color, int x, int y);
	//Changes multiple pixels at once
	void changePixels(SDL_Color color, int* x, int* y, int pixelAmount);

	bool getPixelInCanvas(int x, int y) {
		return (x < width && x >= 0 && y < height && y >= 0);
	}

	//Updates the texture
	//Making the changes visible
	void updateTexture();



	//Gets the coordinates of the mouse related to the layer	 (gets stretched across screen so 4x4 != 8x8)
	void getRelativeMousePosition(int* mouseX, int* mouseY);

	//Gets the coordinates of the pixel related to the screen
	void getScreenPositionFromPixel(int* pixelX, int* pixelY);

	//Gets the zoom size of the layer
	void getZoomSize(int* width, int* height) {
		Camera myCam = myWindow->getCamera();
		*width = this->width * myCam.zoom;
		*height = this->height * myCam.zoom;
	}
	//Gets the size of the layer
	void getSize(int* width, int* height) {
		*width = this->width;
		*height = this->height;
	}
	//Gets the width of the layer
	int getWidth() {
		return width;
	}
	//Gets the height of the layer
	int getHeight() {
		return height;
	}

	//Renders the layer
	void render();

	//Updates where the layer will be rendered
	void updateRenderArea();

};// Layer class


#endif