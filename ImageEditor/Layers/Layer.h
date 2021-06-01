#pragma once


#ifndef LAYER_H
#define LAYER_H

class LayerViewer;

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
	SDL_FRect renderArea;	//What area this will render too

	bool layerError = false;

	//Initializes the texture
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

		alpha = SDL_ALPHA_OPAQUE;	//Defaults the alpha to Opaque
	}
public:
	/*
		Gets a window to render to & an image to load as a surface
	*/
	Layer(Window* myWindow, std::string image) {
		this->myWindow = myWindow;
		renderTo = myWindow->getRenderer();	//Now points to the renderer
		surface = IMG_Load(image.c_str());	//Loads the surface from the file

		if (SDL_SetColorKey(surface, SDL_TRUE, SDL_RLEACCEL) == -1)	//Adds an alpha value per pixel
			std::cout << "Color error";

		if (surface == NULL)
			layerError = true;

		surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);	//Converts to this pixel format
		texture = SDL_CreateTextureFromSurface(renderTo, surface);
		
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

	/*
		Gets a renderer & a string to load as a surface
	*/
	Layer(SDL_Renderer* tRender, std::string image) {
		renderTo = tRender;	//Now points to the renderer
		surface = IMG_Load(image.c_str());	//Loads the surface from the file

		if (SDL_SetColorKey(surface, SDL_TRUE, SDL_RLEACCEL) == -1)	//Adds an alpha value per pixel
			std::cout << "Color error";

		if (surface == NULL)
			layerError = true;

		surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);	//Converts to this pixel format
		texture = SDL_CreateTextureFromSurface(renderTo, surface);

		init();
	}//	Layer Constructor

	~Layer() {
		SDL_FreeSurface(surface);	//Destroys the surface
		surface = NULL;
		SDL_DestroyTexture(texture);	//Destroys the texture
		texture = NULL;
		renderTo = NULL;	//renderTo becomes empty
		myWindow = NULL;	//myWindow becomes empty
	}//	Layer Deconstructor



	//Changes 1 pixel in the area
	void changePixel(SDL_Color color, int x, int y);
	//Changes multiple pixels at once
	void changePixels(SDL_Color color, int* x, int* y, int pixelAmount);

	bool isPixelInCanvas(int x, int y) {
		return (x < width && x >= 0 && y < height && y >= 0);
	}

	//Updates the texture
	//Making the changes visible
	void updateTexture();

	//Gets the coordinates of the mouse related to the layer	 (gets stretched across screen so 4x4 != 8x8)
	void getRelativeMousePosition(LayerViewer* myViewer, int* mouseX, int* mouseY);

	//Gets the coordinates of the pixel related to the screen
	void getScreenPositionFromPixel(LayerViewer* myViewer, int* pixelX, int* pixelY);

	//Gets the zoom size of the layer
	void getZoomSize(LayerViewer* myViewer, int* width, int* height);
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

	//Is the layer safe to modify?
	bool isSafe() {
		return !layerError;
	}

	//Updates where the layer will be rendered
	void updateRenderArea(LayerViewer* myViewer);

};// Layer class


#endif