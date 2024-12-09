#include "display.h"
#include "helper_functions.h"
#include <SDL_image.h>
#include <string.h>
// 45 pixels for the piece sprites!!
#define IMAGE_SIZE 45

#define IMAGELINK "C:\\Users\\david\\source\\repos\\chessupdate\\chess_pieces.png"

App* open_app(int size_x, int size_y) {
	App* app = calloc(1, sizeof(App));
	if (!app) return NULL;
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, size_x, size_y, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	
	if (!surface) printf("%s", SDL_GetError());
	SDL_FillRect(surface, NULL, 
		SDL_MapRGB(surface->format, 0x00, 0x00, 0x00 ));
	//Update the surface
	SDL_UpdateWindowSurface(window);

	app->window = window;
	app->surface = surface;
	app->renderer = renderer;

	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);

	return app;
}

void close_app(App* app) {
	if (app->surface) SDL_FreeSurface(app->surface);
	if(app->renderer) SDL_DestroyRenderer(app->renderer);
	if (app->window) SDL_DestroyWindow(app->window);
	app->surface = NULL;
	app->renderer = NULL;
	app->window = NULL;
	free(app);
	app = NULL;
}

void draw_board(Board* board, App* app, SDL_Texture* image) {
	int x, y;

	int c = 0;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (c % 2 == 0) {

				SDL_SetRenderDrawColor(app->renderer, 88, 77, 100, 255);
			}
			else {

				SDL_SetRenderDrawColor(app->renderer, 48, 34, 62, 255);
			}
			x = j * (DISP_SQUARE_SIZE)+MARGIN;
			y = i * (DISP_SQUARE_SIZE)+MARGIN;

			char piece = get_piece_at(board, j, i);
			SDL_Rect square = { x, y, DISP_SQUARE_SIZE, DISP_SQUARE_SIZE };
			SDL_RenderFillRect(app->renderer, &square);

			if (piece != EMPTY) {
				int piece_number = get_piece_number(piece);
				square.x += (DISP_SQUARE_SIZE - IMAGE_SIZE)/2;
				square.y += (DISP_SQUARE_SIZE - IMAGE_SIZE) / 2;
				square.w = IMAGE_SIZE;
				square.h = IMAGE_SIZE;
				load_sprite(app, image, &square, piece_number);
			}
			c++;
		}
		c++;
	}

    SDL_SetRenderDrawColor(app->renderer, 0,0,0, 255);
}

SDL_Texture* load_image(App* app) {
	SDL_Texture* to_return = NULL;

	SDL_Surface* image_surface = IMG_Load(IMAGELINK);

	to_return = SDL_CreateTextureFromSurface(app->renderer, image_surface);

	SDL_FreeSurface(image_surface);
	return to_return;
}

void load_sprite(App* app, SDL_Texture* texture, SDL_Rect* where, int piece_number) {
	int x, y;

	y = piece_number > 0 ? IMAGE_SIZE : 0;

	if (abs(piece_number) == 6) piece_number = 0;
	x = abs(piece_number) * IMAGE_SIZE;

	SDL_Rect sprite_clip = (SDL_Rect){ x , y, IMAGE_SIZE, IMAGE_SIZE };
	
	SDL_RenderCopy(app->renderer, texture, &sprite_clip, where);
}


