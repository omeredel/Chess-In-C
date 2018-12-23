#include "Moves.h"
#include "Game.h"
#include "minimax.h"
#include "Chess.h"
#include "string.h"
#include <stdio.h>
#include "Files.h"


#define which_color2(x) ((x>0)? ("White"): ("Black"))


//creates xml file with all the parameters as described
void createXML(FILE *file, char board[BOARD_SIZE][BOARD_SIZE]){
	char row[9];
	int i, j;
	fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(file, "<game>\n");
	fprintf(file, "\t<current_turn>%d</current_turn>\n", turn);
	fprintf(file, "\t<game_mode>%d</game_mode>\n", game_mode);
	if (game_mode == 1){
		fprintf(file, "\t<difficulty>%d</difficulty>\n", difficulty);
		fprintf(file, "\t<user_color>%d</user_color>\n", user_color);
	}
	fprintf(file, "\t<board>\n");
	for (i = 8; i >0; i--){
		for (j = 0; j < BOARD_SIZE; j++){
			if (board[j][i - 1] == EMPTY)
				row[j] = '_';
			else
				row[j] = board[j][i-1];
		}
		row[8] = '\0';
		fprintf(file, "\t\t<row_%d>%s</row_%d>\n",i, row,i);
	}
	fprintf(file, "\t</board>\n");
	fprintf(file, "</game>\n");

}


//updates the game mode variable
void setGameMode(char *num){
	if (strstr(num, "1"))
		game_mode = 1;
	else{
		game_mode = 2;
	}
}


//updates the difficulty variable
void setDifficulty(char *num){
	difficulty = 2;
	switch (num[0]){
	case '5':
		difficulty = 5;
		break;
	case '4':
		difficulty = 4;
		break;
	case '3':
		difficulty = 3;
		break;
	case '1':
		difficulty = 1;
		break;
	}
}


//updates the user color variable
void setColor(char *color){
	if (strstr(color, "Black"))
		user_color = 0;
	else{
		user_color = 1;
	}
}


//updates the board variable
void setBoard(char *str, char board[BOARD_SIZE][BOARD_SIZE], int row){
	int i;
	for (i=0; i<BOARD_SIZE; i++){
		if (str[i] == '_') board[i][row] = EMPTY;
		else board[i][row] = str[i];
	}
}


//returns the next line from the file
char *readLine(FILE *file){
	char *str = (char*)malloc(10 * sizeof(char));
	char ch;
	if (str == NULL){
		printf("Error: standard function %s has failed","malloc");
		return NULL;}
	int count = 10;
	int index = 0;
	while ((ch = fgetc(file))!= '\n'){ //didn't reach end of line
		if (index == count-2){
			count=count + 10;
			str = (char*)realloc(str, count * sizeof(char));
			if (str == NULL){
				printf("Error: standard function %s has failed","realloc");
				return NULL;}
		}
		str[index] = ch;
		index++;
	}
	str[index] = '\0';
	return str;
}


//read the xml file and update all the parameters
void readXML(FILE *file, char board[BOARD_SIZE][BOARD_SIZE]){
	char *str = readLine(file);
	int row = 8;
	if (str == NULL){ //no next line or failure
		fclose(file);
		exit(0);
	}
	while (strstr(str, "/board") == NULL){
		if (strstr(str, "current_turn") != NULL){
			if (strstr(str, "0"))
				turn = 0;
			else{
				turn = 1;
			}
		}
		if (strstr(str, "game_mode") != NULL){
			setGameMode(str);
		}
		if (strstr(str, "difficulty") != NULL){
			char *index = strstr(str, ">");
			setDifficulty(index + 1);
		}
		if (strstr(str, "user_color") != NULL){
			if (strstr(str, "0"))
				user_color = 0;
			else{
				user_color = 1;
			}
		}
		if (strstr(str, "row") != NULL){
			char *index = strstr(str, ">");
			setBoard(index + 1, board, row-1);
			row--;
		}
		free(str);
		str = readLine(file);
		if (str == NULL){
			fclose(file);
			exit(0);
		}
	}
	free(str);
}

