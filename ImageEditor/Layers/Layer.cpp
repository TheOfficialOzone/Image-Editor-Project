


#include "Layer.h"
#include "LayerViewer.h"

//Changes 1 pixel in the area
void Layer::changePixel(SDL_Color color, int x, int y) {
	if (isSafe()) {
		SDL_LockSurface(surface);	//Can now safely access pixels

		Uint32* pixels = (Uint32*)surface->pixels;
		Uint32 dColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
		if (isPixelInCanvas(x, y))
			pixels[(y * width) + x] = dColor;	//Sets the pixel to the colour

		SDL_UnlockSurface(surface);	//Does the changes
	}
}

//Changes multiple pixels at once
void Layer::changePixels(SDL_Color color, int* x, int* y, int pixelAmount) {
	if (isSafe()) {
		SDL_LockSurface(surface);	//Can now safely access pixels

		Uint32* pixels = (Uint32*)surface->pixels;
		Uint32 dColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

		for (int i = 0; i < pixelAmount; i++)
			if (isPixelInCanvas(x[i], y[i]))
				pixels[(y[i] * width) + x[i]] = dColor;	//Sets the pixel to the colour

		SDL_UnlockSurface(surface);	//Does the changes
	}
}

//Updates the texture
//Making the changes visible
void Layer::updateTexture() {
	SDL_DestroyTexture(texture);	//Destroys the texture
	SDL_Texture* deleteTexture = texture;
	texture = SDL_CreateTextureFromSurface(renderTo, surface);	//Creates the texture
	SDL_DestroyTexture(deleteTexture);
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);	//Gets size of texture so the layer isn't off
}

//Renders the layer
void Layer::render() {
	SDL_RenderCopyF(renderTo, texture, &sourceArea, &renderArea);	//Copies the image to the renderer
}


//Gets the coordinates of the mouse related to the layer	 (gets stretched across screen so 4x4 != 8x8)
void Layer::getRelativeMousePosition(LayerViewer* myViewer, int* mouseX, int* mouseY) {
	Camera* myCam = myViewer->getCamera();
	SDL_FRect* myPort = myViewer->getViewPort();

	*mouseX -= myCam->x + myPort->x;	//Adjustes for Camera pos & Viewport
	*mouseY -= myCam->y + myPort->y;

	*mouseX = (int)(*mouseX / myCam->zoom);	//Adjustes for zoom
	*mouseY = (int)(*mouseY / myCam->zoom);
}

//Gets the coordinates of the pixel related to the screen
void Layer::getScreenPositionFromPixel(LayerViewer* myViewer, int* pixelX, int* pixelY) {
	Camera* myCam = myViewer->getCamera();
	SDL_FRect* myPort = myViewer->getViewPort();

	*pixelX = (int)(*pixelX * myCam->zoom);	//Adjustes for zoom
	*pixelY = (int)(*pixelY * myCam->zoom);

	*pixelX += myCam->x + myPort->x;	//Adjustes for Camera pos & Viewport
	*pixelY += myCam->y + myPort->y;
}

void Layer::getZoomSize(LayerViewer* myViewer, int* width, int* height) {
	Camera* myCam = myViewer->getCamera();
	*width = this->width * myCam->zoom;
	*height = this->height * myCam->zoom;
}
//Updates where the layer will be rendered
void Layer::updateRenderArea(LayerViewer* myViewer) {
	Camera* myCam = myViewer->getCamera();
	SDL_FRect* myPort = myViewer->getViewPort();

	double rW = 0, rH = 0;
	rW = myPort->w;
	rH = myPort->h;

	renderArea.x = myCam->x + myPort->x;	//render will always be on the camera
	renderArea.y = myCam->y + myPort->y;

	double zoom = myCam->zoom;

	bool topBottom = true;//If it has done all 4 cases it will get that edge case
	bool leftRight = true;

	int addOne = (myCam->zoom == 1.0 ? 0 : 1);
	// RIGHT
	if ((myCam->x + width * zoom) > rW) {
		renderArea.w = rW - myCam->x + addOne;	//Plus one so it touches the right edge when not over the left edge
		sourceArea.w = (rW - myCam->x) / zoom;
	}
	else {
		renderArea.w = width * zoom;
		sourceArea.w = width;

		leftRight = false;
	}

	// BOTTOM
	if ((myCam->y + height * zoom) > rH) {
		renderArea.h = rH - myCam->y + addOne;	//Plus one so it touches the bottom edge when not over the top edge
		sourceArea.h =  (rH - myCam->y) / zoom;
	}
	else {
		renderArea.h = height * zoom;
		sourceArea.h = height;

		topBottom = false;
	}

	// LEFT
	if (myCam->x < 0) {
		renderArea.x = myPort->x;
		renderArea.w = (width * zoom + myCam->x);

		sourceArea.x = -myCam->x / zoom;
	}
	else {
		sourceArea.x = 0;
		leftRight = false;
	}

	// TOP
	if (myCam->y < 0) {
		renderArea.y = myPort->y;
		renderArea.h = (height * zoom + myCam->y);

		sourceArea.y = -myCam->y / zoom;
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
}