#include "move_tree.h"
#include "helper_functions.h"

#define PIECE_COUNT BOARD_SIZE * 2
#define DIRECTIONS 8"

MoveNode* generate_tree(Board* board, int up) {
    Coordinates* pieces = find_pieces(board, up);
    Coordinates* valid_pieces = malloc(sizeof(Coordinates) * (PIECE_COUNT + 1));
    MoveChoices* valid_moves  = malloc(sizeof(MoveChoices) * (PIECE_COUNT + 1));
    if (!valid_pieces || !pieces) return NULL;

    int pos = 0;
    while (pieces[pos].x != EMPTY || pos <= 16) {
        int x = pieces[pos].x;
        int y = pieces[pos].y;
        MoveChoices choices = get_valid_moves(board, x, y);
        if (choices.any) {
            valid_pieces[pos] = (Coordinates){x, y};
            valid_moves[pos] = choices;
        }
        pos++;
    }


}




// Gather coordinates of all pieces on a team
Coordinates* find_pieces(Board* board, int up) {
    // Allocate spots for every piece + an extra buffer ending piece
    Coordinates* coords = (Coordinates*)malloc(sizeof(Coordinates) * (PIECE_COUNT + 1));
    if (!coords) {
        printf("FAIL");
        return NULL;
    }

    int pos_count = 0;
    // loop through every space
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            // if all the pieces were found break
            if (pos_count >= PIECE_COUNT)
                break;

            char piece = get_piece_at(board, x, y);

            if (piece == EMPTY || !(is_correct_team(piece, up)))
                continue;
            else {
                coords[pos_count] = (Coordinates){ x, y };
                pos_count++;
            }
        }
    }

    pos_count++;
    coords[pos_count - 1] = (Coordinates){ EMPTY, EMPTY };

    if (pos_count > 0) {
        Coordinates* new_ret = (Coordinates*)realloc(coords, sizeof(Coordinates) * pos_count);
        return new_ret;
    }

    free(coords);
    return NULL;
}