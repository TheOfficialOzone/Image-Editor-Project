

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



void waitForInput(Window* myWindow) {
	//Checks for mosue wheel scrolling
	SDL_Event event;

	//Stores the previous location of the mouse
	int prevMouseX = 0, prevMouseY = 0;

	while (true) {
		while (SDL_PollEvent(&event)) {
			int mouseX, mouseY;	//Gets mouse position
			SDL_GetMouseState(&mouseX, &mouseY);
			/*
			if (event.type == SDL_MOUSEWHEEL) {	//Mouse Wheel Scrolls


				
				double dStartX, dStartY, dEndX, dEndY;	//Getting the displayed stuffs position
				myWindow->getLayerDrawPositions(&dStartX, &dStartY, &dEndX, &dEndY);

				//Gets aspectRatio
				double viewWidth = dEndX - dStartX;
				double viewHeight = dEndY - dStartY;

				double aspectRatio = viewWidth / viewHeight;

				//If the mouse wheel is infact moved
				//Inverses on zoom out
				double mod = 1.0; 	//Scroll up

				//Coordinates
				double toLeft = mouseX - dStartX;	// X
				double toRight = dEndX - mouseX;

				double toTop = mouseY - dStartY;	// Y
				double toBottom = dEndY - mouseY;

				if (event.wheel.y < 0) 	//Scroll down
					mod = -1.0;

				double zoomAmount = 30.0;	//How far it zooms

				//Zooms from center
				dStartX -= (toLeft / viewWidth) * aspectRatio * zoomAmount * mod;
				dEndX += (toRight / viewWidth) * aspectRatio * zoomAmount * mod;

				dStartY -= (toTop / viewHeight) * zoomAmount * mod;
				dEndY += (toBottom / viewHeight) * zoomAmount * mod;

				myWindow->setLayerDrawPositions(dStartX, dStartY, dEndX, dEndY);
			}
			*/
			if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {

				Camera myCam = myWindow->getCamera();
				//SDL_Rect* windowPos = myWindow->getLayerDrawRect();

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

	Layer myLayer(&myWindow, "images/sample3.png");
	myLayer.updateTexture();
	Layer myLayer2(&myWindow, "images/sample2.png");
	myLayer2.updateTexture();


	SDL_Color white;
	white.r = 0;
	white.g = 0;
	white.b = 0;
	white.a = 255;
	

	std::thread first(waitForInput, &myWindow);	//Runs input on another thread

	double startTime = 0, endTime = 0;
	while (true) {
		startTime = SDL_GetTicks();	//Timer start

		myWindow.clearWindow();
		
		int mouseX, mouseY;

		Camera myCam = myWindow.getCamera();
		//myWindow.setCameraPos(myCam.x - 0.01, 0);
		//myWindow.setCameraZoomAmount(myCam.zoom - 0.001);

		if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			int width, height;
			int zoomWidth, zoomHeight;

			myWindow.getRelativeZoomSize(&width, &height);
			myWindow.setCameraZoom(myCam.zoom + 0.01);
			myWindow.getRelativeZoomSize(&zoomWidth, &zoomHeight);
			float xDif = zoomWidth - width;
			std::cout << zoomWidth << std::endl;
			myWindow.setCameraPos(myCam.x - xDif / 4.0f, myCam.y);

			/*
			myLayer.getRelativeMousePosition(&mouseX, &mouseY);

			int* x = new int[10];
			int* y = new int[10];
			for (int i = 0; i < 10; i++) {
				x[i] = mouseX + i;
				y[i] = mouseY + i;
			}

			myLayer.changePixels(white, x, y, 10);
			myLayer.updateTexture();
			*/
		}
		if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			myWindow.setCameraZoom(myCam.zoom - 0.01);
		}
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_LEFT]) myWindow.setCameraPos(myCam.x - 1, myCam.y);
		if (keys[SDL_SCANCODE_RIGHT]) myWindow.setCameraPos(myCam.x + 1, myCam.y);


		myLayer2.updateRenderArea();
		//myLayer.updateRenderArea();

		myLayer2.render();
		//myLayer.render();
		
		myWindow.render();

		SDL_PumpEvents();	//Allows to move the window

		endTime = SDL_GetTicks();	//Timer end & Print
		//std::cout << endTime - startTime << std::endl;
	}
	SDL_Quit();	//Leaves SDL

	return -1;
}

