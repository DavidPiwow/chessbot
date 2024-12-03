#include "game_logic.h"

FILE* get_history_(int id) {

}

FILE* store_game(MoveHistory* move_history) {
    FILE* to_return = fopen("chess.history","w");
    
    char move[6]; 

    
    
    return to_return;
}

int store_move(FILE* game_log, MoveHistory* move) {
    if (!game_log || !move) return 1;

    char move_string[10]; 

    return 0;
}