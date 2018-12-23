#include "LoadWindow.h"
#include "../Chess.h"
#include <SDL_video.h>
#include <stdio.h>
#include <string.h>


//destroys all load window widgets
void destroyLoadWindowWidgets(Widget** widgets){
	for (int i=0; i<LOAD_WIDGETS_NUMBER; i++){
		if (widgets[i] != NULL){
			widgets[i]->destroyWidget(widgets[i]);
		}
	}
	free(widgets);
}


//creates buttons to be used in the load window
Widget** createLoadWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) return NULL; //argument isn't a valid renderer
	Widget** widgets = calloc(LOAD_WIDGETS_NUMBER, sizeof(Widget*));
	if (widgets == NULL ) return NULL ; //memory allocation error

	SDL_Rect slot1Rec = { .x =BUTTON_SHIFT, .y = 10, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot2Rec  = { .x =BUTTON_SHIFT, .y = 120, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot3Rec = { .x = BUTTON_SHIFT, .y = 230, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot4Rec = { .x = BUTTON_SHIFT, .y = 340, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot5Rec = { .x = BUTTON_SHIFT, .y = 450, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect backRec = { .x = BUTTON_SHIFT, .y = 560, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect loadRec = { .x = BUTTON_SHIFT, .y = 670, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	widgets[0] = createButton("./guiBMPs/load/slot1.bmp", "./guiBMPs/load/slot1p.bmp", renderer, &slot1Rec);
	widgets[1] = createButton("./guiBMPs/load/slot2.bmp", "./guiBMPs/load/slot2p.bmp", renderer, &slot2Rec);
	widgets[2] = createButton("./guiBMPs/load/slot3.bmp", "./guiBMPs/load/slot3p.bmp", renderer, &slot3Rec);
	widgets[3] = createButton("./guiBMPs/load/slot4.bmp", "./guiBMPs/load/slot4p.bmp", renderer, &slot4Rec);
	widgets[4] = createButton("./guiBMPs/load/slot5.bmp", "./guiBMPs/load/slot5p.bmp", renderer, &slot5Rec);
	widgets[5] = createButton("./guiBMPs/load/back.bmp", NULL, renderer, &backRec);
	widgets[6] = createButton("./guiBMPs/load/load.bmp", "./guiBMPs/load/loadp.bmp", renderer, &loadRec);
	for (int i=0; i<LOAD_WIDGETS_NUMBER; i++){
		if (widgets[i] == NULL){
			printf("error creating load window widget number %d",i);
			destroyLoadWindowWidgets(widgets);
			break;
		}
	}
	return widgets;
}


//destroys all the data that is associated with the given load window
void destroyLoadWindow(LoadWin* window) {
	if (window == NULL) return; //argument isn't a valid load window

	destroyLoadWindowWidgets(window->widgets);
	if (window->renderer != NULL ) SDL_DestroyRenderer(window->renderer);
	if (window->window != NULL ) SDL_DestroyWindow(window->window);
	free(window);
}


//creates a load window
LoadWin* createLoadWindow(){
	LoadWin* retWin = NULL;
	SDL_Surface* loadingSurface = NULL; //temporary
	retWin = (LoadWin*) calloc(sizeof(LoadWin), sizeof(char));
	if (retWin == NULL) return NULL; //memory allocation error

	//creates the window itself
	retWin->window = SDL_CreateWindow(/*title-*/ "Load",
					/*x pos-*/ SDL_WINDOWPOS_CENTERED, /*y pos-*/ SDL_WINDOWPOS_CENTERED,
					/*width-*/ 1240,	/*height-*/ 864, /*flags-*/ SDL_WINDOW_OPENGL);

	if (retWin->window == NULL ) { //error creating window
		destroyLoadWindow(retWin);
		printf("Error creating window: %s\n", SDL_GetError());
		return NULL ;
	}
	retWin->renderer = SDL_CreateRenderer(retWin->window, -1,SDL_RENDERER_ACCELERATED);
	if (retWin->renderer == NULL ) { //error creating renderer
		destroyLoadWindow(retWin);
		printf("Error creating renderer: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./guiBMPs/Load/mainmenu_back.bmp");
	if (loadingSurface == NULL){ //error creating surface
		printf("Error creating surface: %s\n", SDL_GetError());
		destroyLoadWindow(retWin);
		return NULL ;
	}
	retWin->bgTexture=SDL_CreateTextureFromSurface(retWin->renderer, loadingSurface);
	if (retWin->bgTexture == NULL){ //error creating texture from surface
		printf("Error creating texture: %s\n", SDL_GetError());
		destroyLoadWindow(retWin);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	retWin->widgets = createLoadWindowWidgets(retWin->renderer);
	if (retWin->widgets == NULL ) { //error creating widgets
		destroyLoadWindow(retWin);
		printf("Error creating widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	retWin->numOfWidgets = LOAD_WIDGETS_NUMBER;
	retWin->isFirst = true;
	retWin->fileNum = 1;
	retWin->isLoadLegal = false;
	return retWin;
}


//draws the given load window
void drawLoadWindow(LoadWin* window) {
	if (window == NULL) return; //argument isn't a valid load window struct

	SDL_Rect rec = {.x=0,.y=0,.w=1240,.h=864 };
	SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
	SDL_RenderClear(window->renderer);
	SDL_RenderCopy(window->renderer, window->bgTexture, NULL, &rec);
	((Button*) window->widgets[0]->data)->isActive = false;
	((Button*) window->widgets[1]->data)->isActive = false;
	((Button*) window->widgets[2]->data)->isActive = false;
	((Button*) window->widgets[3]->data)->isActive = false;
	((Button*) window->widgets[4]->data)->isActive = false;
	((Button*) window->widgets[window->fileNum-1]->data)->isActive = true;
	((Button*) window->widgets[6]->data)->isActive = false;
	if (window->isLoadLegal == true){
		((Button*) window->widgets[6]->data)->isActive = true;
	}
	for (int i=0; i < window->numOfWidgets; i++) {
		(window->widgets[i])->drawWidget(window->widgets[i]);
	}
	SDL_RenderPresent(window->renderer);
}


//gets the file path
char* getFilePath(LoadWin* window){
	char* path = (char*)calloc(FILES_PATH_LENGTH,sizeof(char));
	char name[2];
	strcpy(path, FILES_PATH);
	name[0] = (char)(window->fileNum+48);
	name[1] = '\0';
	strcat(path, name);
	return path;
}


//gets the file path and checks if it's valid
void legalPath(LoadWin* window){
	char* path = getFilePath(window);
	FILE* file = fopen(path,"r");
	if (file == NULL){ //error opening file
		printf("the file path is illegal\n");
		window->isLoadLegal = false;
	}
	else{
		window->isLoadLegal = true;
	}
}


//handles load window events, given a window and an event
LOAD_EVENT handleLoadWindowEvent(LoadWin* window, SDL_Event* event) {
	if (!event) return LOAD_EVENT_INVALID_ARGUMENT; //argument isn't a valid event

	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (window->widgets[0]->handleWidgetEvent(window->widgets[0],event) == 1) {
			window->fileNum = 1;
			legalPath(window);
			return LOAD_EVENT_UPDATE;
		} else if (window->widgets[1]->handleWidgetEvent(window->widgets[1],event) == 1){
			window->fileNum = 2;
			legalPath(window);
			return LOAD_EVENT_UPDATE;
		}else if (window->widgets[2]->handleWidgetEvent(window->widgets[2],event) == 1){
			window->fileNum = 3;
			legalPath(window);
			return LOAD_EVENT_UPDATE;
		}else if (window->widgets[3]->handleWidgetEvent(window->widgets[3],event) == 1){
			window->fileNum = 4;
			legalPath(window);
			return LOAD_EVENT_UPDATE;
		}else if (window->widgets[4]->handleWidgetEvent(window->widgets[4],event) == 1){
			window->fileNum = 5;
			legalPath(window);
			return LOAD_EVENT_UPDATE;
		}else if (window->widgets[5]->handleWidgetEvent(window->widgets[5],event) == 1)  {
			return LOAD_EVENT_BACK;
		}else if (window->widgets[6]->handleWidgetEvent(window->widgets[6],event) == 1){
			if (window->isLoadLegal == true){
				return LOAD_EVENT_LOAD; //start
			}
			return LOAD_EVENT_UPDATE;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return LOAD_EVENT_EXIT;
		}
		break;
	default:
		return LOAD_EVENT_NONE;
	}
	return LOAD_EVENT_NONE;
}

