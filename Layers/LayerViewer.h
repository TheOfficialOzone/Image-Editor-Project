#pragma once


#ifndef LAYERVIEWER_H
#define LAYERVIEWER_H

#include "SDL.h"

#include "Camera/Camera.h"

class LayerViewer {
	SDL_Renderer* myRenderer;	//What will render to this window

	Camera myCam;
	ViewPort layerPort;
public:
	//Makes the window the size inputed with the appropriate Cameras
	LayerViewer(SDL_Renderer* myRenderer, Camera newCam, ViewPort newPort) {
		this->myRenderer = myRenderer;
		myCam = newCam;	//Sets the camera & viewport
		layerPort = newPort;
	}//	Window Constructor

	//Makes the window the size inputed with the appropriate Cameras
	LayerViewer(SDL_Renderer* myRenderer) {
		this->myRenderer = myRenderer;
		resetCamera();	//Sets the camera & viewport
		resetViewPort();
	}//	Window Constructor


	~LayerViewer() {
		myRenderer = NULL;
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
		myCam.zoom = nZoom;
	}
	//Sets the position of the LayerViewer's Camera
	void setCameraPos(double x, double y) {
		myCam.x = x;
		myCam.y = y;
	}

	//Gets a camera copy from the LayerViewer's
	Camera getCamera() {
		return myCam;
	}
	//Sets a new Camera for the LayerViewer's
	void setCamera(Camera newCam) {
		myCam = newCam;
	}

	//Gets a Viewport copy from the window
	ViewPort getViewPort() {
		return layerPort;
	}
	//Sets a new Viewport for the Window
	void setViewPort(ViewPort newPort) {
		layerPort = newPort;
	}
};





#endif