

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
#include "Tools/Interactables/interactables.h"	//Clickable buttons etc


#define BUTTONAMOUNT 4
bool haltProgram = false;

void waitForInput(LayoutManager* myManager, UserInfoCache* myCache) {
	//Checks for mouse wheel scrolling
	SDL_Event event;

	LayerViewer* myViewer = myManager->getLayerViewer();
	Toolbar* myToolbar = myManager->getToolbar();
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

			const Uint8* keys = SDL_GetKeyboardState(NULL);	//resets the keys state

			int tempX, tempY;	//Gets mouse position
			SDL_GetMouseState(&tempX, &tempY);
			int64_t mouseX = tempX, mouseY = tempY;

			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				//double myMouseX = mouseX, myMouseY = mouseY;
				//myViewer->getRelativeMousePosition(myMouseX, myMouseY);

				//const int root = myCache->getBrushSize();
				//const int amount = root * root;
				//const int half = root / 2;

				//int* x = new int[amount];
				//int* y = new int[amount];

				//for (int i = 0; i < amount; i++) {
				//	x[i] = i % root - half;
				//	y[i] = i / root - half;
				//}

				//for (int i = 0; i < amount; i++) {
				//	x[i] += myMouseX;
				//	y[i] += myMouseY;
				//}

				//Layer* editLayer = myViewer->getLayerAtIndex(myCache->getLayerID());
				//editLayer->changePixels(myCache->getColour(), x, y, amount);
				//editLayer->updateTexture();

				//delete x, y;

				std::vector<Interactable*>* theButtons = myToolbar->getInteractables();	//Gets the interactables
				for (int i = 0; i < theButtons->size(); i++) {
					Interactable* myInteract = theButtons->at(i);
					bool ans = myInteract->checkIfClickInBounds(mouseX, mouseY);

					if (ans) {
						switch (myInteract->getOperationID()) {
						case UpLayer:
							myCache->changeLayerID(1);
							break;
						case DownLayer:
							myCache->changeLayerID(-1);
							break;
						case ChangeColour:
							myInteract->actionClick(&mouseX, &mouseY);
							break;
						case ChangeHue:
							myInteract->actionClick(&mouseX, &mouseY);
							break;
						case ChangeBrushSize:
							myInteract->actionClick(&mouseX, &mouseY);
							InteractBar* tempBar = (InteractBar*)myInteract;
							int newBrushSize = 100 * tempBar->getFillPercent();
							myCache->setBrushSize(newBrushSize);
							break;
						}
					}
				}
			}
			
			if (event.type == SDL_MOUSEWHEEL) {	//Mouse Wheel Scrolls
				if (!myViewer->pointOverLayer(tempX, tempY)) {
					int wheelSignValue = (event.wheel.y > 0 ? 1 : -1);	//Gives either a 1 or -1 

					std::vector<Interactable*>* theButtons = myToolbar->getInteractables();	//Gets the interactables
					for (int i = 0; i < theButtons->size(); i++) {
						Interactable* myInteract = theButtons->at(i);
						bool ans = myInteract->checkIfClickInBounds(mouseX, mouseY);

						if (ans)
							myInteract->actionScroll(wheelSignValue);

						if (myInteract->getOperationID() == ChangeBrushSize) {
							InteractBar* tempBar = (InteractBar*)myInteract;
							int newBrushSize = 100 * tempBar->getFillPercent();
							tempBar->getUserCache()->setBrushSize(newBrushSize);
						}
					}
				} else {
					const double factor = (event.wheel.y > 0 ? 10.0 / 9.0 : 0.9);
					Camera* myCam = myViewer->getCamera();	//Gets the Camera

					double width, height;
					double zoomWidth, zoomHeight;

					double myMouseX = mouseX, myMouseY = mouseY;

					myViewer->getRelativeMousePosition(myMouseX, myMouseY);

					double xPercent = (double)myMouseX / myViewer->getWidth();
					double yPercent = (double)myMouseY / myViewer->getHeight();

					myViewer->getZoomSize(width, height);
					myViewer->setCameraZoom(myCam->zoom * factor);
					myViewer->getZoomSize(zoomWidth, zoomHeight);

					double xDif = (zoomWidth - width);
					double yDif = (zoomHeight - height);

					myViewer->setCameraPos(myCam->x - xDif * xPercent, myCam->y - yDif * yPercent);
				}
			}
			
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
				Camera* myCam = myViewer->getCamera();

				myCam->x += mouseX - prevMouseX;
				myCam->y += mouseY - prevMouseY;

				myViewer->setCameraPos(myCam->x, myCam->y);
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
		manager->feedSizes();

		manager->clearWindow();
		manager->forceRenderAll();
	}
	return 0;
}

int main() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0)	//Initialize SDL
		std::cout << "SDL2 Not Initialized";

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)	//Initializing SDL_Image
		std::cout << "SDL_Image Not Initialized";

	Window* myWindow = new Window(1500, 800);
	LayerViewer* myViewer = new LayerViewer(myWindow->getRenderer());

	InteractButton** myButtons = new InteractButton*[BUTTONAMOUNT];
	Interactable::resetIDGenerator();	//Defaults the generator to 0
	for (int i = 0; i < BUTTONAMOUNT; i++) {
		myButtons[i] = new InteractButton(myWindow->getRenderer());
	}
	myButtons[0]->setTexture(myWindow->getRenderer(), "Tools/ToolbarImages/redPencil.png");
	myButtons[1]->setTexture(myWindow->getRenderer(), "Tools/ToolbarImages/Eraser.png");
	myButtons[2]->setTexture(myWindow->getRenderer(), "Tools/ToolbarImages/Up Arrow.png");
	myButtons[3]->setTexture(myWindow->getRenderer(), "Tools/ToolbarImages/downArrow.png");
	myButtons[1]->setOperationID(Eraser);
	myButtons[2]->setOperationID(UpLayer);
	myButtons[3]->setOperationID(DownLayer);

	Interactable** myInteractables = (Interactable**)myButtons;

	Toolbar* myBar = new Toolbar(myWindow->getRenderer(), new SDL_FRect());
	LayoutManager* myManager = new LayoutManager(myWindow, myViewer, myBar);

	myBar->addInteractable(myInteractables, BUTTONAMOUNT);


	Layer* myLayer4 = new Layer(myWindow->getRenderer(), "images/sample2.png");
	Layer* myLayer3 = new Layer(myWindow->getRenderer(), "images/Benvery_sexy.jpg");

	myViewer->addLayer(myLayer4);
	myViewer->addLayer(myLayer3);
	myViewer->updateLayers();

	SDL_Color white;	//Test Colours
	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;
	SDL_Color color2;
	color2.r = 255;
	color2.g = 0;
	color2.b = 0;
	color2.a = 255;

	//Holding info for the user
	UserInfoCache* myCache = new UserInfoCache(myViewer, color2);

	//Watches for window size changes
	SDL_AddEventWatch(resizingEventWatcher, myManager);
	std::thread first(waitForInput, myManager, myCache);	//Runs input on another thread

	InteractColourSelector* col = new InteractColourSelector(myWindow->getRenderer(), color2, myCache);	//Makes a colour selector
	col->setOperationID(ChangeColour);
	InteractHueSelector* hueSelector = new InteractHueSelector(myWindow->getRenderer(), col);
	hueSelector->setOperationID(ChangeHue);
	myBar->addInteractable(col);
	myBar->addInteractable(hueSelector);

	InteractBar* changeBar = new InteractBar(myWindow->getRenderer(), myCache, color2);
	changeBar->setOperationID(ChangeBrushSize);
	changeBar->setFillPercent(0.5f);

	myBar->addInteractable(changeBar);

	myManager->updateLayout();	//Fixes the toolbar

	while (true) {
		Uint32 num = SDL_GetTicks();
		myWindow->clearWindow();	//Clears the window
		int mouseX, mouseY;
		 
		myManager->forceRenderAll();

		Uint32 num2 = SDL_GetTicks();

		SDL_PumpEvents();	//INPUT

		if (haltProgram)	//why is this not in the while(true) statement??? cause future plans so im just leaving it as this
			break;
	}

	first.join();	//Waits for the thread to stop. program then stops
	SDL_Quit();	//Leaves SDL

	return -1;
}

