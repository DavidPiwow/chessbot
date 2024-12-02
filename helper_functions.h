#ifndef helper_functions
#define helper_functions


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "bitboard.h"

#include "game_logic.h"


int correct_direction(Move* move, char piece);


void move_piece(Board* board, Move* move);
char get_piece_at(Board* board, int x, int y);

int piece_score(char piece);
int num_from_char(char c); 

int can_move_vertical(char piece);
int can_move_horizontal(char piece);
int can_move_diagonal(char piece);
int out_of_bounds(Move* move);

int is_same_team(char piece, char piece2);

//MoveChoices get_coords(Board* board);
MoveHistory* push_history(Move* move, MoveHistory* last);
MoveHistory* pop_history(MoveHistory* last);

int get_piece_number(char piece);
void free_history(MoveHistory* history);


#endif
