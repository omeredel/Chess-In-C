
#include "minimax.h"
#include "Chess.h"
#include "Game.h"
#include "Moves.h"
#include <math.h>
#include <time.h>
#include <float.h>
#include "Files.h"
#include "string.h"
#include <stdio.h>

#define NullVal 10000000


//scoring function
int scoring(int color, char board[BOARD_SIZE][BOARD_SIZE]){
	int score = 0;
	int score_w = 0;
	int score_b = 0;
	for (int col = 0; col < BOARD_SIZE; col++){
		for (int row = 0; row < BOARD_SIZE; row++){
			if (board[row][col] == WHITE_P) score_w++;
			else if (board[row][col] == WHITE_B) score_w = score_w + 3;
			else if (board[row][col] == WHITE_N) score_w = score_w + 3;
			else if (board[row][col] == WHITE_R) score_w = score_w + 5;
			else if (board[row][col] == WHITE_Q) score_w = score_w + 9;
			else if (board[row][col] == WHITE_K) score_w = score_w + 100;
			else if (board[row][col] == BLACK_P) score_b++;
			else if (board[row][col] == BLACK_B) score_b = score_b + 3;
			else if (board[row][col] == BLACK_N) score_b = score_b + 3;
			else if (board[row][col] == BLACK_R) score_b = score_b + 5;
			else if (board[row][col] == BLACK_Q) score_b = score_b + 9;
			else if (board[row][col] == BLACK_K) score_b = score_b + 100;
		}
	}
	if (color == WHITE) score = score_w - score_b;
	else{ score = score_b - score_w; }
	return score;
}


//counts the number of moves
int countMoves(Move* moves){
	int counter = 0;
	if (moves->type == 'l' || moves->type =='t') return 0; //game ended
	while (moves != NULL){
		counter++;
		moves = moves->next;
	}
	return counter;
}


//puts the 6 best moves in depth 1 in the first of the list
Move *sortMoves(int color, char board[BOARD_SIZE][BOARD_SIZE], Move* moves){
	int len = countMoves(moves);
	int k = 6, max = 1000000, maxloc = 0;
	Move* retMoves = NULL;
	Move* temp = NULL;
	Move* m = moves;
	int * score = (int*)malloc(len*sizeof(int));
	if (score == NULL){
		printf("Error: standard function %s has failed","malloc");
		freeMoves(moves);
		return NULL;
	}
	int i = 0;
	while(m != NULL){ //there are moves
		char piece = board[m->pos_start->x][m->pos_start->y];
		char eatpiece = board[m->pos_end->x][m->pos_end->y];
		makeMove(board, m->pos_start->x, m->pos_start->y, m->pos_end->x, m->pos_end->y);
		score[i] = scoring(color, board);
		board[m->pos_start->x][m->pos_start->y] = piece;
		board[m->pos_end->x][m->pos_end->y] = eatpiece;
		m = m->next;
		i++;
	}
	if (k > len) k = len;
	for (int i = 0; i < k; i++){
		max = -1000000;
		maxloc = 0;
		Move* temp2 = moves;
		int j = 0;
		while(temp2 != NULL){
			if (score[j] >max){
				max = score[j];
				m = temp2;
				maxloc = j;
			}
			j++;
			temp2 = temp2->next;
		}
		if (maxloc == 0){
			moves = m->next;
			m->next = NULL;
		}
		else{
			Move *mov = moves;
			for (int i = 0; i < maxloc-1; i++){
				mov = mov->next;
			}
			mov->next = mov->next->next;
			m->next = NULL;
		}
		if (retMoves == NULL){
			retMoves = m;
			temp = m;
		}
		else{
			temp->next = m;
			temp = m;
		}
	}
	temp->next = moves;
	free(score);
	return retMoves;
}


//copy a given board
void copyBoard(char board[BOARD_SIZE][BOARD_SIZE], char boardCopy[BOARD_SIZE][BOARD_SIZE]){
	for (int col = 0; col < BOARD_SIZE; col++){
		for (int row = 0; row < BOARD_SIZE; row++){
			boardCopy[row][col] = board[row][col];
		}
	}
}


//checks for check or tie and returns leaf's score accordingly
int leafScore(Move* move){
	if (move->type == 'l') return -600;
	else if (move->type == 't') return -555;
	else return 0;
}


//clones a move
Move* cloneMove(Move *move){
	Move *retMove = (Move*)malloc(sizeof(Move));
	if (retMove == NULL){
		printf("Error: standard function %s has failed","malloc");
		return NULL;
	}
	Pos *p1 = (Pos*)malloc(sizeof(Pos));
	if (p1 == NULL){ 
		printf("Error: standard function %s has failed","malloc");
		free(retMove);
		return NULL;
	}
	Pos *p2 = (Pos*)malloc(sizeof(Pos));
	if (p2 == NULL){
		printf("Error: standard function %s has failed","malloc");
		free(retMove);
		free(p1);
		return NULL;
	}
	p1->x = move->pos_start->x;
	p1->y = move->pos_start->y;
	p2->x = move->pos_end->x;
	p2->y = move->pos_end->y;
	retMove->next = NULL;
	retMove->type = move->type;
	retMove->pos_start = p1;
	retMove->pos_end = p2;

	return retMove;
}


//makes one move - from <x1,y1> to <x2,y2>
void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2){
	board[x2][y2] = board[x1][y1];
	board[x1][y1] = EMPTY;
}


//frees a given list of moves
void freeMoves(Move* moves){
	Move* current = moves;
	while (moves != NULL){
		moves = moves->next;
		free(current->pos_start);
		free(current->pos_end);
		free(current);
		current = moves;
	}
}


//checks if a given move is a promotion move
int isPromotion(Move *m, int color, char board[BOARD_SIZE][BOARD_SIZE]){
	if (color && board[m->pos_start->x][m->pos_start->y] == 'm'){
		if (m->pos_end->y == 7) return 1;
	}
	else if (!color && board[m->pos_start->x][m->pos_start->y] == 'M'){
		if (m->pos_end->y == 0) return 1;
	}
	return 0;
}


//checks if a given move is a promotion move, for the minimax function
int isPromotionMin(Move *m, int color){
	if (color && m->type == 'm'){
		if (m->pos_end->y == 7) return 1;
	}
	else if (!color && m->type == 'M'){
		if (m->pos_end->y == 0)return 1;
	}
	return 0;
}


//puts legal promotion moves pieces
char whichPiece(int i, int color){
	if (color){
		if (i == 0){ return('b'); }
		else if (i == 1){ return('n'); }
		else if (i == 2){ return('r'); }
		else if (i == 3){ return('q'); }
	}
	else{
		if (i == 0){ return('B'); }
		else if (i == 1){ return('N'); }
		else if (i == 2){ return('R'); }
		else if (i == 3){ return('Q'); }
	}
	return '\0';
}


unsigned int boardConter = 1;


//makes a list of legal promotion moves
Move* promotion(int color, Move *moves){
	Move* retList = NULL;
	Move* current = NULL;
	for (int i = 0; i < 4; i++){
		if (retList == NULL){
			retList = cloneMove(moves);
			if (retList == NULL){
				return NULL;
			}
			retList->type = whichPiece(i, color);
			current = retList;
		}
		else{
			current->next = cloneMove(moves);
			if (current->next == NULL){
				free(retList);
				return NULL;
			}
			current = current->next;
			current->type = whichPiece(i, color);
		}
	}
	return retList;
}


//if there exists a mate move, puts it first
Move *firstMove(int color,Move* m,char board[BOARD_SIZE][BOARD_SIZE]){
	Move *m3 = NULL;
	Move *moves = m;
	Move * pre = NULL;
	char piece, eatpiece; 
	while(moves !=NULL){
		piece = board[moves->pos_start->x][moves->pos_start->y];
		eatpiece = board[moves->pos_end->x][moves->pos_end->y];
		if (moves->type == 'c'){
			makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
			makeMove(board, 4, 7 * (1 - color), 4 - (2 * (left_right(moves->pos_start->x) - 2)), 7 * (1 - color));
		}
		else{
			makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
			board[moves->pos_end->x][moves->pos_end->y] = moves->type;
		}
		m3 = getMoves(board, 1-color);
		board[moves->pos_start->x][moves->pos_start->y] = piece;
		board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
		if (moves->type == 'c'){
			board[4][7 * (1 - color)] = 'k' - (32 * (1 - color));
			board[(4) - (2 * (left_right(moves->pos_start->x) - 2))][7 * (1 - color)] = EMPTY;
		}
		if (m3 == NULL) return NULL;
		if (m3->type == 'l'){
			freeMoves(m3);
			if (pre == NULL) return m;
			else{
				pre->next = moves->next;
				moves->next=m;
				m= moves;
				return m;
			}			
		}
		freeMoves(m3);
		pre = moves;
		moves =moves->next;
	}
	return m;
}


//minimax function
Move *minimax(Move *move, unsigned int depth, char board[BOARD_SIZE][BOARD_SIZE], int color, char score, int bestM){
	int val = 0, tmpVal = -10000;
	int lowB = 10000;
	int upB = -10000;
	int max = 1;
	if (move == NULL) return NULL; //memory failure
	if (move->next == NULL && score == '\0'){ 
		Move* m = cloneMove(move);
		if (m == NULL){
			freeMoves(move); 
			return NULL;
		}
		freeMoves(move);
		return m; 
	} //only one move
	Move *moves = move;
	Move *best = NULL;
	Move *current = NULL;
	Move* previous = NULL;
	Move *t = NULL;
	while (moves != NULL){
		lowB = 10000;
		upB = -10000;
		boardConter++;
		if (bestM && boardConter > 1000000){
			Move *mbest = cloneMove(move);
			if (mbest == NULL){
				freeMoves(move);
				freeMoves(best);
				return NULL;
			}
			mbest->type = 'I';
			freeMoves(move);
			freeMoves(best);
			return mbest;
		}
		char piece = board[moves->pos_start->x][moves->pos_start->y];
		char eatpiece = board[moves->pos_end->x][moves->pos_end->y];
		if (isPromotionMin(moves, color)){ //promotion
			if (score == '\0' ){ //the user didn't give an input
				t = promotion(color, moves);
				if (t == NULL){ 
					freeMoves(move);
					return NULL; }
				if (previous == NULL){
					t->next->next->next->next = moves->next;
					moves->next = NULL;
					move = t;
				}
				else{
					previous->next = t;
					t->next->next->next->next = moves->next;
					moves->next = NULL;
				}
				freeMoves(moves);
				moves = t;
			}
			else{ //user chose to promote the pawn
				moves->type= score;
			}
			makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
			board[moves->pos_end->x][moves->pos_end->y] = moves->type;
			val = minimax_rec(depth - 1, board, 1 - max, color, lowB, upB, bestM);
			board[moves->pos_start->x][moves->pos_start->y] = piece;
			board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
			if (val == NullVal){ //memory leak
				freeMoves(move);
				freeMoves(best);
				return NULL;
			}
		}
		else{
			if (moves->type == 'c'){
				makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
				makeMove(board, 4, 7 * (1 - color), 4 - (2 * (left_right(moves->pos_start->x) - 2)), 7 * (1 - color));
			}
			else{
				makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
				board[moves->pos_end->x][moves->pos_end->y] = moves->type;
			}
			val = minimax_rec(depth - 1, board, 1 - max, color, lowB, upB, bestM);
			board[moves->pos_start->x][moves->pos_start->y] = piece;
			board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
			if (moves->type == 'c'){
				board[4][7 * (1 - color)] = 'k' - (32 * (1 - color));
				board[(4) - (2 * (left_right(moves->pos_start->x) - 2))][7 * (1 - color)] = EMPTY;
			}
			if (val == NullVal){//memory leak
				freeMoves(move);
				freeMoves(best);
				return NULL;
			}
		}
		if (bestM && (val == valBest || boardConter >1000000)){
			Move *mbest = cloneMove(move); 
			if (mbest == NULL){
				freeMoves(move);
				freeMoves(best);
				return NULL;
			}
			mbest->type = 'I';
			freeMoves(move);
			freeMoves(best);
			return mbest;
		}
		if (val > tmpVal){
			tmpVal = val;
			freeMoves(best);
			best = cloneMove(moves);
			if (best == NULL){
				freeMoves(move);
				return NULL;
			}
			current = best;
		}
		else if (val == tmpVal){
			current->next = cloneMove(moves);
			if (current->next == NULL){
				freeMoves(move);
				freeMoves(best);
				return NULL;
			}
			current = current->next;
		}
		previous = moves;
		moves = moves->next;
	}
	if (score!='\0'){
		char *piecetypeb4 = whichType(board[moves->pos_start->x][moves->pos_start->y]);
		printf("Computer: move %s at <%c,%c> to <%c,%c>\n", piecetypeb4,
				(moves->pos_start->y) + '1',(moves->pos_start->x) + 'A',
				(moves->pos_end->y) + '1', (moves->pos_end->x) + 'A');
	}
	freeMoves(move);
	return best;
}


int minimax_rec(unsigned int depth, char board[BOARD_SIZE][BOARD_SIZE], int max, int color, int lowB, int upB, int bestM){
	int val = 0, v = 0;
	Move *moves = NULL;
	Move *head;
	Move *t = NULL;
	Move *previous = NULL;
	if (depth == 0){
		v = scoring(color, board);
		int f=0;
		if (max) f = isCheck(board, color);
		else f = isCheck(board, 1-color);
		if (f==0) return NullVal;
		if (f==1){
			if (max) moves = getMoves(board, color);
			else moves = getMoves(board, 1-color);
			if (moves == NULL) return NullVal; //memory leak
			int leaf = leafScore(moves);
			if (leaf != 0){
				if (max) v =leaf;
				else{
					if (leaf == -555) v = -555;
					else v= -leaf;
				}
			} //no moves possible
			freeMoves(moves);
		}
		return v;
	}
	if (max) moves = getMoves(board, color);
	else moves = getMoves(board, 1-color);
	if (moves == NULL) return NullVal;  //memory leak
	int leaf = leafScore(moves);
	if (leaf != 0){
		freeMoves(moves);
		if (max) return leaf;
		else{
			if (leaf == -555) return -555;
			else return -leaf;
		}
	} //no moves possible
	if (bestM){
		if (max) moves = sortMoves(color, board, moves);
		else moves = sortMoves(1 - color, board, moves);
	}
	head = moves;
	if (max){//max case
		while (moves != NULL){
			boardConter++;
			if (bestM){
				if (boardConter > 1000000) return valBest;
			}
			char piece = board[moves->pos_start->x][moves->pos_start->y];
			char eatpiece = board[moves->pos_end->x][moves->pos_end->y];
			if (isPromotionMin(moves, color)){//promotion
				t = promotion(color, moves);
				if (t == NULL){ return NullVal; }
				if (previous == NULL){
					t->next->next->next->next = moves->next;
					moves->next = NULL;
					head = t;
				}
				else{
					previous->next = t;
					t->next->next->next->next = moves->next;
					moves->next = NULL;
				}
				freeMoves(moves);
				moves = t;
				makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
				board[moves->pos_end->x][moves->pos_end->y] = moves->type;
				val = minimax_rec(depth - 1, board, 1 - max, color, lowB, upB, bestM);
				board[moves->pos_start->x][moves->pos_start->y] = piece;
				board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
				if (val == NullVal){//memory leak
					//freeMoves(move);
					return NullVal;
				}
			}
			else{
				if (moves->type == 'c'){
					makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
					makeMove(board, 4, 7 * (1-color), (4) - (2 * (left_right(moves->pos_start->x) - 2)), 7 *(1- color));
				}
				else{
					makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
					board[moves->pos_end->x][moves->pos_end->y] = moves->type;
				}
				val = minimax_rec(depth - 1, board, 1 - max, color, lowB, upB, bestM);
				board[moves->pos_start->x][moves->pos_start->y] = piece;
				board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
				if (moves->type == 'c'){
					board[4][7 * (1 - color)] = 'k' - (32*(1 - color));
					board[(4) -(2 * (left_right(moves->pos_start->x) - 2))][7 *(1- color)] = EMPTY;
				}
				if (val == NullVal){//memory leak
					freeMoves(head);
					return NullVal;
				}
			}
			if (bestM && ( val == valBest || boardConter >1000000)){
				freeMoves(head);
				return valBest;}
			if (val > upB){ upB = val; }
			if (upB >= lowB){ 
				freeMoves(head);
				return upB; }
			previous = moves;
			moves = moves->next;
		}
		freeMoves(head);
		return upB;
	}
	else{ //min case
		while (moves != NULL){
			boardConter++;
			if (bestM){
				if (boardConter > 1000000){ return valBest; }
			}
			char piece = board[moves->pos_start->x][moves->pos_start->y];
			char eatpiece = board[moves->pos_end->x][moves->pos_end->y];
			if (isPromotionMin(moves, 1 - color)){
				t = promotion(1-color, moves);
				if (t == NULL){ return NullVal; }
				if (previous == NULL){
					t->next->next->next->next = moves->next;
					moves->next = NULL;
					head = t;
				}
				else{
					previous->next = t;
					t->next->next->next->next = moves->next;
					moves->next = NULL;
				}
				freeMoves(moves);
				moves = t;
				makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
				board[moves->pos_end->x][moves->pos_end->y] = moves->type;
				val = minimax_rec(depth - 1, board, 1 - max, color, lowB, upB, bestM);
				board[moves->pos_start->x][moves->pos_start->y] = piece;
				board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
				if (val == NullVal){//memory leak
					//freeMoves(move);
					return NullVal;

				}
			}
			else{
				if (moves->type == 'c'){
					makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
					makeMove(board, 4, 7 * color, (4) - (2 * (left_right(moves->pos_start->x) - 2)), 7 * color);
				}
				else{
					makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
					board[moves->pos_end->x][moves->pos_end->y] = moves->type;
				}
				val = minimax_rec(depth - 1, board, 1 - max, color, lowB, upB, bestM);
				board[moves->pos_start->x][moves->pos_start->y] = piece;
				board[moves->pos_end->x][moves->pos_end->y] = eatpiece;
				if (moves->type == 'c'){
					board[4][7 * color] = 'k' - (32 * color);
					board[(4) - (2 * (left_right(moves->pos_start->x) - 2))][7 * color] = EMPTY;
				}
				if (val == NullVal){//memory leak
					freeMoves(head);
					return NullVal;
				}
			}
			if (bestM && (val == valBest || boardConter >1000000)){
				freeMoves(head);
				return valBest;}
			if (val < lowB){ lowB = val; }
			if (upB >= lowB){ 
				freeMoves(head);
				return lowB; }
			previous = moves;
			moves = moves->next;
		}
		freeMoves(head);
		return lowB;
	}
}


Move* minimaxManager(char board[BOARD_SIZE][BOARD_SIZE], Move* moves, int color, unsigned int depth, char score){
	Move *moveToMake= NULL;
	moveToMake = minimax(moves, depth, board, color, score, 0);
	if (moveToMake == NULL){
		freeMoves(moves);
		return NULL;
	}
	moveToMake = firstMove(color,moveToMake,board);
	if (moveToMake == NULL) return NULL;
	return moveToMake;
}


