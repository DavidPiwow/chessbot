#include "file_handler.h"


    

char store_move(FILE* game_log, MoveHistory* move, int num, int turn) {
    if (!game_log || !move) return 1;

    if (num%2 == 0) 
        if (move->captured != EMPTY)
            fprintf(game_log, "%d. %cx%c%c ", turn, toupper(move->piece), 
            'a' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');
        else 
            fprintf(game_log, "%d. %c%c%c ", turn, toupper(move->piece),
                'a' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');
    else
        if (move->captured != EMPTY)
            fprintf(game_log, "%cx%c%c\n", toupper(move->piece), 
            'a' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');
        else 
            fprintf(game_log, "%c%c%c\n", toupper(move->piece),
                'a' + move->where->x2, BOARD_SIZE - (move->where->y2) + '0');

    if (toupper(move->captured) == 'K') return 'K';
    return 0;
}