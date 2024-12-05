#include "file_handler.h"


    

int store_move(FILE* game_log, MoveHistory* move, int num, int turn) {
    if (!game_log || !move) return 1;

    if (num%2 == 0) 
    fprintf(game_log, "%d. %c%c%c%c ", turn, toupper(move->piece), 
    move->captured == EMPTY ? EMPTY : 'x', 
    'a' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');
    else
    fprintf(game_log, "%c%c%c%c ", toupper(move->piece), 
    move->captured == EMPTY ? EMPTY : 'x', 
    'a' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');
    return 0;
}