#ifndef FILES_H_
#define FILES_H_

#include "Chess.h"

void createXML(FILE *file, char board[BOARD_SIZE][BOARD_SIZE]);

void readXML(FILE *file, char board[BOARD_SIZE][BOARD_SIZE]);

#endif
