


#include "Window.h"	//	Header file



//Gets the windows renderer
SDL_Renderer* Window::getRenderer() {
	return renderer;
}// getRenderer

//Removes everything displayed on the renderer
//To White
void Window::clearWindow() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);	//White
	SDL_RenderClear(renderer);	//Clears to white
}//	clearWindow

//Renders the Window
void Window::render() {
	SDL_RenderPresent(renderer);
}//	render

