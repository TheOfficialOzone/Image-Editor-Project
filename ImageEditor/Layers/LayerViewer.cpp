


#include "LayerViewer.h"



//Gets the windows renderer
SDL_Renderer* LayerViewer::getRenderer() {
	return myRenderer;
}// getRenderer

//Renders the Window
void LayerViewer::render() {
	SDL_Rect vA;
	vA.x = layerPort.x;
	vA.y = layerPort.y;
	vA.w = layerPort.w;
	vA.h = layerPort.h;
	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(myRenderer, &vA);
}//	render

//Gets the size of the window
void LayerViewer::getSize(int* width, int* height) {
	*width = layerPort.w;
	*height = layerPort.h;
}// getSize

//Resets the LayerDrawPositions
void LayerViewer::resetCamera() {
	int width, height;
	getSize(&width, &height);

	myCam.x = 0;
	myCam.y = 0;
	myCam.w = width;
	myCam.h = height;
	myCam.zoom = 1.0;
}// resetLayerDrawPositions

	//resets the viewPorts's information
void LayerViewer::resetViewPort() {
	int width, height;
	getSize(&width, &height);

	layerPort.x = 0;
	layerPort.y = 0;
	layerPort.w = 800;
	layerPort.h = 800;
}