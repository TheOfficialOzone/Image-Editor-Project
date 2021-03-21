


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
void Window::resetLayerDrawPositions() {
	layerDrawStartX = 0;	//Resets the layer draw start
	layerDrawStartY = 0;

	int width, height;
	getSize(&width, &height);
	layerDrawEndX = width;
	layerDrawEndY = height;

	myCam.x = 0;
	myCam.y = 0;
	myCam.w = width;
	myCam.h = height;
	myCam.zoom = 1;
}// resetLayerDrawPositions

//Setting the layerDrawPositions() 
void Window::setLayerDrawPositions(SDL_Rect* newPos) {
	layerDrawStartX = newPos->x;
	layerDrawStartY = newPos->y;

	layerDrawEndX = newPos->w + newPos->x;
	layerDrawEndY = newPos->h + newPos->y;

}// setLayerDrawPositions

//Setting the layerDrawPositions
void Window::setLayerDrawPositions(double xStart, double yStart, double xEnd, double yEnd) {
	layerDrawStartX = xStart;
	layerDrawStartY = yStart;

	layerDrawEndX = xEnd;
	layerDrawEndY = yEnd;
}// setLayerDrawPositions

//Gets an SDL_Rect of where to draw the layer
SDL_Rect* Window::getLayerDrawRect() {
	SDL_Rect* drawPosRect = new SDL_Rect();

	drawPosRect->x = layerDrawStartX;
	drawPosRect->y = layerDrawStartY;

	drawPosRect->w = layerDrawEndX - layerDrawStartX;
	drawPosRect->h = layerDrawEndY - layerDrawStartY;

	return drawPosRect;
}// getLayerDrawRect

//Gets the camera from the window
Camera Window::getCamera() {
	return myCam;
}// getCamera

//Sets the amount of zoom there is
void Window::setCameraZoom(double nZoom) {
	myCam.zoom = nZoom;
}// setCameraZoomAmount

void Window::setCameraPos(double x, double y) {
	myCam.x = x;
	myCam.y = y;
}

//Gets the numbers for where to draw the layer
void Window::getLayerDrawPositions(double* xStart, double* yStart, double* xEnd, double* yEnd) {
	*xStart = layerDrawStartX;
	*yStart = layerDrawStartY;
	*xEnd = layerDrawEndX;
	*yEnd = layerDrawEndY;
}// getLayerDrawPositions