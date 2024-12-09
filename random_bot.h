#ifndef BOT 
#define BOT
#include "game_logic.h"

Move get_random_move(Board* board, int lower_turn);
Coordinates get_ran_pos(Board* board, int lower_turn);
Move random_straight(Board* board, MoveChoices* choices, Coordinates* start);
Move get_random_horse(Board* board, int x, int y);
Move random_diagonal(MoveChoices* choices);
Move get_random_pawn(Board* board, int x, int y);
#endif
