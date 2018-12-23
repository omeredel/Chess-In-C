#ifndef GAMEWINDOW_H_
#define GAMEWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include "Buttons.h"
#include "../Chess.h"


typedef enum {
	GAME_EVENT_RESTART,
	GAME_EVENT_SAVE,
	GAME_EVENT_LOAD,
	GAME_EVENT_UNDO,
	GAME_EVENT_MAIN_MENU,
	GAME_EVENT_EXIT,
	GAME_EVENT_INVALID_ARGUMENT,
	GAME_EVENT_NONE,
	GAME_EVENT_UPDATE,
} GAME_EVENT;


typedef enum {
	GAME_TIE,
	GAME_MATE,
	GAME_CHECK,
	GAME_CONT,
} GAME_MESSAGE;


typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* boardTexture;
	SDL_Texture* bgTexture;
	Widget** widgets;
	int numOfWidgets;
	bool isFirst;
}GameWindow;

void destroyGameWindowWidgets(Widget** widgets, char board[BOARD_SIZE][BOARD_SIZE]);

char* getPieceImagePath(char board[BOARD_SIZE][BOARD_SIZE], int i, int j);

Widget** createGameWindowWidgets(SDL_Renderer* renderer,char board[BOARD_SIZE][BOARD_SIZE]);

void destroyGameWindow(GameWindow* src, char board[BOARD_SIZE][BOARD_SIZE]);

GameWindow* createGameWindow(char board[BOARD_SIZE][BOARD_SIZE]);

void drawGameWindow(GameWindow* src, bool update, char board[BOARD_SIZE][BOARD_SIZE]);

GAME_MESSAGE checkWinnerOrTie(char board[BOARD_SIZE][BOARD_SIZE]);

GAME_EVENT handleGameWindowEvent(GameWindow* src, SDL_Event* event, char board[BOARD_SIZE][BOARD_SIZE]);


#endif
