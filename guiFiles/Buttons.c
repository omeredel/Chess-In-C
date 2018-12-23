#include <stdlib.h>
#include <stdio.h>
#include "Buttons.h"


//destroys all the data that is associated with the button widget b
void destroyButton(Widget* b) {
	if (b == NULL) return; //argument isn't a valid widget

	Button* castData = (Button*) b->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	SDL_DestroyTexture(castData->buttonPressedTexture);
	free(castData);
	free(b);
}


//copies the Rect srcRec and returns the copy
SDL_Rect* copyRec(SDL_Rect* srcRec){
	if(srcRec == NULL) return NULL; //argument isn't a valid rect

	SDL_Rect* recCopy = malloc(sizeof(SDL_Rect));
	if (recCopy == NULL) return NULL; //memory allocation error

	recCopy->h = srcRec->h;
	recCopy->w = srcRec->w;
	recCopy->x = srcRec->x;
	recCopy->y = srcRec->y;

	return recCopy;
}


//creates a button widget given the necessary arguments
Widget* createButton(const char* buttonImage, const char* buttonPressedImage, SDL_Renderer* renderer, SDL_Rect* location){
	if (buttonImage == NULL || location == NULL || renderer == NULL) return NULL ; //invalid arguments

	//allocates memory
	Widget* res = (Widget*) malloc(sizeof(Widget));
	Button* data = (Button*) malloc(sizeof(Button));
	SDL_Surface* loadingSurface = SDL_LoadBMP(buttonImage); //temporary
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);

	if (buttonPressedImage == NULL){
		data->buttonPressedTexture = NULL;
		data->isActive=true;
	}
	else{
		SDL_Surface* loadingPressedSurface = SDL_LoadBMP(buttonPressedImage); //temporary
		SDL_Texture* buttonPressedTexture = SDL_CreateTextureFromSurface(renderer,loadingPressedSurface);
		SDL_FreeSurface(loadingPressedSurface); //we are done with the surface
		data->buttonPressedTexture = buttonPressedTexture;
		data->isActive = false;
		if (buttonPressedTexture == NULL){
			free(res);
			free(data);
			SDL_FreeSurface(loadingSurface);
			SDL_DestroyTexture(buttonTexture);
			return NULL ;
		}
	}
	if (loadingSurface == NULL || buttonTexture == NULL || res == NULL || data == NULL){
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(buttonTexture);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //we are done with the surface
	data->buttonTexture = buttonTexture;
	data->location = copyRec(location);
	data->windowRenderer = renderer;
	data->isShown = true;
	res->destroyWidget = destroyButton;
	res->drawWidget = drawButton;
	res->handleWidgetEvent = handleButtonEvent;
	res->data = data;
	return res;
}


//draws the button given a button widget b
void drawButton(Widget* b) {
	if (b == NULL)return; //argument isn't a valid widget

	Button* drawButt = (Button*) b->data;
	if (drawButt->isShown == true){
		if (drawButt->buttonPressedTexture == NULL){
			SDL_RenderCopy(drawButt->windowRenderer, drawButt->buttonTexture, NULL,	drawButt->location);
		}
		else{
			if (drawButt->isActive == true){
				SDL_RenderCopy(drawButt->windowRenderer, drawButt->buttonTexture, NULL,	drawButt->location);
			}
			else{
				SDL_RenderCopy(drawButt->windowRenderer, drawButt->buttonPressedTexture, NULL,	drawButt->location);
			}
		}

	}
}


//handles the event, given an event and a button widget b
int handleButtonEvent(Widget* b, SDL_Event* event) {
	if (b == NULL || event == NULL) return 0; //error value, arguments not valid

	Button* castData = (Button*) b->data;
	if(castData->isShown == false) return 0; //button is not shown
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			return 1;
		}
	}
	return 0;
}


//destroys all the data that is associated with the piece widget b
void destroyPiece(Widget* src) {
	if (src == NULL ) return; //invalid argument

	Piece* castData = (Piece*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	free(castData);
	free(src);
}


//creates a piece widget given the necessary arguments
Widget* createPiece(const char* image, SDL_Renderer* renderer, SDL_Rect* location) {
	if (image == NULL || renderer == NULL || location == NULL) return NULL ; //invalid arguments

	//allocates memory
	Widget* res = (Widget*) malloc(sizeof(Widget));
	Piece* data = (Piece*) malloc(sizeof(Piece));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //temporary
	SDL_SetColorKey(loadingSurface, SDL_TRUE,SDL_MapRGB(loadingSurface->format, 255, 0, 255));
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL|| buttonTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(buttonTexture);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //we are done with the surface
	data->buttonTexture = buttonTexture;
	data->location = copyRec(location);
	data->windowRenderer = renderer;
	data->isShown = true;
	res->destroyWidget = destroyPiece;
	res->drawWidget = drawPiece;
	res->handleWidgetEvent = handlePieceEvent;
	res->data = data;
	return res;
}


//draws the piece given a piece widget p
void drawPiece(Widget* p) {
	if (p == NULL ) return; //argument is not a valid widget

	Piece* castData = (Piece*) p->data;
	if(castData->isShown==true){ //draws the piece only if it's supposed to be shown
		SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
	}
}


//handles the event, given an event and a piece widget p
int handlePieceEvent(Widget* p, SDL_Event* event) {
	if (p == NULL || event == NULL) return 0; //invalid arguments

	Piece* castData = (Piece*) p->data;
	if (castData->isShown == false) return 0; //piece is not shown

	if (event->type==SDL_MOUSEMOTION||event->type==SDL_MOUSEBUTTONDOWN||event->type==SDL_MOUSEBUTTONUP){
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			return 1;
		}
	}
	return 0;
}
