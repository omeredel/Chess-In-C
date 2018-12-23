#include "Chess.h"
#include "Moves.h"
#include "minimax.h"
#include "Game.h"
#include "Files.h"


//checks if a piece (given by a char) is black
int isBlack(char ch){
	switch (ch){
	case BLACK_B:
		return 1;
		break;
	case BLACK_K:
		return 1;
		break;
	case BLACK_Q:
		return 1;
		break;
	case BLACK_N:
		return 1;
		break;
	case BLACK_R:
		return 1;
		break;
	case BLACK_P:
		return 1;
		break;
	}
	return 0;
}


//checks if a piece (given by a char) is white
int isWhite(char ch){
	switch (ch){
	case WHITE_B:
		return 1;
		break;
	case WHITE_K:
		return 1;
		break;
	case WHITE_Q:
		return 1;
		break;
	case WHITE_N:
		return 1;
		break;
	case WHITE_R:
		return 1;
		break;
	case WHITE_P:
		return 1;
		break;
	}
	return 0;
}


// creates a new move from <x,y> to <xnew,ynew> for some piece
Move *createMove(int x, int y, int xnew, int ynew, char type){
	Move *retMove = (Move *)malloc(sizeof(Move));
	if (retMove == NULL){
		printf("Error: standard function %s has failed","malloc");
		return NULL;
	}
	Pos *pos1 = (Pos *)malloc(sizeof(Pos));
	if (pos1 == NULL){
		free(retMove);
		printf("Error: standard function %s has failed","malloc");
		return NULL;
	}
	Pos *pos2 = (Pos *)malloc(sizeof(Pos));
	if (pos2 == NULL){
		free(pos1);
		free(retMove);
		printf("Error: standard function %s has failed","malloc");
		return NULL;
	}
	pos1->x = x;
	pos1->y = y;
	pos2->x = xnew;
	pos2->y = ynew;
	retMove->pos_start = pos1;
	retMove->pos_end = pos2;
	retMove->type = type;
	retMove->next = NULL;
	return retMove;
}


//creates a list of moves from <x,y> for some pawn
Move *pawnMovesList(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int color, Move *moves){
	char boardCopy[BOARD_SIZE][BOARD_SIZE];
	int flag;
	if (color){
		if (board[x][y + 1] == EMPTY){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x, y + 1);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x, y + 1, WHITE_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
		if (y == 1 && board[x][y + 2] == EMPTY){ //two steps from the starting position
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x, y + 2);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x, y + 2, WHITE_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
		if (x > 0 && isBlack(board[x - 1][y + 1])){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x - 1, y + 1);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x - 1, y + 1, WHITE_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
		if (x < BOARD_SIZE - 1 && isBlack(board[x + 1][y + 1])){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x + 1, y + 1);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x + 1, y + 1, WHITE_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
	}
	else{
		if (board[x][y - 1] == EMPTY){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x, y - 1);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x, y - 1, BLACK_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
		if (y == BOARD_SIZE-2 && board[x][y - 2] == EMPTY){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x, y - 2);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x, y - 2, BLACK_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
		if (x > 0 && isWhite(board[x - 1][y - 1])){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x - 1, y - 1);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x - 1, y - 1, BLACK_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
		if (x < BOARD_SIZE - 1 && isWhite(board[x + 1][y - 1])){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, x + 1, y - 1);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, x + 1, y - 1, BLACK_P);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
	}
	return moves;
}


// creates a move if it's legal
Move *legalMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int xnew, int ynew, int color, Move *moves){
	char boardCopy[BOARD_SIZE][BOARD_SIZE];
	int flag;
	if ((0 <= xnew) && (xnew < BOARD_SIZE) && (0 <= ynew) && (ynew < BOARD_SIZE)){
		if ((board[xnew][ynew] == EMPTY) || (color&&isBlack(board[xnew][ynew])) || (color == 0 && isWhite(board[xnew][ynew]))){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, x, y, xnew, ynew);
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				Move *node = createMove(x, y, xnew, ynew, board[x][y]);
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ free(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
	}
	return moves;
}


// creates a list of moves from <x,y> for some bishop
Move *bishopMovesList(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int color, Move *moves){
	int left_up = 1;
	int right_up = 1;
	int left_down = 1;
	int right_down = 1;
	for (int k = 1; k < BOARD_SIZE; k++){
		if (right_up){
			moves = legalMove(board, x, y, x + k, y + k, color, moves);
			if (moves == NULL)
				return NULL;
			if ((x + k == BOARD_SIZE - 1) || (y + k == BOARD_SIZE - 1))
				right_up = 0;
			if ((x == BOARD_SIZE - 1) || (y == BOARD_SIZE - 1) || board[x + k][y + k] != EMPTY)
				right_up = 0;
		}
		if (left_up){
			moves = legalMove(board, x, y, x - k, y + k, color, moves);
			if (moves == NULL)
				return NULL;
			if ((y + k == BOARD_SIZE - 1) || (x - k == 0))
				left_up = 0;
			if ((x == 0) || (y == BOARD_SIZE - 1) || board[x - k][y + k] != EMPTY)
				left_up = 0;
		}
		if (left_down){
			moves = legalMove(board, x, y, x - k, y - k, color, moves);
			if (moves == NULL)
				return NULL;
			if ((x - k == 0) || (y - k == 0))
				left_down = 0;
			if ((x == 0) || (y == 0) || board[x - k][y - k] != EMPTY)
				left_down = 0;
		}
		if (right_down){
			moves = legalMove(board, x, y, x + k, y - k, color, moves);
			if (moves == NULL)
				return NULL;
			if ((y - k == 0) || (x + k == BOARD_SIZE - 1))
				right_down = 0;
			if ((x == BOARD_SIZE - 1) || (y == 0) || board[x + k][y - k] != EMPTY)
				right_down = 0;
		}
	}
	return moves;
}


// creates a list of moves from <x,y> for some rook
Move *rookMovesList(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int color, Move *moves){
	int left = 1;
	int right = 1;
	int down = 1;
	int up = 1;
	for (int k = 1; k < BOARD_SIZE; k++){
		if (up){
			moves = legalMove(board, x, y, x, y + k, color, moves);
			if (moves == NULL)
				return NULL;
			if (y + k == BOARD_SIZE - 1)
				up = 0;
			if ((y == BOARD_SIZE - 1) || board[x][y + k] != EMPTY)
				up = 0;
		}
		if (down){
			moves = legalMove(board, x, y, x, y - k, color, moves);
			if (moves == NULL)
				return NULL;
			if (y - k == 0)
				down = 0;
			if ((y == 0) || board[x][y - k] != EMPTY)
				down = 0;
		}
		if (left){
			moves = legalMove(board, x, y, x - k, y, color, moves);
			if (moves == NULL)
				return NULL;
			if (x - k == 0)
				left = 0;
			if ((x == 0) || board[x - k][y] != EMPTY)
				left = 0;
		}
		if (right){
			moves = legalMove(board, x, y, x + k, y, color, moves);
			if (moves == NULL)
				return NULL;
			if (x + k == BOARD_SIZE - 1)
				right = 0;
			if ((x == BOARD_SIZE - 1) || board[x + k][y] != EMPTY)
				right = 0;
		}
	}
	if (board[x][y] == 'r' - (32 * (1 - color))){
		if (((isKingMove(color)) == 0) && board[4][7 - (7 * color)] == KING(color)){
			if (whichRook(color, 0) == 0 && x == 0 && y == 7 - (7 * color) && board[0][7 - (7 * color)] == ROOK(color)){
				moves = castelingMove(board, color, 0, moves);
				if (moves == NULL){
					return NULL;
				}
			}
			if (whichRook(color, 1) == 0 && x == 7 && y == 7 - (7 * color) && board[7][7 - (7 * color)] == ROOK(color)){
				moves = castelingMove(board, color, 1, moves);
				if (moves == NULL){
					return NULL;
				}
			}
		}
	}
	return moves;
}


// creates a list of moves from <x,y> for some Knight
Move *knightMovesList(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int color, Move *moves){
	for (int i = 0; i < BOARD_SIZE; i++){
		moves = legalMove(board, x, y, x + ((left_right(i) - 2)*(direction(i))), y + ((up_down(i) - 2)*(direction(BOARD_SIZE - 1 - i))), color, moves);
		if (moves == NULL)
			return NULL;
	}
	return moves;
}


// creates a list of moves from <x,y> for some Queen
Move *queenMovesList(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int color, Move *moves){
	moves = bishopMovesList(board, x, y, color, moves);
	moves = rookMovesList(board, x, y, color, moves);
	return moves;
}


// creates a list of moves from <x,y> for some King
Move *kingMovesList(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int color, Move *moves){
	int y2 = 0, x2 = 0;
	for (int i = 0; i < BOARD_SIZE; i++){
		y2 = (((direction_to_go(i)) / 10) - 1));
		x2 = (((direction_to_go(i)) % 10) - 1));
		moves = legalMove(board, x, y, x + x2, y + y2, color, moves);
		if (moves == NULL)
			return NULL;
	}
	return moves;
}


//checks for king move
int isKingMove(int color){
	if (color) return is_move_king_white;
	else return is_move_king_black;
}


//checks for rook move
int whichRook(int color, int side){
	if (color == 1 && side == 1) return is_move_right_rook_white;
	if (color == 1 && side == 0) return is_move_left_rook_white;
	if (color == 0 && side == 1) return is_move_right_rook_black;
	if (color == 0 && side == 0) return is_move_left_rook_black;
	return -1;
}


//creates a list of legal moves
Move *getMoves(char board[BOARD_SIZE][BOARD_SIZE], int color){
	Move *moves = (Move *)malloc(sizeof(Move));
	if (moves == NULL){
		printf("Error: standard function %s has failed","malloc");
		return NULL;
	}
	moves->type = 'a';
	moves->pos_start = NULL;
	moves->pos_end = NULL;
	moves->next = NULL;
	for (int x = 0; x < BOARD_SIZE; x++){
		for (int y = 0; y < BOARD_SIZE; y++){
			if (!color){
				switch (board[x][y]){
				case BLACK_B:
					moves = bishopMovesList(board, x, y, color, moves);
					break;
				case BLACK_K:
					moves = kingMovesList(board, x, y, color, moves);
					break;
				case BLACK_Q:
					moves = queenMovesList(board, x, y, color, moves);
					break;
				case BLACK_N:
					moves = knightMovesList(board, x, y, color, moves);
					break;
				case BLACK_R:
					moves = rookMovesList(board, x, y, color, moves);
					break;
				case BLACK_P:
					moves = pawnMovesList(board, x, y, color, moves);
					break;
				}
			}
			else{
				switch (board[x][y]){
				case WHITE_B:
					moves = bishopMovesList(board, x, y, color, moves);
					break;
				case WHITE_K:
					moves = kingMovesList(board, x, y, color, moves);
					break;
				case WHITE_Q:
					moves = queenMovesList(board, x, y, color, moves);
					break;
				case WHITE_N:
					moves = knightMovesList(board, x, y, color, moves);
					break;
				case WHITE_R:
					moves = rookMovesList(board, x, y, color, moves);
					break;
				case WHITE_P:
					moves = pawnMovesList(board, x, y, color, moves);
					break;
				}
			}
			if (moves == NULL) return NULL;
		}
	}
	if (moves->type == 'a'){
		int isCheckBool = isCheck(board, color);
		if (isCheckBool == 0) return NULL;
		if (isCheckBool == 1) moves->type = 'l';
		else moves->type = 't';
	}
	return moves;
}


//finds the king's position
Pos *kingPosition(char board[BOARD_SIZE][BOARD_SIZE], int color){
	Pos *pos = (Pos *)malloc(sizeof(Pos));
	if (pos == NULL){
		printf("Error: standard function %s has failed","malloc");
		return pos;
	}
	for (int x = 0; x < BOARD_SIZE; x++){
		for (int y = 0; y < BOARD_SIZE; y++){
			if (color && board[x][y] == WHITE_K){
				pos->x = x;
				pos->y = y;
				return pos;
			}
			if (color == 0 && board[x][y] == BLACK_K){
				pos->x = x;
				pos->y = y;
				return pos;
			}
		}
	}
	free(pos);
	return NULL;
}



// checks for check
int isCheck(char board[BOARD_SIZE][BOARD_SIZE], int color){
	Pos *pos = kingPosition(board, color);
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	if (pos == NULL){
		return 0;
	}
	if (color && (pos->y + 1)<BOARD_SIZE){
		if ((pos->x + 1)<BOARD_SIZE){
			if (board[pos->x + 1][pos->y + 1] == BLACK_P){
				free(pos);
				return 1;
			}
		}
		if ((pos->x - 1)>-1){
			if (board[pos->x - 1][pos->y + 1] == BLACK_P){
				free(pos);
				return 1;
			}
		}
	}
	if (color == 0 && (pos->y - 1)>-1){
		if ((pos->x + 1)<BOARD_SIZE){
			if (board[pos->x + 1][pos->y - 1] == WHITE_P){
				free(pos);
				return 1;
			}
		}
		if ((pos->x - 1)>-1){
			if (board[pos->x - 1][pos->y - 1] == WHITE_P){
				free(pos);
				return 1;
			}
		}
	}
	for (int i = 0; i < BOARD_SIZE; i++){
		x1 = ((direction(BOARD_SIZE - i - 1)))*((up_down(i)) - 2);
		y1 = ((direction(i)))*((left_right(i)) - 2);
		x2 = (((direction_to_go(i)) / 10) - 1));
		y2 = (((direction_to_go(i)) % 10) - 1));
		if ((0 <= pos->y + y1) && (pos->y + y1 < BOARD_SIZE) && (0 <= pos->x + x1) && (pos->x + x1< BOARD_SIZE)){
			if (board[pos->x + x1][pos->y + y1] == KNIGHT(!color)){
				free(pos);
				return 1;
			}
		}
		if ((0 <= pos->y + y2) && (pos->y + y2 < BOARD_SIZE) && (0 <= pos->x + x2) && (pos->x + x2< BOARD_SIZE)){
			if (board[pos->x + x2][pos->y + y2] == KING(!color)){
				free(pos);
				return 1;
			}
		}
		for (int j = 1; j < BOARD_SIZE; j++){
			y1 = j*(((direction_to_go(i)) / 10) - 1));
			x1 = j*(((direction_to_go(i)) % 10) - 1));
			if ((0 <= (pos->y + y1) && (pos->y + y1)<BOARD_SIZE) && (0 <= (pos->x + x1) && (pos->x + x1)<BOARD_SIZE)){
				if (board[pos->x + x1][pos->y + y1] != EMPTY){
					if ((board[pos->x + x1][pos->y + y1] == QUEEN(!color)) || (board[pos->x + x1][pos->y + y1] == type(!color, i))){
						free(pos);
						return 1;
					}
					break;
				}
			}
		}
	}
	free(pos);
	return -1;
}


//checks if there are no pieces between the king and the given rook
int isEmpty(char board[BOARD_SIZE][BOARD_SIZE], int color, int side){
	for (int i = 1; i < 4; i++){
		if ((4) + (i* (left_right(2 * (1 - side)) - 2)) > 0 && ((4) + (i* (left_right(2 * (1 - side)) - 2)) < (BOARD_SIZE - 1))){
			if (board[(4) + (i* ((left_right(2 * (1 - side)) - 2)))][(7 - (7 * color))] != EMPTY){
				return 0;
			}
		}
	}
	return 1;
}


// creates a list of castling moves
Move *castelingMove(char board[BOARD_SIZE][BOARD_SIZE], int color, int side, Move *moves){
	char boardCopy[BOARD_SIZE][BOARD_SIZE];
	Move *node;
	int flag = isCheck(board, color);
	if (flag == 0){
		freeMoves(moves);
		return NULL;
	}
	if (flag == -1){
		if (isEmpty(board, color, side)){
			copyBoard(board, boardCopy);
			makeMove(boardCopy, 7 * side, (7 - (7 * color)), (4) + (left_right(2 * (1 - side)) - 2), (7 - (7 * color)));
			makeMove(boardCopy, (4), (7 - (7 * color)), (4) + 2 * (left_right(2 * (1 - side)) - 2), (7 - (7 * color)));
			flag = isCheck(boardCopy, color);
			if (flag == 0){
				freeMoves(moves);
				return NULL;
			}
			if (flag == -1){
				node = createMove(7 * side, (7 - (7 * color)), (4) + (left_right(2 * (1 - side)) - 2), (7 - (7 * color)), 'c');
				if (node == NULL){
					freeMoves(moves);
					return NULL;
				}
				if (moves->type == 'a'){ freeMoves(moves); }
				else { node->next = moves; }
				moves = node;
			}
		}
	}
	return moves;
}


// prints a specific line of the gameboard
void printLine(){
	int i;
	printf(" |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}


void printBoard(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	printLine();
	for (j = BOARD_SIZE - 1; j >= 0; j--){
		printf((j < 9 ? "%d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++)
			printf("| %c ", board[i][j]);
		printf("|\n");
		printLine();
	}
	for (j = 0; j < BOARD_SIZE; j++){
		printf("   ");
		printf("%c", (char)('A' + j));
	}
	printf("  \n");
}

