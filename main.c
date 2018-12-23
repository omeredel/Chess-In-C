#include "Moves.h"
#include "minimax.h"
#include "Chess.h"
#include "Game.h"
#include "string.h"
#include <stdio.h>
#include "guiFiles/GUIManager.h"


// main function
int main(int argc, char **argv){
	char board[BOARD_SIZE][BOARD_SIZE];
	restart(board);
	if  (argc == 1){
		isGui = 0;
		mainConsole(board); //console main function
	}
	else if  ((argc == 2) && (strcmp(argv[1], "-g") == 0)) {
		isGui = 1;
		gameSaved = 0;
		mainGui(board); //gui main function
	}
	else if ((argc == 2) && (strcmp(argv[1], "-c") == 0)){
		isGui = 0;
		mainConsole(board); //console main function
	}
	else exit(0);
	return 0;
}

