



#include "LayoutManager.h"


//Calculates the size of each aspect of a window
void LayoutManager::calculateSizes() {
	//double layerViewerX, layerViewerY;
	//double layerViewerWidth, layerViewerHeight;

	int width, height;
	myWindow->getSize(&width, &height);

	//Image Viewer Scaler
	layerViewerX = layerXPercent * width;
	layerViewerY = layerYPercent * height;

	layerViewerW = layerWidthToPercent * width - layerViewerX;
	layerViewerH = layerHeightToPercent * height - layerViewerY;

	//Toolbar Viewer Scalar
	toolbarX = layerViewerW;
	toolbarY = 0;

	toolbarW = width - toolbarX;
	toolbarH = height - toolbarY;
}


//Gives the size to each component
void LayoutManager::feedSizes() {
	ViewPort imageView;
	imageView.x = layerViewerX;
	imageView.y = layerViewerY;
	imageView.w = layerViewerW;
	imageView.h = layerViewerH;
	myLayerViewer->setViewPort(imageView);

	ViewPort toolbarView;
	toolbarView.x = toolbarX;
	toolbarView.y = toolbarY;
	toolbarView.w = toolbarW;
	toolbarView.h = toolbarH;
	myToolbar->setRenderArea(toolbarView);

}