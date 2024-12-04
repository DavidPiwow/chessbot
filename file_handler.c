#include "file_handler.h"


    

int store_move(FILE* game_log, MoveHistory* move) {
    if (!game_log || !move) return 1;
    fprintf(game_log, "%c%c%c%c%c%c\n", toupper(move->piece), 
    move->captured == EMPTY ? ' ' : 'x', 
    'A' + move->where->x1, BOARD_SIZE - (move->where->y1) + '0',
    'A' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');
    return 0;
}