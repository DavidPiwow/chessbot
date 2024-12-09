#include "game_logic.h"
#include <SDL.h>

#define IMAGE_PATH "C:\NEWCHESS\chess_pieces.png"

#define SCREEN_SIZE 640
#define SQUARE_SIZE SCREEN_SIZE/BOARD_SIZE

#define MARGIN SQUARE_SIZE / 2
#define DISP_SQUARE_SIZE (SCREEN_SIZE - MARGIN * 2) / BOARD_SIZE

struct _app {
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Window* window;
};

struct _user_move_input {
	int x1, x2, y1, y2;
};

typedef struct _user_move_input UserMove;

typedef struct _app App;

App* open_app(int size_x, int size_y);
void close_app(App* app);
void draw_board(Board* board, App* app, SDL_Texture* image);
SDL_Texture* load_image(App* app);
void load_sprite(App* app, SDL_Texture* texture, SDL_Rect* where, int piece_number);
