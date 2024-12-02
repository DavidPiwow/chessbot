#ifndef PRINTFUNC
#define PRINTFUNC
#include "game_logic.h"
#include "helper_functions.h"

Move get_move(void);
void print_board(Board* board);
void print_move(Board* board, Move* move);

void print_coords(Coordinates* c);

#endif
