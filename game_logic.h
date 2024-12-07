#ifndef chess
#define chess


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define EMPTY ' '
#define BOARD_SIZE 8 
#define SQUARE_COUNT BOARD_SIZE * BOARD_SIZE
#define PIECE_COUNT 16

typedef uint64_t BITBOARD;

struct _move {
	int x1,x2,y1,y2,dx,dy;
};

struct _coordinates {
    int x, y;
};

struct _move_history {
    struct _move* where;
    char captured, piece;
    struct _move_history* last;
};

struct _move_choices {
    int x,y,d_u, d_d, d_l, d_r, d_pu,d_pd, d_nu, d_nd, any;
};


struct _board {
    char grid[BOARD_SIZE * BOARD_SIZE];
    char* board_end;
    struct _board_state* state;
    struct _move_history* history;

    struct _coordinates* lower_pieces;
    struct _coordinates* upper_pieces;
};


typedef struct _move Move;
typedef struct _coordinates Coordinates;
typedef struct _board Board;
typedef struct _move_history MoveHistory;
typedef struct _move_choices MoveChoices;

Board* create_board(void);

void free_board(Board* board);
int count_in_direction(Board *board, Move *move, int dx, int dy);

int verify_move(Board* board, Move* move);
MoveChoices get_valid_moves(Board *board, int x1, int y1);
Coordinates get_horse_move(Board* board, int x1, int y1);

#endif
