

/*
	Image Editor

	Making a custom image editor for fun!

	Owen Zonneveld
*/


//SDL
#include "SDL.h"
#undef main	//Necessary IDK why but SDL2 needs this cause "main" is already def.

#include "SDL_image.h"	//For loading images

#include <iostream>	//For printing
#include <thread>

#include "Windows/Window.h"	//Where the window is and such
#include "Windows/LayoutManager.h" //How to handle the layout of a window and what not
#include "Layers/Layer.h"	//Where Layers are stored
#include "Layers/LayerViewer.h"	//How I bind layers to certain screen positions

bool haltProgram = false;

void waitForInput(LayerViewer* myViewer, Layer* myLayer2) {
	//Checks for mouse wheel scrolling
	SDL_Event event;

	//Stores the previous location of the mouse
	int prevMouseX = 0, prevMouseY = 0;

	while (true) {
		if (haltProgram)	//leaves the porgram
			break;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				haltProgram = true;
				break;
			}

			int mouseX, mouseY;	//Gets mouse position
			SDL_GetMouseState(&mouseX, &mouseY);
			
			Camera myCam = myViewer->getCamera();	//Gets the Camera

			if (event.type == SDL_MOUSEWHEEL) {	//Mouse Wheel Scrolls
				const double factor = (event.wheel.y > 0 ? 10.0/9.0 : 0.9);

				int width, height;
				int zoomWidth, zoomHeight;

				int myMouseX = mouseX, myMouseY = mouseY;
				myLayer2->getRelativeMousePosition(&myMouseX, &myMouseY);

				float xPercent = (float)myMouseX / myLayer2->getWidth();
				float yPercent = (float)myMouseY / myLayer2->getHeight();

				myLayer2->getZoomSize(&width, &height);
				myViewer->setCameraZoom(myCam.zoom * factor);
				myLayer2->getZoomSize(&zoomWidth, &zoomHeight);

				float xDif = (zoomWidth - width);
				float yDif = (zoomHeight - height);

				myViewer->setCameraPos(myCam.x - xDif * xPercent, myCam.y - yDif * yPercent);
			}
			
			if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
				Camera myCam = myViewer->getCamera();

				myCam.x += mouseX - prevMouseX;
				myCam.y += mouseY - prevMouseY;

				myViewer->setCameraPos(myCam.x, myCam.y);
			}
			prevMouseX = mouseX;	//Stores the previous mouse position
			prevMouseY = mouseY;
		}
	}
}

static int resizingEventWatcher(void* data, SDL_Event* event) {
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_RESIZED) {
		LayoutManager* manager = (LayoutManager*)data;
		//Layout Manager
		manager->calculateSizes();
		manager->feedSizes();	//Works???
	}
	return 0;
}

int main() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image
		std::cout << "SDL_Image Not Initialized";

	Window myWindow(1500, 1000);

	LayerViewer myViewer(myWindow.getRenderer());
	Toolbar myBar(myWindow.getRenderer());
	LayoutManager myManager(&myWindow, &myViewer, &myBar);

	Layer myLayer(&myViewer, "images/sample2.png");
	myLayer.updateTexture();

	//Layer myLayer2(&myViewer, "images/Benvery_sexy.jpg");
	//myLayer2.updateTexture();

	SDL_Color white;
	white.r = 0;
	white.g = 255;
	white.b = 255;
	white.a = 50;
	SDL_Color color2;
	color2.r = 255;
	color2.g = 255;
	color2.b = 0;
	color2.a = 50;

	//watches for window size changes
	SDL_AddEventWatch(resizingEventWatcher, &myManager);

	std::thread first(waitForInput, &myViewer, &myLayer);	//Runs input on another thread

	double startTime = 0, endTime = 0;

	while (true) {
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		startTime = SDL_GetTicks();	//Timer start

		myWindow.clearWindow();	//Clears the window
		
		int mouseX, mouseY;

		Camera myCam = myViewer.getCamera();	//Gets the Camera

		if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			int myMouseX = mouseX, myMouseY = mouseY;
			myLayer.getRelativeMousePosition(&myMouseX, &myMouseY);

			const int root = 5;
			const int amount = root * root;
			const int half = root / 2;
			
			int x[amount];
			int y[amount];

			for (int i = 0; i < amount; i++) {
				x[i] = i % root - half;
				y[i] = i / root - half;
			}

			for (int i = 0; i < amount; i++) {
				x[i] += myMouseX;
				y[i] += myMouseY;
			}
			
			myLayer.changePixels(white, x, y, amount);
			myLayer.updateTexture();
		}

		if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			int myMouseX = mouseX, myMouseY = mouseY;
			myLayer.getRelativeMousePosition(&myMouseX, &myMouseY);

			const int root = 5;
			const int amount = root * root;
			const int half = root / 2;

			int x[amount];
			int y[amount];

			for (int i = 0; i < amount; i++) {
				x[i] = i % root - half;
				y[i] = i / root - half;
			}
			for (int i = 0; i < amount; i++) {
				x[i] += myMouseX;
				y[i] += myMouseY;
			}

			myLayer.changePixels(color2, x, y, amount);
			myLayer.updateTexture();
		}

		myLayer.updateRenderArea();
		myLayer.render();
		//myViewer.render();
		myBar.renderToolBar();
		myWindow.render();

		SDL_PumpEvents();	//INPUT

		endTime = SDL_GetTicks();	//Timer end & Print

		if (haltProgram)	//why is this not in the while(true) statement??? cause future plans so im just leaving it as this
			break;
	}

	first.join();	//Waits for the thread to stop. program then stops
	SDL_Quit();	//Leaves SDL

	return -1;
}

