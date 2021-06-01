#pragma once


#ifndef LAYERVIEWER_H
#define LAYERVIEWER_H

#include "SDL.h"

#include "Camera/Camera.h"
#include "Layer.h"
#include <vector>



class LayerViewer {
	SDL_Renderer* myRenderer;	//What will render to this window

	Camera* myCam;
	SDL_FRect* layerPort;

	std::vector<Layer*> myLayers;
	void init() {
		layerPort = new SDL_FRect();
		myCam = new Camera();
	}

public:
	//Makes the window the size inputed with the appropriate Cameras
	LayerViewer(SDL_Renderer* myRenderer, Camera* newCam, SDL_FRect* newPort) {
		init();

		this->myRenderer = myRenderer;
		setViewPort(newPort); //Sets the camera & viewport
		setCamera(newCam);
		
	}//	Window Constructor

	//Makes the window the size inputed with the appropriate Cameras
	LayerViewer(SDL_Renderer* myRenderer) {
		init();

		this->myRenderer = myRenderer;
		resetViewPort();
		resetCamera();	//Sets the camera & viewport
	}//	Window Constructor

	~LayerViewer() {
		myRenderer = NULL;
		myLayers.clear();
	}//	Window Deconstructor

	SDL_Renderer* getRenderer();	//Gets the LayerViewers renderer

	//Renders the LayerViewers
	void render();

	//Gets the size of the LayerViewer
	void getSize(int* width, int* height);

	//resets the camera's information
	void resetCamera();
	//resets the viewPorts's information
	void resetViewPort();

	//Sets the amount of zoom there is
	void setCameraZoom(double nZoom) {
		myCam->zoom = nZoom;
	}

	//Sets the position of the LayerViewer's Camera
	void setCameraPos(double x, double y) {
		myCam->x = x;
		myCam->y = y;
	}

	//Gets a camera copy from the LayerViewer
	Camera* getCamera() {
		return myCam;
	}
	//Sets a new Camera for the LayerViewer
	void setCamera(Camera* newCam) {
		myCam = newCam;
	}

	//Gets a Viewport copy from the window
	SDL_FRect* getViewPort() {
		return layerPort;
	}

	//Sets a new Viewport for the Window
	void setViewPort(SDL_FRect* newPort) {
		layerPort = newPort;
	}

	//Gets the coordinates of the mouse related to the layerViewer	 (gets stretched across screen so 4x4 != 8x8)
	void getRelativeMousePosition(double& mouseX, double& mouseY) {
		mouseX -= myCam->x + layerPort->x;	//Adjustes for Camera pos & Viewport
		mouseY -= myCam->y + layerPort->y;

		mouseX = (int)(mouseX / myCam->zoom);	//Adjustes for zoom
		mouseY = (int)(mouseY / myCam->zoom);
	}

	//Gets the Width
	double getWidth() {
		return layerPort->w;
	}

	//Gets the Height
	double getHeight() {
		return layerPort->h;
	}

	//Gets the zoom size of the layer
	void getZoomSize(double& width, double& height) {
		width = this->getWidth() * myCam->zoom;
		height = this->getHeight() * myCam->zoom;
	}

	//Updates the layers so they render within the Viewport
	void updateLayers() {
		for (int i = 0; i < myLayers.size(); i++) {
			myLayers[i]->updateRenderArea(this);
		}
	}

	//renders the Layers
	void renderLayers() {
		for (int i = 0; i < getLayerAmount(); i++) {
			myLayers[i]->render();
		}
	}

	//Gets the layer at a specific index
	Layer* getLayerAtIndex(int index) {
		return myLayers.at(index);
	}

	//Gets the amount of layers there currently are
	int getLayerAmount() {
		return myLayers.size();
	}

	//Adds a Layer
	void addLayer(Layer* newLayer) {	//Adds a layer to the ebd
		myLayers.push_back(newLayer);
	}

	//Removes a Layer
	void removeLayer(int layerPos) {
		myLayers.erase(myLayers.begin() + layerPos);
	}

	//If a point is over
	bool pointOverLayer(int x, int y) {
		return (layerPort->x < x && layerPort->y < y && layerPort->x + layerPort->w > x && layerPort->y + layerPort->h > y);
	}
};





#endif