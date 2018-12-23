#include "GameWindow.h"
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../Chess.h"
#include "../Game.h"
#include "../minimax.h"


int history = 0;


//destroys all game window widgets
void destroyGameWindowWidgets(Widget** widgets, char board[BOARD_SIZE][BOARD_SIZE]){
	int i;
	for (i=0; i<GAME_WIDGETS_NUMBER+countPlayerPieces(2, board); i++){
		if ( widgets[i] != NULL){
			widgets[i]->destroyWidget(widgets[i]);
		}
	}
	free(widgets);
}


//gets a board and a location and returns the image path for the piece in that location
char* getPieceImagePath(char board[BOARD_SIZE][BOARD_SIZE], int i, int j){
	char *path = calloc(PIECES_PATH_LENGTH,sizeof(char));
	char *name = calloc(PIECES_NAME_MAX_LENGTH, sizeof(char));
	char *postfix = calloc(PIECES_POSTFIX_LENGTH, sizeof(char));
	strcpy(path,PIECES_PATH);
	strcpy(name,whichTypeExtended(board[i][j]));
	strcpy(postfix,PIECES_POSTFIX);
	strcat(path,name);
	strcat(path,postfix);
	return path;
}


//creates buttons to be used in the game window
Widget** createGameWindowWidgets(SDL_Renderer* renderer, char board[BOARD_SIZE][BOARD_SIZE]){
	int numOfPieces = 0;
	if (renderer == NULL) return NULL ; //argument isn't a valid renderer

	Widget** widgets = calloc(GAME_WIDGETS_NUMBER+countPlayerPieces(2,board), sizeof(Widget*));
	if (widgets == NULL) return NULL ; //memory allocation error

	SDL_Rect restartRec = {.x =10,.y=20,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect saveRec = {.x =10,.y=130,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect loadRec = {.x=10,.y=240,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect undoRec = {.x=10,.y=350,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect mainmenuRec = {.x=10,.y=460,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect exitRec = {.x=10,.y=570,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	widgets[0] = createButton("./guiBMPs/Game/restart.bmp", NULL, renderer, &restartRec);
	widgets[1] = createButton("./guiBMPs/Game/save.bmp", NULL, renderer, &saveRec);
	widgets[2] = createButton("./guiBMPs/Game/load.bmp", NULL, renderer, &loadRec);
	widgets[3] = createButton("./guiBMPs/Game/undo.bmp", "./guiBMPs/Game/undop.bmp", renderer, &undoRec);
	widgets[4] = createButton("./guiBMPs/Game/mainmenu.bmp", NULL, renderer, &mainmenuRec);
	widgets[5] = createButton("./guiBMPs/Game/exit.bmp", NULL, renderer, &exitRec);
	for (int i=0; i < BOARD_SIZE; i++){
		for (int j=0; j<BOARD_SIZE; j++ ){
			if (board[i][j] != EMPTY){
				SDL_Rect pieceRec = {270+(BOARD_SIZE-i)*90, (BOARD_SIZE-j-1)*90, .h=90, .w=90 };
				char* path = getPieceImagePath(board, i, j);
				widgets[numOfPieces+GAME_WIDGETS_NUMBER] = createPiece(path, renderer, &pieceRec);
				numOfPieces++;
			}
		}
	}
	for (int i=0; i<GAME_WIDGETS_NUMBER+numOfPieces; i++){
		if (widgets[i] == NULL){
			printf("error creating game window widget number %d",i);
			destroyGameWindowWidgets(widgets, board);
			break;
		}
	}
	return widgets;
}


//destroys all the data that is associated with the given game window
void destroyGameWindow(GameWindow* window, char board[BOARD_SIZE][BOARD_SIZE]){
	if (window == NULL) return; //argument isn't a valid game eindow struct

	destroyGameWindowWidgets(window->widgets, board);
	if (window->bgTexture != NULL) SDL_DestroyTexture(window->bgTexture);
	if (window->boardTexture != NULL) SDL_DestroyTexture(window->boardTexture);
	if (window->renderer != NULL) SDL_DestroyRenderer(window->renderer);
	if (window->window != NULL)	SDL_DestroyWindow(window->window);
	free(window);
}


//creates a game window
GameWindow* createGameWindow(char board[BOARD_SIZE][BOARD_SIZE]){
	GameWindow* retWin = (GameWindow*) calloc(sizeof(GameWindow), sizeof(char));
	SDL_Surface* loadingSurface1 = NULL; //temporary
	SDL_Surface* loadingSurface2 = NULL; //temporary
	if (retWin == NULL){ //memory allocation error
		printf("Error creating GameWindow struct\n");
		return NULL ;
	}

	// Creates the window itself
	retWin->window = SDL_CreateWindow(/*title-*/ "Game",
					/*x pos-*/ SDL_WINDOWPOS_CENTERED, /*y pos-*/ SDL_WINDOWPOS_CENTERED,
					/*width-*/ 1024, /*height-*/ 720, /*flags-*/ SDL_WINDOW_OPENGL);

	if (retWin->window == NULL){ //error creating window
		printf("Error creating window: %s\n", SDL_GetError());
		destroyGameWindow(retWin, board);
		return NULL ;
	}
	retWin->renderer = SDL_CreateRenderer(retWin->window, -1,SDL_RENDERER_ACCELERATED);
	if (retWin->renderer == NULL){ //error creating renderer
		printf("Error creating renderer: %s\n", SDL_GetError());
		destroyGameWindow(retWin, board);
		return NULL ;
	}

	//creates board texture
	loadingSurface1 = SDL_LoadBMP("./guiBMPs/Board/board.bmp");
	if (loadingSurface1 == NULL){
		printf("Error creating surface: %s\n", SDL_GetError());
		destroyGameWindow(retWin, board);
		return NULL ;
	}
	retWin->boardTexture = SDL_CreateTextureFromSurface(retWin->renderer,loadingSurface1);
	if (retWin->boardTexture == NULL){ //error creating texture from surface
		printf("Error creating texture: %s\n", SDL_GetError());
		destroyGameWindow(retWin, board);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface2); //we are done with the temporary surface

	//creates background texture
	loadingSurface2 = SDL_LoadBMP("./guiBMPs/Game/game_back.bmp");
	if (loadingSurface2 == NULL){ //error loading image
		printf("Error creating surface: %s\n", SDL_GetError());
		destroyGameWindow(retWin, board);
		return NULL ;
	}
	retWin->bgTexture = SDL_CreateTextureFromSurface(retWin->renderer, loadingSurface2);
	if (retWin->bgTexture == NULL){ //error creating texture from surface
		printf("Error creating texture: %s\n", SDL_GetError());
		destroyGameWindow(retWin, board);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface2); //we are done with the temporary surface
	retWin->widgets = createGameWindowWidgets(retWin->renderer, board);
	if (retWin->widgets == NULL){ //error creating widgets
		destroyGameWindow(retWin, board);
		printf("Error creating widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	retWin->numOfWidgets = GAME_WIDGETS_NUMBER+countPlayerPieces(2,board);
	retWin->isFirst = true;
	history = 0;
	return retWin;
}


//draws the given game window
void drawGameWindow(GameWindow* window, bool update, char board[BOARD_SIZE][BOARD_SIZE]){
	if (window == NULL) return; //argument isn't a valid game window struct

	SDL_Rect boardRec ={.x=270,.y=0,.w=720,.h=720 };
	SDL_Rect bgRec ={.x=0,.y=0,.w=1024,.h=720 };
	SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
	SDL_RenderClear(window->renderer);
	SDL_RenderCopy(window->renderer, window->bgTexture, NULL, &bgRec);
	SDL_RenderCopy(window->renderer, window->boardTexture, NULL, &boardRec);
	if (update == true){
		((Button*) window->widgets[3]->data)->isActive=false;
		if (history > 0){
			if (game_mode == 1){
				((Button*) window->widgets[3]->data)->isActive = true;
			}
		}
		int k, j, i = 0;
		for (k=GAME_WIDGETS_NUMBER; k<window->numOfWidgets; k++){
			((Piece*) window->widgets[k]->data)->isShown = false;
		}
		int numOfPieces = 0;
		for (i=0;i < BOARD_SIZE;i++){
			for (j=0 ; j<BOARD_SIZE; j++ ){
				if (board[i][j] != EMPTY){
					SDL_Rect pieceRec ={270+i*90, (BOARD_SIZE-j-1)*90, .h=90, .w=90 };
					char* path = getPieceImagePath(board, i, j);
					window->widgets[numOfPieces+GAME_WIDGETS_NUMBER] = createPiece(path, window->renderer, &pieceRec);
					numOfPieces++;
					free(path);
				}
			}
		}
	}
	for (int i=0; i < window->numOfWidgets; i++){
		(window->widgets[i])->drawWidget(window->widgets[i]);
	}
	SDL_RenderPresent(window->renderer);
}


//checks if there's a winner or a tie game
GAME_MESSAGE checkWinnerOrTie(char board[BOARD_SIZE][BOARD_SIZE]){
	Move *moves = getMoves(board, turn);
	if (moves == NULL) exit(0);
	if (moves->type == 't') return GAME_TIE; //tie
	else if (moves->type == 'l') return GAME_MATE; //mate
	int isCheckBool = isCheck(board, turn);
	if (isCheckBool == 0){ //not check
		freeMoves(moves);
		exit(0);
	}
	if (isCheckBool == 1){ //check
		freeMoves(moves);
		return GAME_CHECK;
	}
	else{ //no winner or tie
		freeMoves(moves);
		return GAME_CONT;
	}
}


//handles game window events given a window and an event
GAME_EVENT handleGameWindowEvent(GameWindow* window, SDL_Event* event, char board[BOARD_SIZE][BOARD_SIZE]){
	if (event == NULL || window == NULL) return GAME_EVENT_INVALID_ARGUMENT; //invalid arguments

	switch (event->type){
	case SDL_MOUSEBUTTONUP:
		if (window->widgets[0]->handleWidgetEvent(window->widgets[0],event) == 1){
			turn = 1; //so that white player always starts
			window->isFirst = true;
			return GAME_EVENT_RESTART;
		}
		else if (window->widgets[1]->handleWidgetEvent(window->widgets[1],event) == 1){
			return GAME_EVENT_SAVE;
		}
		else if (window->widgets[2]->handleWidgetEvent(window->widgets[2],event) == 1){
			return GAME_EVENT_LOAD;
		}
		else if (window->widgets[3]->handleWidgetEvent(window->widgets[3],event) == 1){
			if (game_mode == 2){
				printf("Undo command not available in 2 players mode\n");
			}
			else if (game_mode == 1){
				if (history > 0){
					undoMove(board);
					history--;
				}
				else{
					break;
				}
			}
			return GAME_EVENT_UPDATE;
		}
		else if (window->widgets[4]->handleWidgetEvent(window->widgets[4],event) == 1){
			turn = 1;	//so that white always starts
			return GAME_EVENT_MAIN_MENU;
		}
		else if (window->widgets[5]->handleWidgetEvent(window->widgets[5],event) == 1){
			return GAME_EVENT_EXIT;
		}
		break;

	case SDL_MOUSEBUTTONDOWN:{
		int legal = -1;
		int x,y,i,j;
		for (i=0; i<countPlayerPieces(2, board); i++){
			int numwidget = i+GAME_WIDGETS_NUMBER;
			if (window->widgets[numwidget]->handleWidgetEvent(window->widgets[numwidget],event) == 1){
				x = (event->button.x-270)/90;
				y = BOARD_SIZE-(event->button.y/90)-1;
				if (board[x][y] != EMPTY){
					if (blackOrWhite(board[x][y]) == turn){
						legal = numwidget;
						break;
					}
				}
			}
		}
		if (legal != -1){
			SDL_Event eventmotion;
			SDL_WaitEvent(&eventmotion);
			while(eventmotion.type == SDL_MOUSEMOTION){
				((Piece*)window->widgets[legal]->data)->location->x = eventmotion.motion.x-45;
				((Piece*)window->widgets[legal]->data)->location->y = eventmotion.motion.y-45;
				drawGameWindow(window, false, board);
				SDL_WaitEvent(&eventmotion);
				if (eventmotion.type != SDL_MOUSEMOTION){
					break;
				}
			}
			i = (eventmotion.button.x-270)/90;
			j = BOARD_SIZE - (eventmotion.button.y/90) - 1;
			int validmove = move(board, turn, x, y, i, j, '\0');
			if (validmove == 1){
				turn = 1-turn; //change turn
				drawGameWindow(window, true, board);
				gameSaved = 0;
				GAME_MESSAGE gameover = checkWinnerOrTie(board); //represents the game state
				if(game_mode == 2){ //two players
					if (gameover == GAME_CHECK){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","Check!", window->window );
					}
					if(gameover == GAME_MATE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","YOU HAVE WON!", NULL );
						return GAME_EVENT_MAIN_MENU;
					}
					if(gameover == GAME_TIE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","IT'S A TIE!", NULL );
						return GAME_EVENT_MAIN_MENU;
					}
				}
				else if (game_mode == 1){ //one player
					if (history < 3){
						history++;
					}
					if (gameover == GAME_MATE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog", "YOU HAVE WON!", NULL );
						return GAME_EVENT_MAIN_MENU;
					}
					if (gameover == GAME_TIE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog", "IT'S A TIE!", NULL );
						return GAME_EVENT_MAIN_MENU;
					}
					Move *moves = getMoves(board, turn);
					moves = minimaxManager(board, moves, turn, difficulty, '\0');
					if (moves->type == 'c'){
						makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
						updateCastling(board, moves->pos_start->y, moves->pos_start->x);
						makeMove(board, 4, 7 * (1-turn), 4  + (2 * (left_right(1-turn) - 2)), 7 * (1-turn));
						updateCastling(board, 7 * (1-turn), 4);
					}
					else{
						char *piecetypeb4 = whichType(board[moves->pos_start->x][moves->pos_start->y]);
						saveMove((moves->pos_start->y),(moves->pos_start->x), (moves->pos_end->y), (moves->pos_end->x), board);
						makeMove(board, moves->pos_start->x, moves->pos_start->y, moves->pos_end->x, moves->pos_end->y);
						updateCastling(board, moves->pos_start->x, moves->pos_start->y);
						char *piecetypeafter = whichType(board[moves->pos_end->x][moves->pos_end->y]);
						if (strcmp(piecetypeb4, piecetypeafter)){
							char* message = piecetypeafter;
							char premessage[39];
							strcpy(premessage, "Computer promoted the pawn to ");
							strcat(premessage,message);
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Computer Promoted", premessage, NULL);
						}
					}
					turn = 1-turn; //change turn
					freeMoves(moves);
					drawGameWindow(window, true, board);
					gameover = checkWinnerOrTie(board);
					if(gameover == GAME_CHECK){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chess","Check!", NULL );
					}
					if(gameover == GAME_MATE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chess","YOU HAVE LOST!", NULL );
						return GAME_EVENT_MAIN_MENU;
					}
					if(gameover == GAME_TIE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chess","IT'S A TIE!", NULL );
						return GAME_EVENT_MAIN_MENU;
					}
				}
			}
		}
		return GAME_EVENT_UPDATE;
		break;
	}
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE){
			return GAME_EVENT_EXIT;
		}
		break;
	default:
		return GAME_EVENT_NONE;
	}
	return GAME_EVENT_NONE;
}
