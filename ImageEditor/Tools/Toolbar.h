#pragma once


#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "SDL.h"
#include "Camera/Camera.h"
#include "Interactables/interactables.h"
#include <string>
#include <iostream>
#include <vector>


class Toolbar {
private:
	SDL_Renderer* renderTo;	//What will render to this window
	SDL_FRect* renderArea;	//The area it renders to

	std::vector<Interactable*> myInteracts;
public:

	Toolbar(SDL_Renderer* newRenderer, SDL_FRect* newArea) {
		renderTo = newRenderer;	//Renderer
		renderArea = newArea;
	}

	~Toolbar() {
		for (int i = 0; i < myInteracts.size(); i++) {
			delete myInteracts[i];
		}
		myInteracts.clear();
		delete renderArea;
	}

	///Gets where the Toolbar must be rendered
	void setRenderArea(SDL_FRect* newArea) {
		SDL_FRect* tempArea = renderArea;
		renderArea = newArea;
		delete tempArea;
		setButtonPositions();
	}

	void setButtonPositions();	//Sets the buttons positions

	void renderToolBar();	//Renders the Tool bar

	//Adds multiple Interactables to the list
	void addInteractable(Interactable** newInteractables, int amount) {
		for (int i = 0; i < amount; i++) {
			myInteracts.push_back(newInteractables[i]);
		}
	}

	//Adds an interactable to the list
	void addInteractable(Interactable* newInteractables) {
		myInteracts.push_back(newInteractables);
	}

	//removes an interactable from the list
	void removeInteractable(int index) {
		delete myInteracts[index];
		myInteracts.erase(myInteracts.begin() + index);
	}

	//Gets the vector of all the interactables
	std::vector<Interactable*>* getInteractables() {
		return &myInteracts;
	}

private:
	void renderToolbarButtons();	//Renders the Tool bar's buttons
};

#endif
