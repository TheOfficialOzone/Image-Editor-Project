


#include "Window.h"	//	Header file



//Gets the windows renderer
SDL_Renderer* Window::getRenderer() {
	return renderer;
}// getRenderer

//Removes everything displayed on the renderer
//To White
void Window::clearWindow() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);	//White
	SDL_RenderClear(renderer);	//Clears to white
}//	clearWindow

//Renders the Window
void Window::render() {
	SDL_RenderPresent(renderer);
}//	render

//Gets the size of the window
void Window::getSize(int* width, int* height) {
	SDL_GetWindowSize(window, width, height);
}// getSize

//Gets the size of the window while reffing to zoom
void Window::getRelativeZoomSize(int* width, int* height) {
	getSize(width, height);
	*width *= myCam.zoom;	
	*height *= myCam.zoom;
}

//Resets the LayerDrawPositions
void Window::resetCamera() {

	int width, height;
	getSize(&width, &height);

	myCam.x = 0;
	myCam.y = 0;
	myCam.w = width;
	myCam.h = height;
	myCam.zoom = 1;

	myPort.x = 10;
	myPort.y = 10;
	myPort.w = 1000;
	myPort.h = 1000;
}// resetLayerDrawPositions

//Sets the amount of zoom there is
void Window::setCameraZoom(double nZoom) {
	myCam.zoom = nZoom;
}// setCameraZoomAmount

void Window::setCameraPos(double x, double y) {
	myCam.x = x;
	myCam.y = y;
}
