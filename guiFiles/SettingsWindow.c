#include "SettingsWindow.h"
#include <SDL_video.h>
#include <stdio.h>
#include "../Chess.h"
#include "../Game.h"


//destroys all settings window widgets
void destroySettingsWindowWidgets(Widget** widgets){
	for (int i=0; i<SETTINGS_WIDGETS_NUMBER; i++){
		if (widgets[i] != NULL){
			widgets[i]->destroyWidget(widgets[i]);
		}
	}
	free(widgets);
}


//creates buttons to be used in the settings window
Widget** createSettingsWindowWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) return NULL; //argument isn't a valid renderer

	Widget** widgets = calloc(SETTINGS_WIDGETS_NUMBER, sizeof(Widget*));
	if (widgets == NULL) return NULL; //memory allocation error

	//page 1 - game mode
	SDL_Rect oneplayerRec = { .x =BUTTON_SHIFT, .y = 60, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect twoplayersRec = { .x =BUTTON_SHIFT, .y = 170, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	//page 2 - difficulty
	SDL_Rect noobRec = { .x = BUTTON_SHIFT, .y = 60, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect easyRec = { .x = BUTTON_SHIFT, .y = 170, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect moderateRec = { .x = BUTTON_SHIFT, .y = 280, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect hardRec = { .x = BUTTON_SHIFT, .y = 390, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	//page 3 - color select
	SDL_Rect blackRec = { .x = BUTTON_SHIFT, .y = 60, .h = 90, .w = 90 };
	SDL_Rect whiteRec = { .x = BUTTON_SHIFT+150, .y = 60, .h = 90, .w = 90 };
	//common
	SDL_Rect nextRec = { .x = BUTTON_SHIFT, .y = 500, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect startRec = { .x = BUTTON_SHIFT, .y = 500, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect backRec = { .x = BUTTON_SHIFT, .y = 610, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };

	widgets[0] = createButton("./guiBMPs/Settings/gameMode/oneplayer.bmp", "./guiBMPs/Settings/gameMode/oneplayerp.bmp", renderer, &oneplayerRec);
	widgets[1] = createButton("./guiBMPs/Settings/gameMode/twoplayers.bmp", "./guiBMPs/Settings/gameMode/twoplayersp.bmp", renderer, &twoplayersRec);
	widgets[2] = createButton("./guiBMPs/Settings/gameMode/next.bmp", NULL, renderer, &nextRec);
	widgets[3] = createButton("./guiBMPs/Settings/gameMode/start.bmp", NULL, renderer, &startRec);
	widgets[4] = createButton("./guiBMPs/Settings/gameMode/back.bmp", NULL, renderer, &backRec);
	widgets[5] = createButton("./guiBMPs/Settings/Difficulty/noob.bmp", "./guiBMPs/Settings/Difficulty/noobp.bmp", renderer, &noobRec);
	widgets[6] = createButton("./guiBMPs/Settings/Difficulty/easy.bmp", "./guiBMPs/Settings/Difficulty/easyp.bmp", renderer, &easyRec);
	widgets[7] = createButton("./guiBMPs/Settings/Difficulty/moderate.bmp", "./guiBMPs/Settings/Difficulty/moderatep.bmp", renderer, &moderateRec);
	widgets[8] = createButton("./guiBMPs/Settings/Difficulty/hard.bmp", "./guiBMPs/Settings/Difficulty/hardp.bmp", renderer, &hardRec);
	widgets[9] = createButton("./guiBMPs/Settings/UserColor/blackKingSelected.bmp", "./guiBMPs/Settings/UserColor/blackKing.bmp", renderer, &blackRec);
	widgets[10] = createButton("./guiBMPs/Settings/UserColor/whiteKingSelected.bmp", "./guiBMPs/Settings/UserColor/whiteKing.bmp", renderer, &whiteRec);

	for (int i=0; i<SETTINGS_WIDGETS_NUMBER; i++){
		if (widgets[i] == NULL){
			printf("error creating settings window widget number %d",i);
			destroySettingsWindowWidgets(widgets);
			break;
		}
	}
	return widgets;
}


//destroys all the data that is associated with the given settings window
void destroySettingsWindow(SettingsWin* window) {
	if (window == NULL) return; //argument isn't a valid settings window struct

	destroySettingsWindowWidgets(window->widgets);
	if (window->bgTexture != NULL) SDL_DestroyTexture(window->bgTexture);
	if (window->renderer != NULL ) SDL_DestroyRenderer(window->renderer);
	if (window->window != NULL ) SDL_DestroyWindow(window->window);
	free(window);
}


//creates a settings window
SettingsWin* createSettingsWindow(){
	SettingsWin* retWin = NULL;
	SDL_Surface* loadingSurface = NULL; //used as temporary surface
	retWin = (SettingsWin*) calloc(sizeof(SettingsWin), sizeof(char));
	if (retWin == NULL) return NULL; //memory allocation error

	//creates the window itself
	retWin->window = SDL_CreateWindow(/*title-*/ "Settings",
				/*x pos-*/ SDL_WINDOWPOS_CENTERED, /*y pos-*/ SDL_WINDOWPOS_CENTERED,
				/*width-*/ 1240,	/*height-*/ 864, /*flags-*/ SDL_WINDOW_OPENGL);

	if (retWin->window == NULL ) { //error creating window
		destroySettingsWindow(retWin);
		printf("Error creating window: %s\n", SDL_GetError());
		return NULL ;
	}
	retWin->renderer = SDL_CreateRenderer(retWin->window, -1,SDL_RENDERER_ACCELERATED);
	if (retWin->renderer == NULL ) { //error creating renderer
		destroySettingsWindow(retWin);
		printf("Error creating renderer: %s\n", SDL_GetError());
		return NULL ;
	}
	loadingSurface = SDL_LoadBMP("./guiBMPs/Settings/gameMode/gamemode_back.bmp");
	if (loadingSurface == NULL){ //error creating surface
		printf("Error creating surface: %s\n", SDL_GetError());
		destroySettingsWindow(retWin);
		return NULL ;
	}
	retWin->bgTexture = SDL_CreateTextureFromSurface(retWin->renderer, loadingSurface);
	if (retWin->bgTexture == NULL){ //error creating texture from surface
		printf("Error creating texture: %s\n", SDL_GetError());
		destroySettingsWindow(retWin);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //we are done with the temporary surface
	retWin->widgets = createSettingsWindowWidgets(retWin->renderer);
	if (retWin->widgets == NULL ){ //error creating widgets
		destroySettingsWindow(retWin);
		printf("Error creating widgets: %s\n", SDL_GetError());
		return NULL;
	}
	retWin->numOfWidgets = SETTINGS_WIDGETS_NUMBER;
	retWin->isFirst = true;
	//default settings:
	game_mode = 1;
	user_color = 1;
	difficulty = 2;
	return retWin;
}


//changes the background for different pages in the settings window
void changeSettingsWindowBackground(SettingsWin* window, int page){
	SDL_Surface* loadingSurface = NULL; //temporary
	if (page == 1){ //game mode page
			loadingSurface = SDL_LoadBMP("./guiBMPs/Settings/gameMode/gamemode_back.bmp");
				if (loadingSurface == NULL){
					printf("Could not create a surface: %s\n", SDL_GetError());
					destroySettingsWindow(window);
				}
				window->bgTexture = SDL_CreateTextureFromSurface(window->renderer, loadingSurface);
				if (window->bgTexture == NULL){ //error creating texture from surface
					printf("Could not create a texture: %s\n", SDL_GetError());
					destroySettingsWindow(window);
				}
				SDL_FreeSurface(loadingSurface); //we are done with the temporary surface
		}
	else if (page == 2){ //difficulty page
		loadingSurface = SDL_LoadBMP("./guiBMPs/Settings/Difficulty/difficulty_back.bmp");
			if (loadingSurface == NULL){
				printf("Error creating surface: %s\n", SDL_GetError());
				destroySettingsWindow(window);
			}
			window->bgTexture = SDL_CreateTextureFromSurface(window->renderer, loadingSurface);
			if (window->bgTexture == NULL){ //error creating texture from surface
				printf("Error creating texture: %s\n", SDL_GetError());
				destroySettingsWindow(window);
			}
			SDL_FreeSurface(loadingSurface); //we are done with the temporary surface
	}
	else if (page == 3){  //select color page
		loadingSurface = SDL_LoadBMP("./guiBMPs/Settings/UserColor/color_back.bmp");
					if (loadingSurface == NULL){
						printf("Error creating surface: %s\n", SDL_GetError());
						destroySettingsWindow(window);
					}
					window->bgTexture = SDL_CreateTextureFromSurface(window->renderer, loadingSurface);
					if (window->bgTexture == NULL){ //error creating texture from surface
						printf("Error creating texture: %s\n", SDL_GetError());
						destroySettingsWindow(window);
					}
					SDL_FreeSurface(loadingSurface); //we are done with the temporary surface
	}
	SDL_RenderPresent(window->renderer);
}


//drwas the given settings window
void drawSettingsWindow(SettingsWin* window) {
	if (window == NULL) return; //argument isn't a valid settings window struct

	SDL_Rect rec = { .x=0,.y=0,.w=1240,.h=864 };
	SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
	SDL_RenderClear(window->renderer);
	changeSettingsWindowBackground(window, window->page);
	SDL_RenderCopy(window->renderer, window->bgTexture, NULL, &rec);
	for (int i=0; i < window->numOfWidgets; i++) {
		((Button*)window->widgets[i]->data)->isShown = false;
		((Button*)window->widgets[i]->data)->isActive = false;
	}
	switch(window->page){
	case(1):{ //game mode
		((Button*) window->widgets[0]->data)->isShown = true;
		((Button*) window->widgets[1]->data)->isShown = true;
		if (game_mode == 1){
			((Button*) window->widgets[0]->data)->isActive = true;
			((Button*) window->widgets[2]->data)->isShown = true;
		}
		else if (game_mode == 2){
			((Button*) window->widgets[1]->data)->isActive = true;
			((Button*) window->widgets[3]->data)->isShown = true;
			((Button*) window->widgets[3]->data)->isActive = true;
		}
	}break;
	case(2):{ //difficulty
		((Button*) window->widgets[5]->data)->isShown = true;
		((Button*) window->widgets[6]->data)->isShown = true;
		((Button*) window->widgets[7]->data)->isShown = true;
		((Button*) window->widgets[8]->data)->isShown = true;
		((Button*) window->widgets[difficulty+4]->data)->isActive = true;
		((Button*) window->widgets[2]->data)->isShown = true;
	}break;
	case(3):{ //color select
		((Button*) window->widgets[9]->data)->isShown = true;
		((Button*) window->widgets[10]->data)->isShown = true;
		((Button*) window->widgets[user_color+9]->data)->isActive = true;
		((Button*) window->widgets[3]->data)->isShown = true;
		((Button*) window->widgets[3]->data)->isActive = true;
	}break;
	}
	((Button*) window->widgets[4]->data)->isShown = true;
	for (int i=0; i < window->numOfWidgets; i++) {
		(window->widgets[i])->drawWidget(window->widgets[i]);
	}
	SDL_RenderPresent(window->renderer);
}


//handles settings window events, given the window and the event
SETTINGS_EVENT handleSettingsWindowEvent(SettingsWin* window, SDL_Event* event) {
	if (event == NULL) return SETTINGS_EVENT_INVALID_ARGUMENT; //argument isn't a valid event

	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (window->widgets[0]->handleWidgetEvent(window->widgets[0],event) == 1){ //one player
			game_mode = 1;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[1]->handleWidgetEvent(window->widgets[1],event) == 1){ //two players
			game_mode = 2;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[2]->handleWidgetEvent(window->widgets[2],event) == 1){ //next
			window->page++;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[3]->handleWidgetEvent(window->widgets[3],event) == 1){ //start
			return SETTINGS_EVENT_START;
		}
		else if (window->widgets[4]->handleWidgetEvent(window->widgets[4],event) == 1){ //back
			if(window->page == 1){
				return SETTINGS_EVENT_BACK;
			}
			window->page--;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[5]->handleWidgetEvent(window->widgets[5],event) == 1){ //noob
			difficulty = 1;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[6]->handleWidgetEvent(window->widgets[6],event) == 1){ //easy
			difficulty = 2;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[7]->handleWidgetEvent(window->widgets[7],event) == 1){ //moderate
			difficulty = 3;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[8]->handleWidgetEvent(window->widgets[8],event) == 1){ //hard
			difficulty = 4;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[9]->handleWidgetEvent(window->widgets[9],event) == 1){ //black
			user_color = BLACK;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (window->widgets[10]->handleWidgetEvent(window->widgets[10],event) == 1){ //white
			user_color = WHITE;
			return SETTINGS_EVENT_UPDATE;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) { //quit
			return SETTINGS_EVENT_EXIT;
		}
		break;
	default:
		return SETTINGS_EVENT_NONE;
	}
	return SETTINGS_EVENT_NONE;
}
