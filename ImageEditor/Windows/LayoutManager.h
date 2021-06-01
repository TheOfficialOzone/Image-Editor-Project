#pragma once

#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include "SDL.h"
#include <cmath>
#include "Window.h"
#include "Layers/LayerViewer.h"
#include "Tools/Toolbar.h"

class LayoutManager {
private:
	double layerYPercent, layerXPercent;	//Relevant sizes
	double layerWidthToPercent, layerHeightToPercent;
	
	//Where the layer viewer will be
	double layerViewerX, layerViewerY;
	double layerViewerW, layerViewerH;

	double toolbarX, toolbarY;
	double toolbarW, toolbarH;

	LayerViewer* myLayerViewer;
	Toolbar* myToolbar;

	Window* myWindow;
	
	//Default stuff that happens at the start
	void init() {
		layerXPercent = 0.0;
		layerYPercent = 0.0;
		layerWidthToPercent = 0.8;
		layerHeightToPercent = 1.0;

		calculateSizes();
	}

public:
	//Constructor
	//Takes a Window for getSize functions and what not
	LayoutManager(Window* myWindow, LayerViewer* myLayerViewer, Toolbar* myToolbar){
		this->myWindow = myWindow;
		this->myLayerViewer = myLayerViewer;
		this->myToolbar = myToolbar;
		init();
	}// LayoutManager constructor

	//Deconstructor
	~LayoutManager() {
		return;
	}//	LayoutManager Deconstructor

	LayerViewer* getLayerViewer() { return myLayerViewer; }

	//Gets the toolbar
	Toolbar* getToolbar() { return myToolbar; }

	//Calculates the size of each component of a window
	void calculateSizes();

	//Gives the size to each component
	void feedSizes();

	void updateLayout() {
		int width, height;
		myWindow->getSize(&width, &height);
	
		//LayerViewer Scalar
		SDL_FRect* imageView = new SDL_FRect();

		imageView->x = layerXPercent * width;
		imageView->y = layerYPercent * height;
		imageView->w = std::round(layerWidthToPercent * width - layerViewerX);
		imageView->h = std::round(layerHeightToPercent * height - layerViewerY);

		myLayerViewer->setViewPort(imageView);

		//Toolbar Scalar
		SDL_FRect* toolbarView = new SDL_FRect();

		toolbarView->x = layerViewerW;
		toolbarView->y = 0;
		toolbarView->w = std::round(width - toolbarX);
		toolbarView->h = std::round(height - toolbarY);

		myToolbar->setRenderArea(toolbarView);		
	}

	//Clears the window
	void clearWindow() { myWindow->clearWindow(); }	

	//Renders the elements
	void renderElements() {
		myLayerViewer->renderLayers();
		myToolbar->renderToolBar();
	}

	//Updates the layers of the layer viewer
	void updateLayers() { myLayerViewer->updateLayers(); }

	//Forces the window to render
	void renderWindow() { myWindow->render(); }

	void forceRenderAll() {
		updateLayers();
		renderElements();
		renderWindow();
	}
};


#endif