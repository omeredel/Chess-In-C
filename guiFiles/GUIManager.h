#ifndef GUIMANAGER_H_
#define GUIMANAGER_H_

#include <SDL.h>
#include "GameWindow.h"
#include "SettingsWindow.h"
#include "LoadWindow.h"
#include "MainMenuWindow.h"


typedef enum{
	MAIN_MENU_WINDOW_ACTIVE,
	LOAD_WINDOW_ACTIVE,
	SETTINGS_WINDOW_ACTIVE,
	GAME_WINDOW_ACTIVE
}ACTIVE_WINDOW;


typedef enum{
	MANAGER_UPDATE,
	MANAGER_RESTART_UPDATE,
	MANAGER_NONE,
	MANAGER_EXIT
}MANAGER_EVENT;


typedef struct {
	MainWin* mainWindow;
	LoadWin* loadWindow;
	SettingsWin* settingsWindow;
	GameWindow* gameWindow;
	ACTIVE_WINDOW activeWindow;
} GuiManager;


void destroyGuiManager(GuiManager* manager, char board[BOARD_SIZE][BOARD_SIZE]);

GuiManager* createGuiManager();

void drawGuiManager(GuiManager* manager, char board[BOARD_SIZE][BOARD_SIZE]);

bool doesFileExist(char *fname);

int checkFilesExist();

void saveGameGui(char board[BOARD_SIZE][BOARD_SIZE]);

int doYouWantToQuit();

MANAGER_EVENT handleGuiManagerInMainMenuEvent(GuiManager* manager, MAIN_MENU_EVENT event);

MANAGER_EVENT handleGuiManagerInLoadEvent(GuiManager* manager, LOAD_EVENT event, char board[BOARD_SIZE][BOARD_SIZE]);

MANAGER_EVENT handleGuiManagerInSettingsEvent(GuiManager* manager, SETTINGS_EVENT event, char board[BOARD_SIZE][BOARD_SIZE]);

MANAGER_EVENT handleGuiManagerInGameEvent(GuiManager* manager, GAME_EVENT event, char board[BOARD_SIZE][BOARD_SIZE]);

MANAGER_EVENT handleGuiManagerEvent(GuiManager* manager, SDL_Event* event, char board[BOARD_SIZE][BOARD_SIZE]);


int mainGui(char board[BOARD_SIZE][BOARD_SIZE]);

#endif
