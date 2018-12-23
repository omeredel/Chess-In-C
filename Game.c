#include "Moves.h"
#include "minimax.h"
#include "Game.h"
#include <string.h>
#include "Chess.h"
#include "Files.h"
#include <ctype.h>


char *command;


//removes the piece from a given place
void removePiece(char board[BOARD_SIZE][BOARD_SIZE], int i, int j){
	board[i][j] = EMPTY;
}


//clears the board
void clearBoard(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for (i=0; i<BOARD_SIZE; i++){
		for (j=0; j<BOARD_SIZE; j++){
			removePiece(board, i, j);
		}
	}
}


//removes the spaces from a given string
void removeSpaces(char* str){
	int i = 0;
	int j = 0;
	int f = 0;
	while (str[j] != '\0'){
		str[i] = str[j];
		j++;
		if (str[i] != ' ' && str[i] != '\t'){
			f = 1;
			i++;
		}
		else if (f){
			str[i] = ' ';
			f = 0;
			i++;
		}
	}
	if (str[i - 1] == ' '){ i--; }
	str[i] = '\0';
}


int blackOrWhite(char ch){
	if(islower(ch) > 0) return 1;
	else{
		return 0;
	}
}


//counts the number of pieces on the board, first place - white pieces, second - black pieces
void countPieces(int counter[2][6],char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for (i=0; i<BOARD_SIZE; i++){
		for (j=0; j<BOARD_SIZE; j++){
			if (board[i][j] == 'm')counter[0][0]++;
			else if (board[i][j] == 'b') counter[0][1]++;
			else if (board[i][j] == 'n') counter[0][2]++;
			else if (board[i][j] == 'r') counter[0][3]++;
			else if (board[i][j] == 'q') counter[0][4]++;
			else if (board[i][j] == 'k') counter[0][5]++;
			else if (board[i][j] == 'M') counter[1][0]++;
			else if (board[i][j] == 'B') counter[1][1]++;
			else if (board[i][j] == 'N') counter[1][2]++;
			else if (board[i][j] == 'R') counter[1][3]++;
			else if (board[i][j] == 'Q') counter[1][4]++;
			else if (board[i][j] == 'K') counter[1][5]++;
		}
	}
}


//counts player's pieces - 1 for white player, 0 for black player, 2 for both
int countPlayerPieces(int playerColor, char board[BOARD_SIZE][BOARD_SIZE]){
	int counter[2][6] = { { 0, 0, 0, 0, 0, }, { 0, 0, 0, 0, 0, 0 } };
	countPieces(counter, board);
	if (playerColor==WHITE) return (counter[0][0]+counter[0][1]+counter[0][2]+counter[0][3]+counter[0][4]+counter[0][5]);
	else if (playerColor==BLACK) return (counter[1][0]+counter[1][1]+counter[1][2]+counter[1][3]+counter[1][4]+counter[1][5]);
	else if (playerColor==2) return (counter[0][0]+counter[0][1]+counter[0][2]+counter[0][3]+counter[0][4]+counter[0][5]+counter[1][0]+counter[1][1]+counter[1][2]+counter[1][3]+counter[1][4]+counter[1][5]);
	else return 666;
}


//checks if the pawns location is legal
int isPawnLegal(char board[BOARD_SIZE][BOARD_SIZE]){
	int i;
	for (i=0; i<BOARD_SIZE; i++){
		if (board[i][7] == 'M') return 1;
		if (board[i][0] == 'M') return 1;
		if (board[i][7] == 'm') return 1;
		if (board[i][0] == 'm') return 1;
	}
	return 0;
}


//checks if the bishops location is legal
int isBishopLegal(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	int x_black = -1, y_black = -1, x_white = -1, y_white = -1;
	for (i=0; i<BOARD_SIZE; i++){
		for (j=0; j<BOARD_SIZE; j++){
			if (board[i][j] == 'B'){
				if (x_black == -1 && y_black == -1){
					x_black = i;
					y_black = j;
				}
				else{
					if ((x_black + y_black) % 2 == (i + j) % 2)
						return 1;
				}
			}
			if (board[i][j] == 'b'){
				if (x_white == -1 && y_white == -1){
					x_white = i;
					y_white = j;
				}
				else{
					if ((x_white + y_white) % 2 == (i + j) % 2)
						return 1;
				}
			}
		}
	}
	return 0;
}


//checks if the number of pieces is legal
int isPiecesNumLegal(char board[BOARD_SIZE][BOARD_SIZE]){
	int counter[2][6] = { { 0, 0, 0, 0, 0, }, { 0, 0, 0, 0, 0, 0 } };
	countPieces(counter, board);
	if (counter[0][0] > 8) return 0;
	else if (counter[0][1] > 2) return 0;
	else if (counter[0][2] > 2) return 0;
	else if (counter[0][3] > 2) return 0;
	else if (counter[0][4] > 1) return 0;
	else if (counter[0][5] > 1) return 0;
	else if (counter[1][0] > 8) return 0;
	else if (counter[1][1] > 2) return 0;
	else if (counter[1][2] > 2) return 0;
	else if (counter[1][3] > 2) return 0;
	else if (counter[1][4] > 1) return 0;
	else if (counter[1][5] > 1) return 0;
	if (isPawnLegal(board)) return 0;
	if (isBishopLegal(board)) return 0;
	return 1;
}


//converts a given string representing a piece (also given- its color) to the char that represents it
char pieceSTRtoCHAR(int color, char *str){
	if (color){
		if (strcmp(str, "king") == 0) return WHITE_K;
		else if (strcmp(str, "bishop") == 0) return WHITE_B;
		else if (strcmp(str, "queen") == 0) return WHITE_Q;
		else if (strcmp(str, "rook") == 0) return WHITE_R;
		else if (strcmp(str, "knight") == 0) return WHITE_N;
		else if (strcmp(str, "pawn") == 0) return WHITE_P;
	}
	else{
		if (strcmp(str, "king") == 0) return BLACK_K;
		else if (strcmp(str, "bishop") == 0) return BLACK_B;
		else if (strcmp(str, "queen") == 0) return BLACK_Q;
		else if (strcmp(str, "rook") == 0) return BLACK_R;
		else if (strcmp(str, "knight") == 0) return BLACK_N;
		else if (strcmp(str, "pawn") == 0) return BLACK_P;
	}
	return '\0';
}


//Settings state function
void SettingsState(char board[BOARD_SIZE][BOARD_SIZE]){
	char c;
	int i = 0, flag = 0;
	settingsToDefault();
	while (1){
		i = 0;
		flag = 0;	//indicates if the command is valid
		print_message(ENTER_SETTINGS);
		char *command = (char*)malloc(51*sizeof(char));
		if (command == NULL){
			printf("Error: standard function %s has failed","malloc");
			exit(0);
		}
		while ((c = getchar()) != '\n'){
			command[i] = c;
			i++;
		}
		command[i] = '\0';
		removeSpaces(command);
		if (strstr(command, "game_mode") != NULL){	//game mode
			flag = 1;
			if (command[10] == '1' && command[11] == '\0'){
				game_mode = 1;
				print_message("Game mode is set to 1 player\n");
			}
			else if (command[10] == '2' && command[11] == '\0'){
				game_mode = 2;
				print_message("Game mode is set to 2 players\n");
			}
			else{print_message(WRONG_GAME_MODE);}
		}

		//1 player settings
		else if (strstr(command, "difficulty") != NULL && game_mode == 1){//difficulty
			flag = 1;
			if (command[11] >= '1' && command[11] <= '5' && command[12] == '\0'){
				if (command[11] == '5') {print_message("Expert level not supported, please choose a value between 1 to 4:\n");}
				else{
					difficulty = atoi(&command[11]);
				}
			}
			else{print_message("Wrong difficulty level. The value should be between 1 to 5\n");}
		}
		else if (strstr(command, "user_color") != NULL && game_mode == 1){//color
			flag = 1;
			if (command[11] == '0' && command[12] == '\0'){user_color = BLACK;}
			else if (command[11] == '1' && command[12] == '\0'){user_color = WHITE;}
			else{ print_message(ILLEGAL_COMMAND); }
		}
		else if (strstr(command, "load") != NULL){//load game
			flag = 1;
			char* fil = command + 5;
			FILE * fp = fopen(fil, "r");
			if (fp == NULL){ print_message("Error: File doesn’t exist or cannot be opened\n"); }
			else{
				readXML(fp, board);
				printBoard(board);
				fclose(fp);			
			}
		}
		else if (strstr(command, "clear") != NULL){//clear
			flag = 1;
			clearBoard(board);
		}
		else if (strstr(command, "default") != NULL){//default
			flag = 1;
			settingsToDefault();
		}
		else if (strstr(command, "print_setting") != NULL){//print setting
			flag = 1;
			if (game_mode == 1){
				print_message("SETTINGS:\n");
				printf("GAME_MODE: %d\n",game_mode);
				printf("DIFFICULTY_LVL: %d\n",difficulty);
				printf("USER_CLR: %s\n",which_color(user_color));
			}
			else{//game_mode == 2
				print_message("SETTINGS:\n");
				printf("GAME_MODE: %d\n",game_mode);
			}
		}
		else if (strstr(command, "quit") != NULL){//quit
			print_message("Exiting...\n");
			free(command);
			exit(0);
		}
		else if ((strstr(command, "start") != NULL) && command[5] == '\0'){//start
			flag = 1;
			int c[2][6] = { { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } };
			countPieces(c, board);
			if (c[0][5] == 0 || c[1][5] == 0){
				free(command);
				print_message("Wrong board initialization\n");
				continue;
			}
			Move *mo = getMoves(board, 1-turn);
			if (mo == NULL){
				free(command);
				exit(0);
			}
			if (mo->type == 't'){
				printf("%s", TIE);
				freeMoves(mo);
				free(command);
				exit(0);
			}
			else if(mo->type == 'l'){
				printf("Checkmate! %s player wins the game\n", which_color(turn));
				freeMoves(mo);
				free(command);
				exit(0);
			}
			freeMoves(mo);
			mo = getMoves(board, turn);
			if (mo == NULL){
				free(command);
				exit(0);
			}
			if (mo->type == 't'){
				printf("%s", TIE);
				freeMoves(mo);
				free(command);
				exit(0);
			}
			else if(mo->type == 'l'){
				printf("Checkmate! %s player wins the game\n", which_color(1 - turn));
				freeMoves(mo);
				free(command);
				exit(0);
			}
			int isCheckBool = isCheck(board, turn);
			if (isCheckBool == 0){
				freeMoves(mo);
				free(command);
				exit(0);
			}
			if (isCheckBool == 1){
				printf("Check: %s King is threatened!\n", which_color(turn));
				freeMoves(mo);
				free(command);
				break;
			}
			else{
				freeMoves(mo);
				free(command);
				break;
			}
		}
		if (flag == 0){//command not valid
			print_message(ILLEGAL_COMMAND);
		}

		free(command);
	}
}

void settingsToDefault(){
	game_mode = 1;
	difficulty = 2;
	user_color = WHITE;
}


//the main function for console mode
void mainConsole(char board[BOARD_SIZE][BOARD_SIZE]){
	printBoard(board);
	SettingsState(board);
	if (GameState(board)==0) exit(0);
}

