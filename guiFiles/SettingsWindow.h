#ifndef SETTINGSWINDOW_H_
#define SETTINGSWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include "Buttons.h"


typedef enum {
	SETTINGS_EVENT_NEXT,
	SETTINGS_EVENT_BACK,
	SETTINGS_EVENT_START,
	SETTINGS_EVENT_INVALID_ARGUMENT,
	SETTINGS_EVENT_NONE,
	SETTINGS_EVENT_UPDATE,
	SETTINGS_EVENT_EXIT,
} SETTINGS_EVENT;


typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* bgTexture;
	Widget** widgets;
	int numOfWidgets;
	bool isFirst;
	int page;
}SettingsWin;


void destroySettingsWindowWidgets(Widget** widgets);

Widget** createSettingsWindowWidgets(SDL_Renderer* renderer);

void destroySettingsWindow(SettingsWin* window);

SettingsWin* createSettingsWindow();

void changeSettingsWindowBackground(SettingsWin* window, int page);

void drawSettingsWindow(SettingsWin* window);

SETTINGS_EVENT handleSettingsWindowEvent(SettingsWin* window, SDL_Event* event);


#endif
