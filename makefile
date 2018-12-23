
CC = gcc
OBJS = main.o Game.o GameState.o Files.o Moves.o minimax.o Buttons.o GameWindow.o GUIManager.o LoadWindow.o MainMenuWindow.o SettingsWindow.o
EXEC = chessprog
CC_COMP_FLAG = -ggdb -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG =  -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT -I./guiFiles 
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main
COMP_FLAG = -ggdb -std=c99 -Wall -Wextra -g \
-Werror -pedantic-errors
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -o $@
main.o: main.c Moves.h minimax.h Chess.h Game.h guiFiles/GUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c $*.c
Game.o: Game.c Game.h Moves.h minimax.h Chess.h Files.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c $*.c
GameState.o: GameState.c Moves.h minimax.h Game.h Chess.h Files.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c $*.c
Files.o: Files.c Files.h Moves.h Game.h minimax.h Chess.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c $*.c
Moves.o: Moves.c Moves.h Chess.h minimax.h Game.h Files.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c $*.c
minimax.o: minimax.c minimax.h Chess.h Game.h Moves.h Files.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c $*.c
Buttons.o: guiFiles/Buttons.c guiFiles/Buttons.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c ./guiFiles/Buttons.c
GameWindow.o: guiFiles/GameWindow.c guiFiles/GameWindow.h Chess.h Game.h minimax.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c ./guiFiles/GameWindow.c 
GUIManager.o: guiFiles/GUIManager.c guiFiles/GUIManager.h Chess.h Game.h minimax.h Files.h guiFiles/Buttons.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c ./guiFiles/GUIManager.c
LoadWindow.o: guiFiles/LoadWindow.c guiFiles/LoadWindow.h Chess.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c ./guiFiles/LoadWindow.c
MainMenuWindow.o: guiFiles/MainMenuWindow.c guiFiles/MainMenuWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c ./guiFiles/MainMenuWindow.c
SettingsWindow.o: guiFiles/SettingsWindow.c guiFiles/SettingsWindow.h Chess.h Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) $(SDL_LIB) -c ./guiFiles/SettingsWindow.c
clean:
	rm -f *.o $(EXEC)