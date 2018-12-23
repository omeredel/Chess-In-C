#ifndef MAINMENUWINDOW_H_
#define MAINMENUWINDOW_H_

#include <SDL.h>
#include "Buttons.h"


typedef enum {
	MAIN_MENU_NEW_GAME,
	MAIN_MENU_LOAD,
	MAIN_MENU_EXIT,
	MAIN_MENU_INVALID_ARGUMENT,
	MAIN_MENU_NONE,
} MAIN_MENU_EVENT;


typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* bgTexture;
	Widget** widgets;
	int numOfWidgets;
	bool first;
} MainWin;


void destroyMainMenuWindowWidgets(Widget** widgets);

Widget** createMainMenuWindowWidgets(SDL_Renderer* renderer);

void destroyMainMenuWindow(MainWin* window);

MainWin* createMainMenuWindow();

void drawMainMenuWindow(MainWin* window);

MAIN_MENU_EVENT handleMainMenuWindowEvent(MainWin* window, SDL_Event* event);


#endif
