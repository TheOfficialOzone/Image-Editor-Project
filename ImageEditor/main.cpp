

/*
	Image Editor

	Making a custom image editor for fun!

	Owen Zonneveld
*/




//SDL
#include "SDL.h"
#undef main	//Necessary IDK why but SDL likes this

#include <iostream>	//For printing
#include <thread>

#include "Windows/Window.h"	//Where the window is and such
#include "Layers/Layer.h"	//Where Layers are stored



void waitForInput(Window* myWindow, Layer* myLayer2) {
	//Checks for mosue wheel scrolling
	SDL_Event event;

	//Stores the previous location of the mouse
	int prevMouseX = 0, prevMouseY = 0;

	while (true) {
		while (SDL_PollEvent(&event)) {
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
	SDL_Init(SDL_INIT_VIDEO);

	Window myWindow(1500, 1000);

	Layer myLayer(&myWindow, "images/sample2.png");
	myLayer.updateTexture();

	Layer myLayer2(&myWindow, "images/Perfect.jpg");
	myLayer2.updateTexture();

	SDL_Color white;
	white.r = 0;
	white.g = 0;
	white.b = 255;
	white.a = 120;

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

			int x[9];
			int y[9];

			for (int i = 0; i < 9; i++) {
				x[i] = i % 3 - 1;
				y[i] = i / 3 - 1;
			}

			for (int i = 0; i < 9; i++) {
				x[i] += myMouseX;
				y[i] += myMouseY;
			}
			
			myLayer2.changePixels(white, x, y, 9);
			myLayer2.updateTexture();
		}



		myLayer2.updateRenderArea();
		myLayer2.render();

		myLayer.updateRenderArea();
		myLayer.render();

		myWindow.render();

		SDL_PumpEvents();	//INPUT

		endTime = SDL_GetTicks();	//Timer end & Print
		//std::cout << endTime - startTime << std::endl;
	}
	SDL_Quit();	//Leaves SDL

	return -1;
}

