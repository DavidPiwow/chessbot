#include "game_logic.h"
#include "helper_functions.h"

static inline int get_distance_from_center(Coordinates position) {
    return abs(position.x - BOARD_SIZE/2) + abs(position.y - BOARD_SIZE/2);
}

int evaluate_move(Board* board, Move* move) {
    char piece = get_piece_at(board, move->x1, move->y1);
    char goal = get_piece_at(board, move->x2, move->y2);
    if (is_same_team(piece, goal)) return 0;

    int score = piece_score(goal);
    score -= get_distance_from_center((Coordinates){
        move->x2, move->y2
    });
    score += move->dx ? move->dx : move->dy;


    return score;

}