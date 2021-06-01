


#include "Toolbar.h"

//Sets the buttons positions
void Toolbar::setButtonPositions() {
	float widthPer = renderArea->w / 2.0;
	float heightPer = renderArea->h / (myInteracts.size() / 2.0);
	for (int i = 0; i < myInteracts.size(); i++) {
		SDL_FRect* temp = new SDL_FRect();
		temp->x = (i % 2) * widthPer + renderArea->x;
		temp->y = (i / 2) * heightPer + renderArea->y;
		temp->w = widthPer;
		temp->h = heightPer;

		myInteracts[i]->setArea(temp);
	}
}// setButtonPositions


//Renders the toolBar
void Toolbar::renderToolBar() {
	renderToolbarButtons();

	SDL_SetRenderDrawColor(renderTo, 255, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRectF(renderTo, renderArea);
}// renderToolBar


void Toolbar::renderToolbarButtons() {
	for (int i = 0; i < myInteracts.size(); i++) {
		myInteracts[i]->render();
	}
}

