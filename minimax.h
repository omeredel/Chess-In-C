#ifndef MINIMAX_H_
#define MINIMAX_H_

#include <stdio.h>
#include <stdlib.h>
#include "Chess.h"
#include "Moves.h"

#define valBest 700

int isPromotionMin(Move *m, int color);

int isPromotion(Move *m, int color, char BOARD[BOARD_SIZE][BOARD_SIZE]);

Move* promotion(int color, Move *m);

int scoring(int color, char BOARD[BOARD_SIZE][BOARD_SIZE]);

void copyBoard(char BOARD[BOARD_SIZE][BOARD_SIZE], char newB[BOARD_SIZE][BOARD_SIZE]);

int leafScore(Move* move);

Move* cloneMove(Move *m);

void makeMove(char BOARD[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2);

Move *minimax(Move *move, unsigned int depth, char BOARD[BOARD_SIZE][BOARD_SIZE], int color, char score, int bestM);

void freeMoves(Move* m);

int minimax_rec(unsigned int depth, char BOARD[BOARD_SIZE][BOARD_SIZE], int max, int color, int lowB, int upB, int bestM);

Move* minimaxManager(char BOARD[BOARD_SIZE][BOARD_SIZE], Move* moves, int color, unsigned int depth, char score);

int countMoves(Move* m);

int bestScoring(char color, char board[BOARD_SIZE][BOARD_SIZE]);

Move *sortMoves(int color, char BOARD[BOARD_SIZE][BOARD_SIZE], Move* moves);


#endif
