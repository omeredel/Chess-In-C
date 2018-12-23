#include "GUIManager.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_events.h>
#include <SDL_messagebox.h>
#include "../Chess.h"
#include "../Game.h"
#include "../minimax.h"
#include "../Files.h"
#include "Buttons.h"


/*
 * Variables for saveGameGui function:
 */
int savedFiles = 0;


//destroys all the data associated with the gui manager argument
void destroyGuiManager(GuiManager* manager, char board[BOARD_SIZE][BOARD_SIZE]){
	if (manager == NULL) return; //argument isn't a valid gui manager

	if (manager->activeWindow == GAME_WINDOW_ACTIVE){
		destroyGameWindow(manager->gameWindow, board);
	}
	destroyMainMenuWindow(manager->mainWindow);
	free(manager);
}


//creates a manager
GuiManager* createGuiManager(){
	GuiManager* retManager = (GuiManager*) malloc(sizeof(GuiManager));
	if (retManager == NULL)return NULL; //error allocating memory
	retManager->mainWindow = createMainMenuWindow();
	if (retManager->mainWindow == NULL){ //error creating main window
		free(retManager);
		return NULL ;
	}
	retManager->gameWindow = NULL;
	retManager->activeWindow = MAIN_MENU_WINDOW_ACTIVE;
	return retManager;
}


//draws the gui manager manager (active window)
void drawGuiManager(GuiManager* manager, char board[BOARD_SIZE][BOARD_SIZE]){
	if (manager == NULL) return; //argument isn't a valid gui manager

	if (manager->activeWindow == MAIN_MENU_WINDOW_ACTIVE){
		drawMainMenuWindow(manager->mainWindow);
	}
	else if (manager->activeWindow == SETTINGS_WINDOW_ACTIVE){
		drawSettingsWindow(manager->settingsWindow);
	}
	else if (manager->activeWindow == LOAD_WINDOW_ACTIVE){
		drawLoadWindow(manager->loadWindow);
	}
	else {
		drawGameWindow(manager->gameWindow,true, board);
	}
}


//checks if the given file can be opened
bool doesFileExist(char *fname){
	FILE *file;
	if ((file = fopen(fname, "r"))){
		fclose(file);
		return true;
	}
	return false;
}


//checks if the file exists
int checkFilesExist(){
	char checkName[FILES_PATH_LENGTH];
	for (int j = 1; j <= 5; j++){
		sprintf(checkName, "%s%d%c", FILES_PATH , j, '\0');
		if (doesFileExist(checkName)){
			savedFiles++;
		}
		else{
			return savedFiles;
		}
	}
	return savedFiles;
}


//handles the gui save command
void saveGameGui(char board[BOARD_SIZE][BOARD_SIZE]){
	char toDelete[FILES_PATH_LENGTH];
	char oldGameName[FILES_PATH_LENGTH], newGameName[FILES_PATH_LENGTH];
	if (savedFiles == 0){
		savedFiles = checkFilesExist();
		savedFiles++;
	}
	if (savedFiles == 0){
		savedFiles++;
		sprintf(newGameName, "%s%d%c", FILES_PATH ,savedFiles, '\0');
		FILE *file = fopen(newGameName, "w");
		createXML(file, board);
		fclose(file);
		savedFiles++;
	}
	else{
		if (savedFiles == FILES_SLOTS+1){
			sprintf(toDelete, "%s%d%c", FILES_PATH ,FILES_SLOTS, '\0');
			remove(toDelete);
			savedFiles--;
		}
		for (int i = savedFiles; i > 1; i--){
			sprintf(oldGameName, "%s%d%c", FILES_PATH, i-1, '\0');
			sprintf(newGameName, "%s%d%c", FILES_PATH, i, '\0');
			rename(oldGameName, newGameName);
		}
		sprintf(newGameName, "%s%d%c", FILES_PATH, 1, '\0');
		FILE *file = fopen(newGameName, "w");
		createXML(file, board);
		savedFiles++;
		fclose(file);
	}
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Chess", "Game has been saved!", NULL);
	gameSaved = 1;
}


//prints quit window
int doYouWantToQuit(){
	const SDL_MessageBoxButtonData buttons[] = {
			{ 0, 0, "Cancel" },
			{ 0, 1, "No" },
			{ 0, 2, "Yes" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
			{{ 255,   0,   0 },
			{   0, 255,   0 },
			{ 255, 255,   0 },
			{   0,   0, 255 },
			{ 255,   0, 255 }}
	};
	const SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION,
			NULL,
			"Confirm exiting...",
			"Do you want to save your current game?",
			SDL_arraysize(buttons),
			buttons,
			&colorScheme
	};
	int buttonID;
	SDL_ShowMessageBox(&messageboxdata, &buttonID);
	return buttonID;
}


//handles the gui manager given a manager andan main menu event
MANAGER_EVENT handleGuiManagerInMainMenuEvent(GuiManager* manager, MAIN_MENU_EVENT event){
	if (manager == NULL) return MANAGER_NONE; //argument isn't a valid gui manager struct
	if (event == MAIN_MENU_NEW_GAME){
		SDL_HideWindow(manager->mainWindow->window); //hide main menu window
		manager->settingsWindow = createSettingsWindow();
		manager->settingsWindow->page = 1;
		if (manager->settingsWindow == NULL){
			printf("Couldn't create settings window\n");
			return MANAGER_EXIT;
		}
		manager->activeWindow = SETTINGS_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
	}
	else if (event == MAIN_MENU_LOAD){
		SDL_HideWindow(manager->mainWindow->window); //hide main menu window
		manager->loadWindow = createLoadWindow();
		if (manager->loadWindow == NULL){
			printf("Couldn't create load window\n");
			return MANAGER_EXIT;
		}
		manager->activeWindow = LOAD_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
	}
	else if (event == MAIN_MENU_EXIT){
		return MANAGER_EXIT;
	}
	return MANAGER_NONE;
}


//handles the gui manager given a manager a load event
MANAGER_EVENT handleManagerInLoadEvent(GuiManager* manager,LOAD_EVENT event, char board[BOARD_SIZE][BOARD_SIZE]){
	if (manager == NULL) return MANAGER_NONE; //argument isn't a valid gui manager struct
	if (event == LOAD_EVENT_UPDATE) return MANAGER_UPDATE;
	if (event == LOAD_EVENT_BACK){
		destroyLoadWindow(manager->loadWindow);
		if (manager->gameWindow != NULL){
			SDL_ShowWindow(manager->gameWindow->window); //show game window
			manager->activeWindow = GAME_WINDOW_ACTIVE;
		}
		else{
			SDL_ShowWindow(manager->mainWindow->window); //show main menu window
			manager->activeWindow = MAIN_MENU_WINDOW_ACTIVE;
		}
		return MANAGER_UPDATE;
	}
	if (event == LOAD_EVENT_LOAD){
		if (manager->gameWindow != NULL) destroyGameWindow(manager->gameWindow, board);
		char* path = getFilePath(manager->loadWindow);
		FILE *fp = fopen(path, "r");
		if (fp == NULL) printf("Error: File doesn’t exist or cannot be opened\n");
		else{
			readXML(fp, board);
			fclose(fp);
		}
		free(path);
		destroyLoadWindow(manager->loadWindow);
		manager->gameWindow = createGameWindow(board);
		if (manager->gameWindow == NULL){
			printf("Couldn't create game window\n");
			return MANAGER_EXIT;
		}
		manager->activeWindow = GAME_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
	}
	else if (event == LOAD_EVENT_EXIT){
		return MANAGER_EXIT;
	}
	return MANAGER_NONE;
}


//handles the gui manager given a manager and an settings event
MANAGER_EVENT handleGuiManagerInSettingsEvent(GuiManager* manager,SETTINGS_EVENT event, char board[BOARD_SIZE][BOARD_SIZE]){
	if (manager == NULL) return MANAGER_NONE;
	if (event == SETTINGS_EVENT_UPDATE) return MANAGER_UPDATE;
	if (event == SETTINGS_EVENT_START){
		manager->gameWindow = createGameWindow(board);
		manager->activeWindow = GAME_WINDOW_ACTIVE;
		destroySettingsWindow(manager->settingsWindow);
		return MANAGER_UPDATE;
	}
	if (event == SETTINGS_EVENT_BACK){
		destroySettingsWindow(manager->settingsWindow);
		SDL_ShowWindow(manager->mainWindow->window); //show main menu window
		manager->activeWindow = MAIN_MENU_WINDOW_ACTIVE;
		manager->settingsWindow->page = 1;
		return MANAGER_UPDATE;
	}
	if (event == SETTINGS_EVENT_EXIT) return MANAGER_EXIT;
	return MANAGER_NONE;
}


//handles the gui manager given a manager and a game event
MANAGER_EVENT handleGuiManagerInGameEvent(GuiManager* manager,GAME_EVENT event, char board[BOARD_SIZE][BOARD_SIZE]){
	if (event == GAME_EVENT_NONE || manager == NULL) return MANAGER_NONE;
	if (event == GAME_EVENT_UPDATE) return MANAGER_UPDATE;
	else if (event == GAME_EVENT_RESTART) return MANAGER_RESTART_UPDATE;
	else if (event == GAME_EVENT_MAIN_MENU){
		if (gameSaved == 0){
			int buttonID = doYouWantToQuit();
			if (buttonID != 0){
				if (buttonID == 2){ //want to save
					saveGameGui(board);
				}
				destroyGameWindow(manager->gameWindow, board);
				manager->gameWindow = NULL;
				manager->activeWindow = MAIN_MENU_WINDOW_ACTIVE;
				SDL_ShowWindow(manager->mainWindow->window); //show main menu window
				return MANAGER_RESTART_UPDATE;
			}
			else{ //not exiting
				return MANAGER_NONE;
			}
		}
		else{
			destroyGameWindow(manager->gameWindow, board);
			manager->gameWindow = NULL;
			manager->activeWindow = MAIN_MENU_WINDOW_ACTIVE;
			SDL_ShowWindow(manager->mainWindow->window); //show main menu window
			return MANAGER_RESTART_UPDATE;
		}
	}
	else if (event == GAME_EVENT_SAVE){
		saveGameGui(board);
		return MANAGER_NONE;
	}
	else if (event == GAME_EVENT_LOAD){
		SDL_HideWindow(manager->gameWindow->window); //hide game window
		manager->loadWindow = createLoadWindow();
		if (manager->loadWindow == NULL){
			printf("Couldn't create load window\n");
			return MANAGER_EXIT;
		}
		manager->activeWindow = LOAD_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
	}
	else if (event == GAME_EVENT_EXIT){
		if(gameSaved == 0){
			int buttonid = doYouWantToQuit();
			if(buttonid != 0){
				if(buttonid == 2){
					saveGameGui(board);
				}
				return MANAGER_EXIT;
			}
			else{
				return MANAGER_NONE;
			}
		}
		else{
			return MANAGER_EXIT;
		}
	}
	return MANAGER_NONE;
}


//handles the gui manager given a manager and an event
MANAGER_EVENT handleGuiManagerEvent(GuiManager* manager, SDL_Event* event, char board[BOARD_SIZE][BOARD_SIZE]){
	if (manager == NULL || event == NULL) return MANAGER_NONE; //invalid arguments
	if (manager->activeWindow == MAIN_MENU_WINDOW_ACTIVE){
		if (manager->mainWindow->first == true){
			manager->mainWindow->first = false;
			drawGuiManager(manager, board);
		}
		MAIN_MENU_EVENT mainEvent = handleMainMenuWindowEvent(manager->mainWindow, event);
		return handleGuiManagerInMainMenuEvent(manager, mainEvent);
	}
	else if (manager->activeWindow == SETTINGS_WINDOW_ACTIVE){
		if (manager->settingsWindow->isFirst == true){
			manager->settingsWindow->isFirst = false;
			drawGuiManager(manager, board);
		}
		SETTINGS_EVENT mainEvent = handleSettingsWindowEvent(manager->settingsWindow, event);
		return handleGuiManagerInSettingsEvent(manager, mainEvent, board);
	}
	else if (manager->activeWindow == LOAD_WINDOW_ACTIVE){
		if (manager->loadWindow->isFirst == true){
			manager->loadWindow->isFirst = false;
			drawGuiManager(manager, board);
		}
		LOAD_EVENT mainEvent = handleLoadWindowEvent(manager->loadWindow, event);
		return handleManagerInLoadEvent(manager, mainEvent, board);
	}
	else if (manager->activeWindow == GAME_WINDOW_ACTIVE){
		if (manager->gameWindow->isFirst == true){
			manager->gameWindow->isFirst = false;
			if (user_color == BLACK){
				Move *moves = getMoves(board, turn);
				if (moves == NULL) return 0;
				moves = minimaxManager(board, moves, turn, difficulty, '\0');
				if (moves == NULL) return 0;
				if (moves->type == 'c'){
					makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
					updateCastling(board, moves->pos_start->y, moves->pos_start->x);
					makeMove(board, 4, 7 * (1-turn), 4  + (2 * (left_right(1-turn) - 2)), 7 * (1-turn));
					updateCastling(board, 7 * (1-turn), 4);
				}
				else{
					saveMove((moves->pos_start->y),(moves->pos_start->x), (moves->pos_end->y), (moves->pos_end->x), board);
					makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
					updateCastling(board, moves->pos_start->x, moves->pos_start->y);
				}
				turn = 1-turn;
				freeMoves(moves);
			}
			drawGuiManager(manager, board);
		}
		GAME_EVENT gameEvent = handleGameWindowEvent(manager->gameWindow, event, board);
		return handleGuiManagerInGameEvent(manager, gameEvent, board);
	}
	return MANAGER_NONE;
}


//gui main function
int mainGui(char board[BOARD_SIZE][BOARD_SIZE]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 initialization
		printf("ERROR: unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	GuiManager* manager = createGuiManager();
	if (manager == NULL ) { //error creating manager
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		MANAGER_EVENT handle = handleGuiManagerEvent(manager, &event, board);
		if (handle == MANAGER_EXIT) { //exit
			break;
		}
		if (handle == MANAGER_UPDATE){
			drawGuiManager(manager, board);
		}
		else if (handle == MANAGER_RESTART_UPDATE){
			initBoard(board);
			drawGuiManager(manager, board);
		}
	}
	destroyGuiManager(manager, board);
	SDL_Quit();
	return 0;
}


