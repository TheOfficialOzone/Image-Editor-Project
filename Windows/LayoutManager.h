#pragma once

#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include "SDL.h"
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

	//Default stuff that happens at the start
	void init() {
		layerXPercent = 0.0;
		layerYPercent = 0.0;
		layerWidthToPercent = 0.8;
		layerHeightToPercent = 1.0;

		calculateSizes();
	}

	//Calculates the size of each component of a window
	void calculateSizes();

	//Gives the size to each component
	void feedSizes();



};


#endif