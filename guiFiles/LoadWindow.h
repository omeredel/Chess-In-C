#ifndef LOADWINDOW_H_
#define LOADWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include "Buttons.h"


typedef enum {
	LOAD_EVENT_SLOT_1,
	LOAD_EVENT_SLOT_2,
	LOAD_EVENT_SLOT_3,
	LOAD_EVENT_SLOT_4,
	LOAD_EVENT_SLOT_5,
	LOAD_EVENT_BACK,
	LOAD_EVENT_LOAD,
	LOAD_EVENT_INVALID_ARGUMENT,
	LOAD_EVENT_NONE,
	LOAD_EVENT_UPDATE,
	LOAD_EVENT_EXIT,
} LOAD_EVENT;


typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* bgTexture;
	Widget** widgets;
	int numOfWidgets;
	int fileNum;
	bool isFirst;
	bool isLoadLegal;
}LoadWin;


void destroyLoadWindowWidgets(Widget** widgets);

Widget** createLoadWindowWidgets(SDL_Renderer* renderer);

void destroyLoadWindow(LoadWin* window);

LoadWin* createLoadWindow();

void drawLoadWindow(LoadWin* window);

char* getFilePath(LoadWin* window);

void legalPath(LoadWin* window);

LOAD_EVENT handleLoadWindowEvent(LoadWin* window, SDL_Event* event);


#endif
