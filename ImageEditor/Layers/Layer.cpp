


#include "Layer.h"


//Changes 1 pixel in the area
void Layer::changePixel(SDL_Color color, int x, int y) {
	SDL_LockSurface(surface);	//Can now safely access pixels

	Uint32* pixels = (Uint32*)surface->pixels;
	Uint32 dColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
	if (getPixelInCanvas(x, y))
		pixels[(y * width) + x] = dColor;	//Sets the pixel to the colour
	
	SDL_UnlockSurface(surface);	//Does the changes
}

//Changes multiple pixels at once
void Layer::changePixels(SDL_Color color, int* x, int* y, int pixelAmount) {
	SDL_LockSurface(surface);	//Can now safely access pixels

	Uint32* pixels = (Uint32*)surface->pixels;
	Uint32 dColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

	for (int i = 0; i < pixelAmount; i++) 
		if (getPixelInCanvas(x[i], y[i]))
			pixels[(y[i] * width) + x[i]] = dColor;	//Sets the pixel to the colour
	

	SDL_UnlockSurface(surface);	//Does the changes
}

//Updates the texture
//Making the changes visible
void Layer::updateTexture() {
	SDL_DestroyTexture(texture);	//Destroys the texture
	texture = SDL_CreateTextureFromSurface(renderTo, surface);	//Creates the texture

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);	//Gets size of texture so the layer isn't off
}

//Renders the layer
void Layer::render() {
	SDL_RenderCopy(renderTo, texture, &sourceArea, &renderArea);	//Copies the image to the renderer
}


//Gets the coordinates of the mouse related to the layer	 (gets stretched across screen so 4x4 != 8x8)
void Layer::getRelativeMousePosition(int* mouseX, int* mouseY) {
	Camera myCam = myWindow->getCamera();
	ViewPort myPort = myWindow->getViewPort();

	*mouseX -= myCam.x + myPort.x;	//Adjustes for Camera pos & Viewport
	*mouseY -= myCam.y + myPort.y;

	*mouseX = (int)(*mouseX / myCam.zoom);	//Adjustes for zoom
	*mouseY = (int)(*mouseY / myCam.zoom);
}

//Gets the coordinates of the pixel related to the screen
void Layer::getScreenPositionFromPixel(int* pixelX, int* pixelY) {
	Camera myCam = myWindow->getCamera();
	ViewPort myPort = myWindow->getViewPort();

	*pixelX = (int)(*pixelX * myCam.zoom);	//Adjustes for zoom
	*pixelY = (int)(*pixelY * myCam.zoom);

	*pixelX += myCam.x + myPort.x;	//Adjustes for Camera pos & Viewport
	*pixelY += myCam.y + myPort.y;
}

//Updates where the layer will be rendered
void Layer::updateRenderArea() {
	/*
	SDL_Rect* ref = myWindow->getLayerDrawRect();

	renderArea.x = ref->x;
	renderArea.y = ref->y;

	renderArea.w = (ref->w < width ? ref->w : width);
	renderArea.h = (ref->h < height ? ref->h : height);
	*/

	Camera myCam = myWindow->getCamera();
	ViewPort myPort = myWindow->getViewPort();

	int rW = 0, rH = 0;
	rW = myPort.w;
	rH = myPort.h;

	renderArea.x = myCam.x + myPort.x;	//render will always be on the camera
	renderArea.y = myCam.y + myPort.y;

	double renderZoom = (myCam.zoom < 1 ? myCam.zoom : 1);	//Can probably be merged together
	double sourceZoom = (myCam.zoom > 1 ? myCam.zoom : 1);

	double zoom = myCam.zoom;

	bool topBottom = true;//If it has done all 4 cases it will get that edge case
	bool leftRight = true;
	/*
	// RIGHT
	if ((myCam.x - myPort.x + width) * renderZoom > rW) {
		renderArea.w = rW - (myCam.x - myPort.x);
		sourceArea.w = (width - (width - (rW - (myCam.x - myPort.x)))) / sourceZoom / renderZoom;
	}
	else {
		renderArea.w = width * renderZoom;
		sourceArea.w = width / sourceZoom;

		leftRight = false;
	}
	*/
	// RIGHT
	if ((myCam.x + width * zoom) > rW) {
		renderArea.w = rW - (myCam.x);
		sourceArea.w = (width - (width - (rW - (myCam.x)))) / zoom;
	}
	else {
		renderArea.w = width * zoom;
		sourceArea.w = width;

		leftRight = false;
	}

	// BOTTOM
	/*
	if ((myCam.y - myPort.y + height) * renderZoom > rH) {
		renderArea.h = rH - (myCam.y - myPort.y);
		sourceArea.h = (height - (height - (rH - (myCam.y - myPort.y)))) / sourceZoom / renderZoom;
	}
	else {
		renderArea.h = height * renderZoom;
		sourceArea.h = height / sourceZoom;

		topBottom = false;
	}*/
	if ((myCam.y + height * zoom) > rH) {
		renderArea.h = rH - (myCam.y);
		sourceArea.h = (height - (height - (rH - (myCam.y)))) / zoom;
	}
	else {
		renderArea.h = height * zoom;
		sourceArea.h = height;

		topBottom = false;
	}

	/*
	// LEFT
	if (myCam.x < myPort.x) {
		renderArea.x = myPort.x;
		renderArea.w = (width + (myCam.x - myPort.x)) * renderZoom;

		sourceArea.x = -(myCam.x - myPort.x) / sourceZoom;
		sourceArea.w = (width + (myCam.x - myPort.x)) / sourceZoom;
	}
	else {
		sourceArea.x = 0;
		leftRight = false;
	}
	*/
	// LEFT
	if (myCam.x < 0) {
		renderArea.x = myPort.x;
		renderArea.w = (width * zoom + (myCam.x));

		sourceArea.x = -(myCam.x) / zoom;
	}
	else {
		sourceArea.x = 0;
		leftRight = false;
	}

	// TOP
	/*
	if (myCam.y < myPort.y) {
		renderArea.y = myPort.y;
		renderArea.h = (height + (myCam.y - myPort.y) > 0 ? height + (myCam.y - myPort.y) : 0) * renderZoom;

		sourceArea.y = -(myCam.y - myPort.y) / sourceZoom;
		sourceArea.h = (height + (myCam.y - myPort.y) > 0 ? height + (myCam.y - myPort.y) : 0) / sourceZoom;
	}
	else {
		sourceArea.y = 0;
		topBottom = false;
	}
	*/
	if (myCam.y < 0) {
		renderArea.y = myPort.y;
		renderArea.h = (height * zoom + (myCam.y));

		sourceArea.y = -(myCam.y) / zoom;
	}
	else {
		sourceArea.y = 0;
		topBottom = false;
	}
	
	//If it has done both Left right case
	if (leftRight) {	//Clip Width
		renderArea.w = rW;
		sourceArea.w = rW / zoom;
	}
	//If it has done both Left right case
	if (topBottom) {	//Clip  height
		renderArea.h = rH;
		sourceArea.h = rH / zoom;
	}

	SDL_Rect dRect;
	dRect.x = myPort.x - 1;
	dRect.y = myPort.y - 1;
	dRect.w = myPort.w + 2;
	dRect.h = myPort.h + 2;
	SDL_SetRenderDrawColor(renderTo, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderTo, &dRect);
}