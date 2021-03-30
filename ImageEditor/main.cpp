

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
#include "Layers/Layer.h"	//Where Layers are stored

bool haltProgram = false;

void waitForInput(Window* myWindow, Layer* myLayer2) {
	//Checks for mosue wheel scrolling
	SDL_Event event;

	//Stores the previous location of the mouse
	int prevMouseX = 0, prevMouseY = 0;

	while (true) {
		if (haltProgram)	//leaves the porgram
			break;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				haltProgram = true;


			int mouseX, mouseY;	//Gets mouse position
			SDL_GetMouseState(&mouseX, &mouseY);
			
			Camera myCam = myWindow->getCamera();	//Gets the Camera

			if (event.type == SDL_MOUSEWHEEL) {	//Mouse Wheel Scrolls
				const double factor = (event.wheel.y > 0 ? 1.1 : 0.9);

				int width, height;
				int zoomWidth, zoomHeight;

				int myMouseX = mouseX, myMouseY = mouseY;
				myLayer2->getRelativeMousePosition(&myMouseX, &myMouseY);

				float xPercent = (float)myMouseX / myLayer2->getWidth();
				float yPercent = (float)myMouseY / myLayer2->getHeight();

				myLayer2->getZoomSize(&width, &height);
				myWindow->setCameraZoom(myCam.zoom * factor);
				myLayer2->getZoomSize(&zoomWidth, &zoomHeight);

				float xDif = (zoomWidth - width);
				float yDif = (zoomHeight - height);

				myWindow->setCameraPos(myCam.x - xDif * xPercent, myCam.y - yDif * yPercent);
			}
			
			if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {

				Camera myCam = myWindow->getCamera();

				myCam.x += mouseX - prevMouseX;
				myCam.y += mouseY - prevMouseY;

				myWindow->setCameraPos(myCam.x, myCam.y);
			}
			prevMouseX = mouseX;	//Stores the previous mouse position
			prevMouseY = mouseY;
		}
	}
}

int main() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image
		std::cout << "SDL_Image Not Initialized";

	Window myWindow(1500, 1000);

	Layer myLayer(&myWindow, "images/sample2.png");
	myLayer.updateTexture();

	Layer myLayer2(&myWindow, "images/Perfect.jpg");
	myLayer2.updateTexture();

	SDL_Color white;
	white.r = 0;
	white.g = 255;
	white.b = 255;
	white.a = 50;

	std::thread first(waitForInput, &myWindow, &myLayer2);	//Runs input on another thread

	double startTime = 0, endTime = 0;
	while (true) {
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		startTime = SDL_GetTicks();	//Timer start

		myWindow.clearWindow();
		
		int mouseX, mouseY;

		Camera myCam = myWindow.getCamera();	//Gets the Camera

		if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			int myMouseX = mouseX, myMouseY = mouseY;
			myLayer2.getRelativeMousePosition(&myMouseX, &myMouseY);

			const int root = 50;
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
			
			myLayer2.changePixels(white, x, y, amount);
			myLayer2.updateTexture();
		}

		myLayer.updateRenderArea();
		myLayer.render();

		myLayer2.updateRenderArea();
		myLayer2.render();

		//myLayer3.updateRenderArea();
		//myLayer3.render();

		myWindow.render();

		SDL_PumpEvents();	//INPUT

		endTime = SDL_GetTicks();	//Timer end & Print
		//std::cout << endTime - startTime << std::endl;


		if (haltProgram)
			break;
	}
	first.join();	//Waits for the thread to stop. program then stops
	SDL_Quit();	//Leaves SDL

	return -1;
}

