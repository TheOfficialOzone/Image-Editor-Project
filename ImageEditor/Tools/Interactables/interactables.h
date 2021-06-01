#pragma once


#ifndef INTERACT
#define INTERACT


#include <iostream>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"	//For loading images

#include "Layers/LayerViewer.h"


//Struct for what the user has
class UserInfoCache {
private:
	LayerViewer* myViewer;
	SDL_Color myColour;

	int layerID = 0;	//what layer the user is on
	int brushSize = 5;	//The Size of the brush

public:
	UserInfoCache(LayerViewer* newLayerViewer, SDL_Color newColour) {
		myViewer = newLayerViewer;
		myColour = newColour;
	}

	//Changes the layer, only works if in valid range
	bool changeLayerID(int changeAmount) {
		if (layerID + changeAmount >= 0 && layerID + changeAmount < myViewer->getLayerAmount()) {
			layerID += changeAmount;
			return true;
		}
		return false;
	}

	//Sets the layer ID, returns true if it's a legal change
	bool setLayerID(int newLayerID) {
		if (newLayerID >= 0 && newLayerID < myViewer->getLayerAmount()) {
			layerID = newLayerID;
			return true;
		} 
		return false;
	}

	//Gets the layerID from the cache
	int getLayerID() {
		return layerID;
	}

	//Gets the size of the brush
	int getBrushSize() { return brushSize; }

	void setBrushSize(int newBrushSize) { brushSize = newBrushSize; }
	//Sets the colour of the cache
	void setColour(SDL_Color newColour) {
		myColour = newColour;
	}

	//Gets the colour from the userCache
	SDL_Color getColour() {
		return myColour;
	}
};

//The different operations that are available
enum operationIDs {
	Brush,
	Eraser,
	UpLayer,
	DownLayer,
	ChangeColour,
	ChangeHue,
	ChangeBrushSize,
};


//Abstract class used for clicking and what not
class Interactable {
protected:
	SDL_Renderer* renderTo;	//What will render to this window
	SDL_FRect* renderArea;	//Where this will be rendered

	static inline long idGenerator = 0;
	long id;
	int operationID;

	double x, y;	//Position
	double w, h;	//Size

	bool isClicked, isClickable;
	bool isDragging, isDraggable;
	bool isToggled, isToggleable;
	bool isScrolled, isScrollable;

	void initInteractable() {
		id = idGenerator++;
	}
public:
	//To construct an Interactable
	Interactable(SDL_Renderer* newRenderer) {
		initInteractable();
		renderTo = newRenderer;
	}

	//Constructor with an Area
	Interactable(SDL_Renderer* newRenderer, SDL_FRect* newArea) {
		initInteractable();
		renderTo = newRenderer;
		renderArea = newArea;
	}

	~Interactable() {
		delete renderArea;
	}

	virtual bool checkIfClickInBounds(double clickX, double clickY) {
		return (clickX > renderArea->x && clickX - renderArea->x < renderArea->w&& clickY > renderArea->y && clickY - renderArea->y < renderArea->h);
	};	
	virtual void setArea(SDL_FRect* newArea) {
		SDL_FRect* storeForDelete = renderArea;
		renderArea = newArea;
		delete storeForDelete;
		return;
	}	//Where the Interactable will go
	virtual void render() {return;}	//Renders the Interactable		STUB	


	virtual void actionClick(void* firstInfo, void* secondInfo) {return;}	//Called when clicked
	virtual void actionScroll(int scrollValue) {return;}	//Called when scrolled over

	//Gets the buttons potential attributes
	bool getClickable() {return isClickable;}
	bool getDraggable() {return isDraggable;}
	bool getToggleable() {return isToggleable;}
	bool getScrollable() {return isScrollable;}

	//Defaults the ID Generator to 0
	static void resetIDGenerator() {
		idGenerator = 0;
	}

	//Set the operation ID
	void setOperationID(int opID) {
		operationID = opID;
	}

	//Get the operationID
	int getOperationID() {
		return operationID;
	}
};

//Displays a button that can be clicked, just give it an operationID
class InteractButton : public Interactable{
private:
	bool hasTexture;

	SDL_Texture* myTexture;	//The Texture
	double textureAspectRatio;	//The Aspect ratio of the texture

	SDL_FRect outlineRect;
	SDL_FRect textureRenderRect;

	
	//Initializing a default button
	void initButton() {
		isClicked = false;
		isClickable = true;

		isDragging = false;
		isDraggable = false;

		isToggled = false;
		isToggleable = false;
	}
	
	void isNowTexture() {
		hasTexture = true;
	}
public:
	
	//Constructor
	InteractButton(SDL_Renderer* renderer, std::string imageName) : Interactable(renderer) {
		initButton();
		setTexture(renderer, imageName);
	}// Constructor

	//Constructor
	InteractButton(SDL_Renderer* renderer) : Interactable(renderer) {
		initButton();
	}// Constructor

	//Decontructor
	~InteractButton() {
		SDL_DestroyTexture(myTexture);
	}// Deconstructor

	bool setTexture(SDL_Renderer* tempRender, std::string imageName) {
		bool worked = false;
		
		SDL_Surface* tempSurface = IMG_Load(imageName.c_str());	//Loads the surface from the file
		if (tempSurface != NULL) {	//Initializes the texture if it loaded
			worked = true;
			myTexture = SDL_CreateTextureFromSurface(tempRender, tempSurface);
			isNowTexture();

			int width = 0, height = 0;
			SDL_QueryTexture(myTexture, NULL, NULL, &width, &height);
			textureAspectRatio = (double)width / height;
		}
		
		SDL_FreeSurface(tempSurface);	//Destroys the surface
		return worked;
	}

	//Sets the Area of the button
	void setArea(SDL_FRect* newArea) {
		renderArea = newArea;

		outlineRect.x = newArea->x + 1;
		outlineRect.y = newArea->y + 1;
		outlineRect.w = newArea->w - 2;
		outlineRect.h = newArea->h - 2;

		if (hasTexture) {
			if (renderArea->w > renderArea->h * textureAspectRatio) {	//Cap on height
				textureRenderRect.h = renderArea->h;
				textureRenderRect.w = renderArea->h * textureAspectRatio;
			} else {	//Cap on Width
				textureRenderRect.w = renderArea->w;
				textureRenderRect.h = renderArea->w / textureAspectRatio;
			}
			textureRenderRect.x = renderArea->x + (renderArea->w / 2.0) - (textureRenderRect.w / 2.0);
			textureRenderRect.y = renderArea->y + (renderArea->h / 2.0) - (textureRenderRect.h / 2.0);
		}
	}// setArea

	//Renders the button
	void render() {
		if (!hasTexture) {
			SDL_SetRenderDrawColor(renderTo, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRectF(renderTo, renderArea);

			SDL_SetRenderDrawColor(renderTo, 255, 255, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRectF(renderTo, renderArea);
		} else {
			SDL_RenderCopyF(renderTo, myTexture, NULL, &textureRenderRect);	//Copies the image to the renderer

			SDL_SetRenderDrawColor(renderTo, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRectF(renderTo, &outlineRect);
		}
	}// render
};

//Displays a static colour
class InteractDisplayColour : public Interactable {
private:
	SDL_Color myColour;	//The Colour displayed by this

	//initializes the default settings for a display
	void initDisplayColour() {
		isClicked = false;
		isClickable = true;

		isDragging = false;
		isDraggable = false;

		isToggled = false;
		isToggleable = false;
	}

public:
	InteractDisplayColour(SDL_Renderer* renderer, SDL_FRect* newRenderArea, SDL_Color newColour) : Interactable(renderer) {
		initDisplayColour();
		myColour = newColour;
		setArea(newRenderArea);
	}

	InteractDisplayColour(SDL_Renderer* renderer, SDL_Color newColour) : Interactable(renderer) {
		initDisplayColour();
		myColour = newColour;
	}

	//Sets the colour of the display
	void setColour(SDL_Color newColour) {
		myColour = newColour;
	}

	//Gets the colour from the display
	SDL_Color getColour() {return myColour;}

	//Renders the colour
	void render() {
		SDL_SetRenderDrawColor(renderTo, myColour.r, myColour.g, myColour.b, myColour.a);
		SDL_RenderFillRectF(renderTo, renderArea);
	}
};

//Displays
class InteractBar : public Interactable {
private:
	float fillPercent;
	SDL_Color myColour;
	UserInfoCache* myCache;
	SDL_FRect* fillRect = new SDL_FRect();

	void initInteractBar() {
		isClicked = false;
		isClickable = true;

		isDragging = false;
		isDraggable = false;

		isToggled = false;
		isToggleable = false;
	}

public:
	InteractBar(SDL_Renderer* renderer, UserInfoCache* newCache, SDL_Color newColour, SDL_FRect* newRect) : Interactable(renderer, newRect) {
		initInteractBar();
		setColour(newColour);
		setUserCache(newCache);
	}

	InteractBar(SDL_Renderer* renderer, UserInfoCache* newCache, SDL_Color newColour) : Interactable(renderer) {
		initInteractBar();
		setColour(newColour);
		setUserCache(newCache);
	}

	//What happens when clicked (Gives a colour to the UserCache)
	void actionClick(void* clickX, void* clickY) {
		float mouseX = *(int*)clickX - renderArea->x;	//Gets the relavent coords in the button
		float mouseY = *(int*)clickY - renderArea->y;

		float xPercent = mouseX / renderArea->w;	//Where is it within the button (X wise)
		float yPercent = mouseY / renderArea->h;

		setFillPercent(xPercent);
	}

	//Called when scrolled over
	void actionScroll(int scrollValue) {
		changeFillPercent(scrollValue / 100.f);
	}

	//Sets the userCache
	void setUserCache(UserInfoCache* newCache) { myCache = newCache; }

	//Sets the Area of the button
	void setArea(SDL_FRect* newArea) {
		renderArea = newArea;

		fillRect->x = renderArea->x;
		fillRect->y = renderArea->y;
		fillRect->h = renderArea->h;

		fillRect->w = renderArea->w * fillPercent;
	}// setArea

	//Sets the percent of how filled the bar is
	void setFillPercent(float newPercent) {
		fillPercent = newPercent;
		if (fillRect != nullptr && renderArea != nullptr)
			fillRect->w = renderArea->w * fillPercent;
	}// setFillPercent

	//Sets the colour of the Bar
	void setColour(SDL_Color newColour) { myColour = newColour; }

	//Sets the percent of how filled the bar is relative to the current value
	void changeFillPercent(float changePercent) {
		float newPercent = fillPercent + changePercent;
		newPercent = (newPercent > 1.0f ? 1.0f : newPercent);	//Keeps the value within 0 to 1
		newPercent = (newPercent < 0.0f ? 0.0f : newPercent);

		setFillPercent(newPercent);
	}// changeFillPercent

	//Gets the fillPercent
	float getFillPercent() { return fillPercent; }

	//Gets the UserCache
	UserInfoCache* getUserCache() { return myCache; }


	void render() {
		SDL_SetRenderDrawColor(renderTo, myColour.r, myColour.g, myColour.b, myColour.a);
		SDL_RenderDrawRectF(renderTo, renderArea);

		SDL_RenderFillRectF(renderTo, fillRect);	//Draws 
	}
};

//Displays the colours that may be selected
class InteractColourSelector : public Interactable {
private:
	const int displaySize = 300;	//Size of the display

	SDL_Texture* allColoursTexture;
	SDL_Surface* allColoursSurface;
	SDL_Color myColour;
	UserInfoCache* myCache;

	float xSelection = 0;
	float ySelection = 0;
	float widthAtSelection = 0;
	float heightAtSelection = 0;
	SDL_FRect selectionRect;

	void initColourSelector() {
		isClicked = false;
		isClickable = true;

		isDragging = false;
		isDraggable = false;

		isToggled = false;
		isToggleable = false;
	}

	//Gets the colour at a coordinate
	SDL_Color generateColourAtPoint(float x, float y) {
		float xPercent = x / renderArea->w;	//Where is it within the button (X wise)
		float yPercent = y / renderArea->h;

		SDL_Color newColour = generateColourAtPercent(xPercent, yPercent);
		return newColour;
	}

	//Gets the colour at the percent 
	SDL_Color generateColourAtPercent(float xPercent, float yPercent) {
		float invYPercent = 1 - yPercent;

		SDL_Color newColour;
		newColour.r = (myColour.r * xPercent + (255.f) * (1 - xPercent));
		newColour.g = (myColour.g * xPercent + (255.f) * (1 - xPercent));
		newColour.b = (myColour.b * xPercent + (255.f) * (1 - xPercent));

		newColour.r *= invYPercent;
		newColour.g *= invYPercent;
		newColour.b *= invYPercent;
		newColour.a = SDL_ALPHA_OPAQUE;

		return newColour;
	}

	SDL_Texture* deleteTexture;
public:
	InteractColourSelector(SDL_Renderer* renderer, SDL_Color myColour, UserInfoCache* newCache) : Interactable(renderer) {
		allColoursSurface = SDL_CreateRGBSurface(0, displaySize, displaySize, 32, 0, 0, 0, 0);
		allColoursSurface = SDL_ConvertSurfaceFormat(allColoursSurface, SDL_PIXELFORMAT_RGBA32, 0);	//Converts to this pixel format
		allColoursTexture = SDL_CreateTextureFromSurface(renderer, allColoursSurface);
		updateViewerToColour(myColour);

		myCache = newCache;
	}

	//Updates the display to the colour fed in
	void updateViewerToColour(SDL_Color colour) {
		SDL_LockSurface(allColoursSurface);	//Can now safely access pixels
		Uint32* pixels = (Uint32*)allColoursSurface->pixels;	//Gets the pixels

		myColour = colour;
		int hADJ = displaySize;

		for (int y = 0; y < displaySize; y++) {
			for (int x = 0; x < displaySize; x++) {
				Uint32 dColor;
				float rValue, gValue, bValue;

				float saturation = (((float)displaySize - x) / displaySize);	//0 is full Saturation --- 1 is no Sat
				float value = (((float)displaySize - y) / displaySize);
				float downLeftBias = saturation * value;

				rValue = myColour.r * value + (255.f - myColour.r) * downLeftBias;
				gValue = myColour.g * value + (255.f - myColour.g) * downLeftBias;
				bValue = myColour.b * value + (255.f - myColour.b) * downLeftBias;
				dColor = SDL_MapRGBA(allColoursSurface->format, rValue, gValue, bValue, SDL_ALPHA_OPAQUE);

				pixels[x + hADJ * y] = dColor;
			}
		}


		SDL_DestroyTexture(deleteTexture);
		deleteTexture = allColoursTexture;
		SDL_Texture* switchTexture = SDL_CreateTextureFromSurface(renderTo, allColoursSurface);
		allColoursTexture = switchTexture;

		std::cout << deleteTexture << " " << allColoursTexture << "\n";
		//SDL_DestroyTexture(deleteTexture);
		
		SDL_UnlockSurface(allColoursSurface);// Can no longer safely access pixels
	}

	//What happens when clicked (Gives a colour to the UserCache)
	void actionClick(void* clickX, void* clickY) {
		float mouseX = *(int*)clickX - renderArea->x;	//Gets the relavent coords in the button
		float mouseY = *(int*)clickY - renderArea->y;

		float xPercent = mouseX / renderArea->w;	//Where is it within the button (X wise)
		float yPercent = mouseY / renderArea->h;
		float invYPercent = 1 - yPercent;

		xSelection = mouseX;	//Tracks the colour you have selected
		ySelection = mouseY;
		widthAtSelection = renderArea->w;
		heightAtSelection = renderArea->h;

		SDL_Color newColour = generateColourAtPoint(mouseX, mouseY);
		myCache->setColour(newColour);	//Sets the colour
	}

	//Regenerates Colour
	void invalidateColour() {
		SDL_Color newColour = generateColourAtPoint(xSelection, ySelection);	//Gets the colour
		myCache->setColour(newColour);	//Sets the colour
	}

	//Renders the texture
	void render() {
		SDL_RenderCopyF(renderTo, allColoursTexture, NULL, renderArea);
		renderSelector();
	}

	//renders the little highlighter over the colors
	void renderSelector() {
		selectionRect.x = (renderArea->x + xSelection * (renderArea->w / widthAtSelection) - 3);	//Where the Rect will be drawn
		selectionRect.y = renderArea->y + ySelection * (renderArea->h / heightAtSelection) - 3;
		selectionRect.w = 6;
		selectionRect.h = 6;
		SDL_RenderDrawRectF(renderTo, &selectionRect);
	}
};

//Displays all the Hues
class InteractHueSelector : public Interactable {
private:
	const int displayWidth = 100;	//Width of the display
	const int displayHeight = 500;	//Height of the display

	SDL_Texture* allColoursTexture;
	SDL_Surface* allColoursSurface;

	float ySelection = 0;	//Tracks where the last colour selection was
	float widthAtSelection = 0;
	float heightAtSelection = 0;
	SDL_FRect selectionRect;

	InteractColourSelector* myColourSelector;	//What it will pass it's color to


	void initHueSelector() {
		initInteractable();

		isClicked = false;
		isClickable = true;

		isDragging = false;
		isDraggable = false;

		isToggled = false;
		isToggleable = false;
	}
public:
	InteractHueSelector(SDL_Renderer* renderer, InteractColourSelector* newColourSelector) : Interactable(renderer) {
		allColoursSurface = SDL_CreateRGBSurface(0, displayWidth, displayHeight, 32, 0, 0, 0, 0);
		allColoursSurface = SDL_ConvertSurfaceFormat(allColoursSurface, SDL_PIXELFORMAT_RGBA32, 0);	//Converts to this pixel format
		generateHueSelector(renderTo);
		setColourSelector(newColourSelector);
	}

	void actionClick(void* clickX, void* clickY) {
		float mouseX = *(int*)clickX - renderArea->x;	//Gets the relavent coords in the button
		float mouseY = *(int*)clickY - renderArea->y;

		float xPercent = mouseX / renderArea->w;	//Where is it within the button (X wise)
		float yPercent = mouseY / renderArea->h;
		float invYPercent = 1 - yPercent;

		ySelection = mouseY;
		widthAtSelection = renderArea->w;
		heightAtSelection = renderArea->h;

		float distDown = yPercent;
		float rValue, gValue, bValue;

		generateColourAtPos(distDown, &rValue, &gValue, &bValue);

		SDL_Color newColour;
		newColour.r = (Uint8)rValue;
		newColour.g = (Uint8)gValue;	//Green 
		newColour.b = (Uint8)bValue;	//Blue
		newColour.a = SDL_ALPHA_OPAQUE;

		myColourSelector->updateViewerToColour(newColour);	//Sets the colour
		myColourSelector->invalidateColour();
	}

	//Generates the Hue Selector
	void generateHueSelector(SDL_Renderer* renderTo) {
		SDL_LockSurface(allColoursSurface);	//Can now safely access pixels
		Uint32* pixels = (Uint32*)allColoursSurface->pixels;	//Gets the pixels

		int hADJ = displayWidth;
		for (int y = 0; y < displayHeight; y++) {
			for (int x = 0; x < displayWidth; x++) {
				Uint32 dColor;
				float rValue, gValue, bValue;
				float distDown = (float)y / displayHeight;

				generateColourAtPos(distDown, &rValue, &gValue, &bValue);
				dColor = SDL_MapRGBA(allColoursSurface->format, rValue, gValue, bValue, SDL_ALPHA_OPAQUE);

				pixels[x + hADJ * y] = dColor;
			}
		}

		SDL_UnlockSurface(allColoursSurface);
		SDL_Texture* tempTexture = allColoursTexture;
		allColoursTexture = SDL_CreateTextureFromSurface(renderTo, allColoursSurface);
		SDL_DestroyTexture(tempTexture);
	}

	void generateColourAtPos(float distDown, float* r, float* g, float* b) {
		float fadeRange = 1.f / 3.f;	//Fade distance

		float rPos = 0.f / 3.f, rPos2 = 1.f;	//Where they are on the screen
		float gPos = 2.f / 3.f;
		float bPos = 1.f / 3.f;

		*r = (1 - std::abs(distDown - rPos) * 3) * 255.f * 2.0f * (std::abs(distDown - rPos) < fadeRange);	//Top of screen Red
		*r += (1 - std::abs(distDown - rPos2) * 3) * 255.f * 2.0f * (std::abs(distDown - rPos2) < fadeRange);	//Adjusts for bottom of screen

		*g = (1 - std::abs(distDown - gPos) * 3) * 255.f * 2.0f * (std::abs(distDown - gPos) < fadeRange);	//Green
		*b = (1 - std::abs(distDown - bPos) * 3) * 255.f * 2.0f * (std::abs(distDown - bPos) < fadeRange);	//Blue

		*r = (*r > 255.f ? 255.f : *r);	//Keeps them within the range of 0 - 255
		*g = (*g > 255.f ? 255.f : *g);
		*b = (*b > 255.f ? 255.f : *b);
	}

	void setColourSelector(InteractColourSelector* newColourSelector) { myColourSelector = newColourSelector; }

	//Renders the texture
	void render() {
		SDL_RenderCopyF(renderTo, allColoursTexture, NULL, renderArea);
		renderSelector();
	}

	//renders the little highlighter over the colors
	void renderSelector() {
		selectionRect.x = (renderArea->x * (renderArea->w / widthAtSelection) - 1);	//Where the Rect will be drawn
		selectionRect.y = renderArea->y + ySelection * (renderArea->h / heightAtSelection) - 3;
		selectionRect.w = renderArea->w;
		selectionRect.h = 6;
		SDL_RenderDrawRectF(renderTo, &selectionRect);
	}
};


#endif