



#include "LayoutManager.h"


//Calculates the size of each aspect of a window
void LayoutManager::calculateSizes() {
	int width, height;
	myWindow->getSize(&width, &height);

	//Image Viewer Scaler
	layerViewerX = layerXPercent * width;
	layerViewerY = layerYPercent * height;

	layerViewerW = std::round(layerWidthToPercent * width - layerViewerX);
	layerViewerH = std::round(layerHeightToPercent * height - layerViewerY);

	//Toolbar Viewer Scalar
	toolbarX = layerViewerW;
	toolbarY = 0;

	toolbarW = std::round(width - toolbarX);
	toolbarH = std::round(height - toolbarY);
}


//Gives the size to each component
void LayoutManager::feedSizes() {
	SDL_FRect* imageView = new SDL_FRect();
	imageView->x = layerViewerX;
	imageView->y = layerViewerY;
	imageView->w = layerViewerW;
	imageView->h = layerViewerH;
	myLayerViewer->setViewPort(imageView);

	SDL_FRect* toolbarView = new SDL_FRect();
	toolbarView->x = toolbarX;
	toolbarView->y = toolbarY;
	toolbarView->w = toolbarW;
	toolbarView->h = toolbarH;
	myToolbar->setRenderArea(toolbarView);
}