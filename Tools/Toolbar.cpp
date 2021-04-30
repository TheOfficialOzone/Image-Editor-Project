


#include "Toolbar.h"

//Renders the toolBar
void Toolbar::renderToolBar() {
	SDL_SetRenderDrawColor(renderTo, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderTo, &viewArea);

	std::cout << viewArea.x << " " << viewArea.y << std::endl;
}// renderToolBar