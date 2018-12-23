#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include "Chess.h"
#include "Moves.h"

Move *xyMoves(char board[BOARD_SIZE][BOARD_SIZE], int color, int x, int y);

int move(char board[BOARD_SIZE][BOARD_SIZE], int color, int x, int y, int xnew, int ynew, char ch);

char selectPromotionPiece(int color);

int promotionIfGui();

char piece(int color, char *str);

char* whichType(char p);

char* whichTypeExtended(char p);

int blackOrWhite(char);

void removeSpaces(char* str);

void initCastling(char board[BOARD_SIZE][BOARD_SIZE]);

void updateCastling(char board[BOARD_SIZE][BOARD_SIZE], int x, int y);

void initHistory();

void undoMove(char board[BOARD_SIZE][BOARD_SIZE]);

void undoGivenMove(char *moveToUndo, char board[BOARD_SIZE][BOARD_SIZE], int color);

void popHistoryStack();

void insertHistoryStack();

void saveMove(int x, int y, int xnew, int ynew, char board[BOARD_SIZE][BOARD_SIZE]);

int getScore(char board[BOARD_SIZE][BOARD_SIZE], int color, unsigned int depth, int x, int y, int xnew, int ynew, char ch);

int save(char board[BOARD_SIZE][BOARD_SIZE], char *s);

void copyString(char *target, char *source);

int callFunction(char board[BOARD_SIZE][BOARD_SIZE], int depth, char *str, int color);

void SettingsState(char board[BOARD_SIZE][BOARD_SIZE]);

int UserState(char board[BOARD_SIZE][BOARD_SIZE]);

int GameState(char board[BOARD_SIZE][BOARD_SIZE]);

void clear(char board[BOARD_SIZE][BOARD_SIZE]);

int restart(char board[BOARD_SIZE][BOARD_SIZE]);

void initBoard(char board[BOARD_SIZE][BOARD_SIZE]);

int valid(char board[BOARD_SIZE][BOARD_SIZE]);

void countPieces(int counter[2][6], char board[BOARD_SIZE][BOARD_SIZE]);

int countPlayerPieces(int playerColor, char board[BOARD_SIZE][BOARD_SIZE]);

void settingsToDefault();

void mainConsole(char board[BOARD_SIZE][BOARD_SIZE]);


#endif
