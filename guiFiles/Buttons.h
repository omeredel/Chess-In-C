#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>

#define MAIN_MENU_WIDGETS_NUMBER 3
#define LOAD_WIDGETS_NUMBER 7
#define SETTINGS_WIDGETS_NUMBER 11
#define GAME_WIDGETS_NUMBER 6

#define BUTTON_WIDTH 250
#define BUTTON_HEIGTH 100
#define BUTTON_SHIFT 500


typedef struct widget Widget;
struct widget {
	void (*destroyWidget)(Widget*);
	void (*drawWidget)(Widget*);
	int (*handleWidgetEvent)(Widget*, SDL_Event*);
	void* data;
};


typedef struct regularButton{
	SDL_Texture* buttonTexture;
	SDL_Texture* buttonPressedTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool isActive;
	bool isShown;
}Button;

void destroyButton(Widget* b);

SDL_Rect* copyRec(SDL_Rect* srcRec);

Widget* createButton(const char* buttonImage,const char* pressedButtonImage, SDL_Renderer* renderer, SDL_Rect* location);

void drawButton(Widget* b);

int handleButtonEvent(Widget* b, SDL_Event* event);


typedef struct pieceButton{
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool isShown;
}Piece;

void destroyPiece(Widget* p);

Widget* createPiece(const char* image, SDL_Renderer* renderer, SDL_Rect* location);

void drawPiece(Widget* p);

int handlePieceEvent(Widget* p, SDL_Event* event);


#endif
