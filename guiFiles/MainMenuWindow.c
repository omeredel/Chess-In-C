#include "MainMenuWindow.h"
#include <SDL_video.h>
#include <stdio.h>
#include <stdlib.h>


//destroys all main menu window widgets
void destroyMainMenuWindowWidgets(Widget** widgets){
	for (int i=0; i<MAIN_MENU_WIDGETS_NUMBER; i++){
		if (widgets[i] != NULL){
			widgets[i]->destroyWidget(widgets[i]);
		}
	}
	free(widgets);
}


//creates the buttons to be used in the main menu window;
Widget** createMainMenuWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) return NULL ; //argument isn't a valid renderer
	Widget** widgets = calloc(MAIN_MENU_WIDGETS_NUMBER+1, sizeof(Widget*));
	if (widgets == NULL ) return NULL ; //memory allocation error

	SDL_Rect newgameRec = { .x =BUTTON_SHIFT, .y = 40, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect loadRec = { .x = BUTTON_SHIFT, .y = 150, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect exitRec = { .x = BUTTON_SHIFT, .y = 260, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	widgets[0] = createButton("./guiBMPs/MainMenu/newgame.bmp", "./guiBMPs/MainMenu/newgame.bmp", renderer, &newgameRec);
	widgets[1] = createButton("./guiBMPs/MainMenu/load.bmp", "./guiBMPs/MainMenu/load.bmp", renderer, &loadRec);
	widgets[2] = createButton("./guiBMPs/MainMenu/exit.bmp", "./guiBMPs/MainMenu/exit.bmp", renderer, &exitRec);

	for (int i=0; i<MAIN_MENU_WIDGETS_NUMBER; i++){
		if (widgets[i] == NULL){
			printf("error creating main window widget number %d",i);
			destroyMainMenuWindowWidgets(widgets);
			break;
		}
	}
	return widgets;
}


//destroys all the data that is associated with the given main menu window
void destroyMainMenuWindow(MainWin* window) {
	if (window == NULL) return; //argument isn't a valid main window struct

	destroyMainMenuWindowWidgets(window->widgets);
	if (window->bgTexture != NULL) SDL_DestroyTexture(window->bgTexture);
	if (window->renderer != NULL ) SDL_DestroyRenderer(window->renderer);
	if (window->window != NULL ) SDL_DestroyWindow(window->window);
	free(window);
}


//creates a main menu window
MainWin* createMainMenuWindow() {
	MainWin* retWin = NULL;
	SDL_Surface* loadingSurface = NULL; //temporary
	retWin = (MainWin*) calloc(sizeof(MainWin), sizeof(char));
	if (retWin == NULL ) return NULL; //memory allocation error

	// Creates the window itself
	retWin->window = SDL_CreateWindow(/*title-*/ "Main Menu",
				/*x pos-*/ SDL_WINDOWPOS_CENTERED, /*y pos-*/ SDL_WINDOWPOS_CENTERED,
				/*width-*/ 1240,	/*height-*/ 864, /*flags-*/ SDL_WINDOW_OPENGL);

	if (retWin->window == NULL ) { //error creating window
		destroyMainMenuWindow(retWin);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	retWin->renderer = SDL_CreateRenderer(retWin->window, -1, SDL_RENDERER_ACCELERATED);
	if (retWin->renderer == NULL ) { //error creating renderer
		destroyMainMenuWindow(retWin);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./guiBMPs/MainMenu/mainmenu_back.bmp");
	if (loadingSurface == NULL){
		printf("Could not create a surface: %s\n", SDL_GetError());
		destroyMainMenuWindow(retWin);
		return NULL ;
	}
	retWin->bgTexture = SDL_CreateTextureFromSurface(retWin->renderer,loadingSurface); //sets the background
	if (retWin->bgTexture == NULL){
		printf("Could not create a texture: %s\n", SDL_GetError());
		destroyMainMenuWindow(retWin);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //we are done with the temporary surface
	retWin->widgets = createMainMenuWindowWidgets(retWin->renderer);
	if (retWin->widgets == NULL ) { //error creating the widgets
		destroyMainMenuWindow(retWin);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	retWin->numOfWidgets = MAIN_MENU_WIDGETS_NUMBER;
	retWin->first = true;
	return retWin;
}


//draws the given main menu window
void drawMainMenuWindow(MainWin* window) {
	if (window == NULL) return; //argument isn't a valid main window struct

	SDL_Rect rec = {.x=0,.y=0,.w=1240,.h=864 };
	SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
	SDL_RenderClear(window->renderer);
	SDL_RenderCopy(window->renderer, window->bgTexture, NULL, &rec);
	for (int i=0; i < window->numOfWidgets; i++) {
		(window->widgets[i])->drawWidget(window->widgets[i]);
	}
	SDL_RenderPresent(window->renderer);
}


//handles main menu window events, given the window and the event
MAIN_MENU_EVENT handleMainMenuWindowEvent(MainWin* window, SDL_Event* event) {
	if (event == NULL) return MAIN_MENU_INVALID_ARGUMENT; //argument isn't a valid event

	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (window->widgets[0]->handleWidgetEvent(window->widgets[0],event)==1) {
			return MAIN_MENU_NEW_GAME;
		} else if (window->widgets[1]->handleWidgetEvent(window->widgets[1],event)==1) {
			return MAIN_MENU_LOAD;
		}else if (window->widgets[2]->handleWidgetEvent(window->widgets[2],event)==1){
			return MAIN_MENU_EXIT;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return MAIN_MENU_EXIT;
		}
		break;
	default:
		return MAIN_MENU_NONE;
	}
	return MAIN_MENU_NONE;
}
