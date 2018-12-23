#include "Moves.h"
#include "minimax.h"
#include "Game.h"
#include "Chess.h"
#include "string.h"
#include <stdio.h>
#include "Files.h"
#include <SDL.h>

char History[HISTORY_SIZE*2][6];
int history_moves = 0;


// creates a list of moves that are available from position <x,y>
Move *xyMoves(char board[BOARD_SIZE][BOARD_SIZE], int color, int x, int y){
	Move *moves = getMoves(board, color);
	if (moves == NULL)return NULL; //failure
	Move *head = moves;
	Move *xyMovesList = NULL;
	Move *xyNewMoves;
	if (moves->type == 'l' || moves->type == 't'){
		return moves;
	}
	while (head != NULL){
		if (x == head->pos_start->x && y == head->pos_start->y){
			xyNewMoves = cloneMove(head);
			if (xyNewMoves == NULL){
				freeMoves(moves);
				if (xyMovesList != NULL)
					freeMoves(xyMovesList);
				return NULL;
			}
			xyNewMoves->next = xyMovesList;
			xyMovesList = xyNewMoves;
		}
		head = head->next;
	}
	freeMoves(moves);
	if (xyMovesList == NULL)
		xyMovesList = createMove(10, 10, 10, 10, '\0');
	return xyMovesList;
}


// makes the move according to the parameters if it is a legal move
int move(char board[BOARD_SIZE][BOARD_SIZE], int color, int x, int y, int xnew, int ynew, char c){
	if (color && (isBlack(board[x][y] )|| board[x][y]==' ')&& c != 'c'){
		printf("%s", "The specified position does not contain your piece\n");
		return 2;
	}
	if (color==0 && (isWhite(board[x][y]) || board[x][y] == ' ') && c != 'c'){
		printf("%s", "The specified position does not contain your piece\n");
		return 2;
	}
	Move *xy_moves = xyMoves(board, color, x,  y);
	if (xy_moves == NULL) return 0;
	Move *head = xy_moves;
	while (head != NULL){
		if (xnew == head->pos_end->x && ynew == head->pos_end->y){
			saveMove(y, x, ynew, xnew, board);
			if (c == 'c'&& head->type=='c'){
				int xnew1 = 4  - (2 * (left_right(x) - 2));
				makeMove(board, 4 , 7 * (1-color), xnew1, 7 * (1-color));
				updateCastling(board, 4, 7 * (1 - color));
				makeMove(board, x, y, xnew, ynew);
				updateCastling(board, x, y);
				freeMoves(xy_moves);
				return 1;
			}
			else{
				if (ynew == 7 && color && board[x][y] == 'm'){ //promotion in hand
					makeMove(board, x, y, xnew, ynew);
					updateCastling(board, x, y);
					board[xnew][ynew] = selectPromotionPiece(color);
					freeMoves(xy_moves);
					return 1;
				}
				else{
					if (ynew == 0 && color == 0 && board[x][y] == 'M'){
						makeMove(board, x, y, xnew, ynew);
						updateCastling(board, x, y);
						board[xnew][ynew] = selectPromotionPiece(color);
						freeMoves(xy_moves);
						return 1;
					}
					else{
						if (board[x][y] == head->type && c!='c'){
							makeMove(board, x, y, xnew, ynew);
							updateCastling(board, x, y);
							freeMoves(xy_moves);
							return 1;
						}
					}
				}
			}
		}
		head = head->next;
	}
	freeMoves(xy_moves);
	printf("%s", ILLEGAL_MOVE);
	return 2;
}


//lets the user choose which piece he wants to promote his pawn to
char selectPromotionPiece(int color){
	int i = 0;
	char c;
	while (1){
		i = 0;
		char *promotion = (char*)malloc(51*sizeof(char));
		if (promotion == NULL){
			printf("Error: standard function %s has failed","malloc");
			exit(0);
			return 'E';
		}
		if(isGui == 1){
			int buttonid = promotionIfGui();
			switch(buttonid){
			case 0:{
				strcpy(promotion, "pawn");
				strcat(promotion, "\0");
				break;
			}
			case 1:{
				strcpy(promotion, "queen");
				strcat(promotion, "\0");
				break;
			}
			case 2:{
				strcpy(promotion, "rook");
				strcat(promotion, "\0");
				break;
			}
			case 3:{
				strcpy(promotion, "bishop");
				strcat(promotion, "\0");
				break;
			}
			case 4:{
				strcpy(promotion, "knight");
				strcat(promotion, "\0");
				break;
			}
			}
		}else{
			print_message(ENTER_PROM);
			while ((c = getchar()) != '\n'){
				promotion[i] = c;
				i++;
			}
			promotion[i] = '\0';
		}
		removeSpaces(promotion);
		if (strstr(promotion, "queen") != NULL){
			free(promotion);
			if(color) return WHITE_Q;
			else return BLACK_Q;
		}
		else if (strstr(promotion, "rook") != NULL){
			free(promotion);
			if(color) return WHITE_R;
			else return BLACK_R;
		}
		else if (strstr(promotion, "bishop") != NULL){
			free(promotion);
			if(color) return WHITE_B;
			else return BLACK_B;
		}
		else if (strstr(promotion, "knight") != NULL){
			free(promotion);
			if(color) return WHITE_N;
			else return BLACK_N;
		}
		else if (strstr(promotion, "pawn") != NULL){
			free(promotion);
			if(color) return WHITE_P;
			else return BLACK_P;
		}
		else print_message(INVALID_TYPE);
		free(promotion);
		return 'E';
	}
}

Move* addPromotion(Move *m,int color){
	Move* moves = m;
	Move* t = NULL;
	Move* pre = NULL;
	while(moves != NULL){
		if (isPromotionMin(moves, color)){
			t = promotion(color, moves);
			if (t == NULL){
				freeMoves(m);
				return NULL; }
			if(pre == NULL){	
				t->next->next->next->next = moves->next;
				moves->next = NULL;
				m=t;

			}
			else{
				pre->next = t;
				t->next->next->next->next = moves->next;
				moves->next = NULL;
			}
			freeMoves(moves);
			moves = t;
		}
		pre = moves;
		moves = moves->next;
	}
	return m;
}

int promotionIfGui(){
	const SDL_MessageBoxButtonData buttons[] = {
			{ 0, 0, "Pawn" },
			{ 0, 1, "Queen" },
			{ 0, 2, "Rook" },
			{ 0, 3, "Bishop"},
			{ 0, 4, "Knight"},
	};
	const SDL_MessageBoxColorScheme colorScheme = {
			{{ 255,   0,   0 },
			{   0, 255,   0 },
			{ 255, 255,   0 },
			{   0,   0, 255 },
			{ 255,   0, 255 }}
	};
	const SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION,
			NULL,
			"Promote your pawn",
			"which piece do you want instead of your pawn",
			SDL_arraysize(buttons),
			buttons,
			&colorScheme
	};
	int buttonID;
	SDL_ShowMessageBox(&messageboxdata, &buttonID);
	return buttonID;
}


//gets the score for a specific move
int getScore(char board[BOARD_SIZE][BOARD_SIZE], int color, unsigned int depth, int x, int y, int xnew, int ynew, char ch){
	char boardCopy[BOARD_SIZE][BOARD_SIZE];
	Move *moves;
	Move *movesBest;
	copyBoard(board, boardCopy);
	int flag = move(boardCopy, color, x, y, xnew, ynew, ch);
	if (flag == 0) return 0;
	if (flag == 1){
		copyBoard(board, boardCopy);
		moves = createMove(x, y, xnew, ynew, ch);
		if (moves == NULL) return 0;
		movesBest = minimaxManager(boardCopy, moves, color, depth , 'c');
		if (movesBest == NULL) return 0;
		freeMoves(movesBest);
	}
	return 2;
}


// saves the game
int save(char board[BOARD_SIZE][BOARD_SIZE],char *str){
	FILE *file = fopen(str, "w");
	if (file == NULL){
		print_message(FILE_PROBLEM);
		return 0;
	}
	createXML(file, board);
	if (fclose(file)) return 0;
	return 1;
}


// copies source string to target string
void copyString(char *target, char *source) {
	while (*source != ' ' && *source != '\0') {
		*target = *source;
		source++;
		target++;
	}
	*target = '\0';
}


// calls the function that needs to be according to the string
int callFunction(char board[BOARD_SIZE][BOARD_SIZE], int depth, char *str, int color){
	int result = 0;
	char* pointer1 = strstr(str, "<");
	char* pointer = strstr(str, ">");
	if (pointer == NULL || pointer1 == NULL) return 0;

	if (strstr(str, "castle") != NULL){
		int row = (int)((&str[8])[0] - 'A');
		int xnew = 4 + (left_right(2 * (7 - row)) - 2);
		int flag=0;		
		if (depth==0)		
			flag = move(board, turn, str[8] - 'A', str[10] - '1', xnew, str[11] - '1', 'c');
		else
			flag = getScore(board, color, depth, str[8] - 'A', str[10] - '1', xnew, str[10] - '1', 'c');
		return flag;
	}

	char s1[51];
	copyString(s1, pointer1);
	if (s1[1] > 'H' || s1[4] != '>' || s1[3] == '0' || s1[3] == '9'){
		printf("%s", WRONG_POSITION);
		return 2;
	}
	char s2[51];
	copyString(s2, pointer + 5);
	if (s2[1] > 'H' || s2[4] != '>' || s2[3] == '0' || s2[3] == '9'){
		printf("%s", WRONG_POSITION);
		return 2;
	}
	if (depth == 0){
		if ((pointer + 10)[0] == '\0')
			result = move(board, color, s1[1] - 'A', s1[3] - '1', s2[1] - 'A', s2[3] - '1', '\0');
		else
			result = move(board, color, s1[1] - 'A', s1[3] - '1', s2[1] - 'A', s2[3] - '1', (pointer + 11)[0]);
	}
	else{
		if ((pointer + 10)[0] == '\0'){
			if ((s2[3] - '1' == 7 *color) && board[s1[1] - 'A'][s1[3] - '1'] == ('m' - (32 * (1 - color))))
				result = getScore(board, color, depth, s1[1] - 'A', s1[3] - '1', s2[1] - 'A', s2[3] - '1', 'q' - (32 * (1 - color)));
			else
				result = getScore(board, color, depth, s1[1] - 'A', s1[3] - '1', s2[1] - 'A', s2[3] - '1', board[s1[1] - 'A'][s1[3] - '1']);
		}
		else{
			char c = ((pointer + 11)[0] - (32 * (1 - color)));
			result=getScore(board, color, depth, s1[1] - 'A', s1[3] - '1', s2[1] - 'A', s2[3] - '1', c);
		}
	}
	return result;
}


//gets a command from the user and responds accordingly
int UserState(char board[BOARD_SIZE][BOARD_SIZE]){
	int i = 0;
	char ch;
	char *command = (char*)malloc(51 * sizeof(char));
	if (command == NULL){
		printf("Error: standard function %s has failed","malloc");
		return 0;
	}
	while ((ch = getchar()) != '\n'){
		command[i] = ch;
		i++;
	}
	command[i] = '\0';
	removeSpaces(command);
	if (strstr(command, "move") != NULL){
		char tempch = command[6];		//switching to x,y instead of y,x
		command[6] = command[8];
		command[8] = tempch;
		tempch = command[15];
		command[15] = command[17];
		command[17] = tempch;
		int flag = callFunction(board, 0, &command[5], turn);
		free(command);
		return flag;
	}
	else{
		if (strstr(command, "save") != NULL){
			if (save(board, &command[5]) == 1){
				free(command);
				return 2;
			}
			free(command);
			return 0;
		}
		else{
			if (strstr(command, "quit") != NULL){
				free(command);
				printf("%s","Exiting...\n");
				return 0;
			}else{
				if (strstr(command, "castle") != NULL){
					char tempch = command[8];		//switching to x,y instead of y,x
					command[8] = command[10];
					command[10] = tempch;
					int row = (int)((&command[8])[0] - 'A');
					int xnew = (4) + (left_right(2 * (7 - row)) - 2);
					int flag = move(board, turn, (&command[8])[0] - 'A', (&command[10])[0] - '1', xnew, (&command[10])[0] - '1', 'c');
					free(command);
					return flag;
				}
				else{
					if (strstr(command, "reset") != NULL){
						free(command);
						printf("Restarting...\n");
						restart(board);
						printBoard(board);
						SettingsState(board);
						return GameState(board);
					}
					else if(strstr(command, "undo") != NULL){
						if (game_mode == 2){
							printf("Undo command not available in 2 players mode\n");
							free(command);
							return 2;
						}
						else if (history_moves == 0){
							printf("Empty history, move cannot be undone\n");
							free(command);
							return 2;
						}
						else{
							free(command);
							undoMove(board);
							return 2;
						}
					}
				}
			}
		}
		printf("%s", ILLEGAL_COMMAND);
		free(command);
		return 2;
	}
}


// initializes the variable for the piece if they can be used for castling
void initCastling(char Board[BOARD_SIZE][BOARD_SIZE]){
	is_move_king_white = 0;
	is_move_king_black = 0;
	is_move_right_rook_white = 0;
	is_move_left_rook_white = 0;
	is_move_right_rook_black = 0;
	is_move_left_rook_black = 0;
	if (Board[0][0] != 'r') is_move_left_rook_white = 1;
	if (Board[7][0] != 'r') is_move_right_rook_white = 1;
	if (Board[4][0] != 'k') is_move_king_white = 1;
	if (Board[0][7] != 'R') is_move_left_rook_black = 1;
	if (Board[7][7] != 'R') is_move_right_rook_black = 1;
	if (Board[4][7] != 'K') is_move_king_black = 1;
}


// updates if the piece can be used for castling after a move
void updateCastling(char board[BOARD_SIZE][BOARD_SIZE], int x, int y){
	if (x == 0 && y == 0 && board[x][y] == 'r')
		is_move_left_rook_white = 1;
	if (x == 7 && y == 0 && board[x][y] == 'r')
		is_move_right_rook_white = 1;
	if (x == 3 && y == 0 && board[x][y] == 'k')
		is_move_king_white = 1;
	if (x == 0 && y == 7 && board[x][y] == 'R')
		is_move_left_rook_black = 1;
	if (x == 7 && y == 7 && board[x][y] == 'R')
		is_move_right_rook_black = 1;
	if (x == 4 && y == 7 && board[x][y] == 'K')
		is_move_king_black = 1;
}


// initializes the history
void initHistory(){
	for (int i=0; i<HISTORY_SIZE*2; i++){
		for (int j=0; j<HISTORY_LENGTH ; j++){
			History[i][j] = '\0';
		}
	}
	history_moves = 0;
}


//undoes the last 2 moves
void undoMove(char board[BOARD_SIZE][BOARD_SIZE]){
	char *compMove = History[0];
	char *userMove = History[1];
	history_moves--;
	history_moves--;
	undoGivenMove(compMove, board, !user_color);
	undoGivenMove(userMove, board, user_color);
	popHistoryStack();
}


//undoes a specific move
void undoGivenMove(char *moveToUndo, char board[BOARD_SIZE][BOARD_SIZE], int color){
	int fromX = moveToUndo[0] - '1';
	int fromY = moveToUndo[1]-'A';
	int toX = moveToUndo[2] - '1';
	int toY = moveToUndo[3] - 'A';
	char pieceMoved = board[toY][toX];
	char pieceDeleted = moveToUndo[4];
	board[fromY][fromX] = pieceMoved;
	board[toY][toX] = pieceDeleted;
	printf("Undo move for player %s : <%c,%c> -> <%c,%c>\n", which_color(color), fromX + '1', fromY + 'A', toX + '1', toY + 'A');
}


//Orders the history stack
void popHistoryStack(){
	int i, j;
	for (i = 0; i < history_moves; i++){
		for (j = 0; j<HISTORY_LENGTH ; j++){
			History[i][j] = History[i+2][j];
		}
	}
	for (i = history_moves; i < (HISTORY_SIZE*2) ; i++){
		for (j = 0; j<HISTORY_LENGTH ; j++){
			History[i][j] = '\0';
		}
	}
}


//Organizes the history stack before inserting a new move
void insertHistoryStack(){
	for (int i=history_moves; i > 0; i--){
		for (int j=0; j<HISTORY_LENGTH ; j++){
			History[i][j] = History[i-1][j];
		}
	}
	if (history_moves != HISTORY_SIZE*2){
		history_moves++;
	}
}


//saves a move
void saveMove(int x, int y, int xnew, int ynew, char board[BOARD_SIZE][BOARD_SIZE]){
	if (history_moves != 0) insertHistoryStack();
	else if (history_moves != HISTORY_SIZE*2) history_moves++;
	History[0][0] = ('1' + x);
	History[0][1] = ('A' + y);
	History[0][2] = ('1' + xnew);
	History[0][3] = ('A' + ynew);
	History[0][4] = board[ynew][xnew];
	History[0][5] = '\0';
}


// game state function
int GameState(char board[BOARD_SIZE][BOARD_SIZE]){
	initCastling(board);
	initHistory();
	int flag = 2;
	Move *moves;
	while (1){
		if (turn == user_color){
			flag = 2;
			while (flag != 1){
				printf("%s player - enter your move:\n", which_color_lower(turn));
				flag = UserState(board);
				if (flag == 0){ exit(0); }
			}
			printBoard(board);
			turn = 1-turn;
		}
		moves = getMoves(board, turn);
		if (moves == NULL){return 0;}
		else{
			if (moves->type == 'l'){
				printf("Checkmate! %s player wins the game\n", which_color(1 - turn));
				freeMoves(moves);
				return 1;
			}
			if (moves->type == 't'){
				printf("%s", TIE);
				freeMoves(moves);
				return 1;
			}
			freeMoves(moves);
			int isCheckBool = isCheck(board, turn);
			if (isCheckBool == 0){ return 0; }
			if (isCheckBool == 1){ printf("Check: %s King is threatened!\n", which_color_lower(turn)); }
		}
		if (game_mode == 2){
			flag = 2;
			while (flag != 1){
				printf("%s player - enter your move:\n", which_color_lower(turn));
				flag = UserState(board);
				if (flag == 0){ exit(0); }
			}
			turn = 1-turn;
		}
		else{
			moves = getMoves(board, turn);
			if (moves == NULL) return 0;
			moves = minimaxManager(board, moves, turn, difficulty, '\0');
			if (moves == NULL) return 0;
			if (moves->type == 'c'){
				makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
				updateCastling(board, moves->pos_start->y, moves->pos_start->x);
				makeMove(board, 4, 7 * (1-turn), 4  + (2 * (left_right(1-turn) - 2)), 7 * (1-turn));
				updateCastling(board, 7 * (1-turn), 4);
				printf("Computer: castle King at <%c,%c> and Rook at <%c,%c>\n",
						(7*(1-turn)) + '1', (4 + (2 * (left_right(1-turn) - 2))) + 'A',
						(moves->pos_end->y) + '1',(moves->pos_end->x) + 'A');
			}
			else{
				char *piecetypeb4 = whichType(board[moves->pos_start->x][moves->pos_start->y]);
				saveMove((moves->pos_start->y),(moves->pos_start->x), (moves->pos_end->y), (moves->pos_end->x), board);
				makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
				updateCastling(board, moves->pos_start->x, moves->pos_start->y);
				char *piecetypeafter = whichType(board[moves->pos_end->x][moves->pos_end->y]);
				if(!strcmp(piecetypeb4, piecetypeafter)){
					printf("Computer: move %s at <%c,%c> to <%c,%c>\n", piecetypeb4,
							(moves->pos_start->y) + '1',(moves->pos_start->x) + 'A',
							(moves->pos_end->y) + '1', (moves->pos_end->x) + 'A');
				}
				else{
					printf("Computer: move %s at <%c,%c> to <%c,%c> and promote to %s\n", piecetypeb4,
							(moves->pos_start->y) + '1', (moves->pos_start->x) + 'A',
							(moves->pos_end->y) + '1', (moves->pos_end->x) + 'A',  piecetypeafter);
				}
			}
			turn = 1 - turn;
			freeMoves(moves);
		}
		printBoard(board);
		moves = getMoves(board, turn);
		if (moves == NULL) return 0;
		else{
			if (moves->type == 'l'){
				printf("Checkmate! %s player wins the game\n", which_color_lower(1 - turn));
				freeMoves(moves);
				return 1;

			}
			if (moves->type == 't'){
				printf("%s", TIE);
				freeMoves(moves);
				return 1;
			}
			freeMoves(moves);
			int isCheckBool = isCheck(board, turn);
			if (isCheckBool == 0) return 0;
			if (isCheckBool == 1) printf("Check: %s King is threatened!\n", which_color_lower(turn));
		}
	}
}


// initializes gameboard
void initBoard(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (j == 0){
				if (i == 0 || i == 7) board[i][j] = WHITE_R;
				if (i == 1 || i == 6) board[i][j] = WHITE_N;
				if (i == 2 || i == 5)board[i][j] = WHITE_B;
				if (i == 3) board[i][j] = WHITE_Q;
				if (i == 4)	board[i][j] = WHITE_K;
			}
			else if (j == 7){
				if (i == 0 || i == 7) board[i][j] = BLACK_R;
				if (i == 1 || i == 6) board[i][j] = BLACK_N;
				if (i == 2 || i == 5) board[i][j] = BLACK_B;
				if (i == 3)	board[i][j] = BLACK_Q;
				if (i == 4)	board[i][j] = BLACK_K;
			}
			else if (j == 1) board[i][j] = WHITE_P;
			else if (j == 6) board[i][j] = BLACK_P;
			else board[i][j] = EMPTY;
		}
	}
}


// restarts all the variables
int restart(char board[BOARD_SIZE][BOARD_SIZE]){
	turn = 1;
	user_color = WHITE;
	game_mode = 1;
	difficulty = 2;
	is_move_king_white = 0;
	is_move_king_black = 0;
	is_move_right_rook_white = 0;
	is_move_left_rook_white = 0;
	is_move_right_rook_black = 0;
	is_move_left_rook_black = 0;
	initBoard(board);
	return 0;
}


//returns the piece according to str and color
char piece(int color, char *str){
	if (strcmp(str, "king") == 0){
		if (color) return WHITE_K;
		else return BLACK_K;
	}
	if (color){
		switch (str[0]){
		case WHITE_B:
			return WHITE_B;
			break;
		case WHITE_Q:
			return WHITE_Q;
			break;
		case WHITE_N:
			return WHITE_N;
			break;
		case WHITE_R:
			return WHITE_R;
			break;
		case 'p':
			return WHITE_P;
			break;
		}
	}
	else{
		switch (str[0]){
		case WHITE_B:
			return BLACK_B;
			break;
		case WHITE_Q:
			return BLACK_Q;
			break;
		case WHITE_N:
			return BLACK_N;
			break;
		case WHITE_R:
			return BLACK_R;
			break;
		case 'p':
			return BLACK_P;
			break;
		}
	}
	return '\0';
}


//gets a piece p and returns its type
char *whichType(char p){
	switch (p){
	case WHITE_B:
		return "bishop";
		break;
	case WHITE_Q:
		return "queen";
		break;
	case WHITE_N:
		return "knight";
		break;
	case WHITE_R:
		return "rook";
		break;
	case WHITE_K:
		return "king";
		break;
	case WHITE_P:
		return "pawn";
		break;
	case BLACK_B:
		return "bishop";
		break;
	case BLACK_Q:
		return "queen";
		break;
	case BLACK_N:
		return "knight";
		break;
	case BLACK_R:
		return "rook";
		break;
	case BLACK_K:
		return "king";
		break;
	case BLACK_P:
		return "pawn";
		break;
	}
	return "Problem";
}

//gets a piece p and returns its type
char *whichTypeExtended(char p){
	switch (p){
	case WHITE_B:
		return "WhiteBishop";
		break;
	case WHITE_Q:
		return "WhiteQueen";
		break;
	case WHITE_N:
		return "WhiteKnight";
		break;
	case WHITE_R:
		return "WhiteRook";
		break;
	case WHITE_K:
		return "WhiteKing";
		break;
	case WHITE_P:
		return "WhitePawn";
		break;
	case BLACK_B:
		return "BlackBishop";
		break;
	case BLACK_Q:
		return "BlackQueen";
		break;
	case BLACK_N:
		return "BlackKnight";
		break;
	case BLACK_R:
		return "BlackRook";
		break;
	case BLACK_K:
		return "BlackKing";
		break;
	case BLACK_P:
		return "BlackPawn";
		break;
	}
	return "ExtendedProblem";
}

