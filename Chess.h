#ifndef CHESS_H_
#define CHESS_H_

#include <stdio.h>
#include <stdlib.h>


typedef struct Position
{
	int x;
	int y;
}Pos;


typedef struct Move
{
	char type;
	Pos *pos_start;
	Pos *pos_end;
	struct Move *next;
}Move;

#define WHITE 1
#define BLACK 0


#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'


#define EMPTY ' '


#define BOARD_SIZE 8
#define HISTORY_SIZE 3
#define HISTORY_LENGTH 6
#define FILES_PATH "./guiBMPs/files/"
#define FILES_SLOTS 5
#define FILES_PATH_LENGTH 14
#define PIECES_PATH "./guiBMPs/Pieces/"
#define PIECES_PATH_LENGTH 34
#define PIECES_NAME_MAX_LENGTH 15
#define PIECES_POSTFIX ".bmp"
#define PIECES_POSTFIX_LENGTH 5

#define ENTER_SETTINGS "Specify game setting or type 'start' to begin a game with the current setting:\n"
#define WRONG_GAME_MODE "Wrong game mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define FILE_PROBLEM "File cannot be created or modified\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"  
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define INVALID_TYPE "Invalid Type\n"
#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  
#define TIE "The game is tied\n"
#define ENTER_PROM "Pawn promotion- please replace the pawn by queen, rook, knight, bishop or pawn:\n"
#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));

int gameSaved;
int isGui;
int turn;
int user_color;
int difficulty;
int game_mode;
int is_move_king_white;
int is_move_king_black;
int is_move_right_rook_white;
int is_move_left_rook_white;
int is_move_right_rook_black;
int is_move_left_rook_black;

#define which_color(x) ((x>0)? ("WHITE"): ("BLACK"))
#define which_color_lower(x) ((x>0)? ("white"): ("black"))


#endif

