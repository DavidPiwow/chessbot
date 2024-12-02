#ifndef BITBOARDH
#define BITBOARDH

#include "game_logic.h"
#include <stdlib.h>

typedef uint64_t BITBOARD;


struct _board_state {
    BITBOARD white;
    BITBOARD black;
};


void print_num(BITBOARD* board);

typedef struct _board_state BoardState;

int piece_swap(BoardState* game_state, struct _move* move, int white_team);
int attempt_move(BoardState* game_state, struct _move* move, int white_team);

#endif
