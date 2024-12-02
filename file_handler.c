#include "game_logic.h"

FILE* get_history_(int id) {

}

FILE* store_game(MoveHistory* move_history, int move_count) {
    FILE* to_return = fopen("chess.history","w");
    
    for (int i = 0; i < move_count; i ++) {
        if (!move_history) break; 
        move_history.
    }

    return to_return;
}