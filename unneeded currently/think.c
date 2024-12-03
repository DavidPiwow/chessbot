#include "helper_functions.h"

#define PIECE_COUNT BOARD_SIZE * 2
#define DIRECTIONS 8

static inline int can_capture_piece(Board* board, int x, int y, int dx, int dy) {
    char piece = get_piece_at(board, x, y);
    int goal = get_piece_at(board, x + dx, y + dy);

    if (goal != EMPTY) {
        return !is_same_team(piece, goal);
    }

    return 0;
}

static inline int is_correct_team(char piece, int up) {
    if (up)
        return isupper(piece);
   
    return islower(piece);
}

// Gather coordinates of all pieces on a team
Coordinates* find_pieces(Board* board, int up) {
    // Allocate spots for every piece + an extra buffer ending piece
    Coordinates* coords = (Coordinates*)malloc(sizeof(Coordinates) * (PIECE_COUNT + 1) );
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
    coords[pos_count - 1] = (Coordinates){EMPTY, EMPTY};

    if (pos_count > 0) {
        Coordinates* new_ret = (Coordinates*)realloc(coords, sizeof(Coordinates) * pos_count);  
        return new_ret; 
    }

    free(coords);
    return NULL;
}
 
static Coordinates* find_longest(Coordinates* pieces, MoveChoices* choices) {
    int  index = 0;
    int longest_move = 0;
    int longest_index = 0;

    while (pieces[index].x != EMPTY) {
        if (choices[index].any > 0) {
            if (choices[index].any > longest_move) {
                longest_move = choices[index].any;
                longest_index = index;
            }
        }
        index++;
    }

    return &pieces[longest_index];
}

Coordinates get_most_moves(Board* board, int up) {
    MoveChoices* choices = malloc(sizeof(MoveChoices) * (PIECE_COUNT + 1));
    if (!choices)
        return (Coordinates) { EMPTY, EMPTY };

    Coordinates* pieces = find_pieces(board, up);
    int index = 0;

    while (pieces[index].x != EMPTY) {
        choices[index] = get_valid_moves(board, pieces[index].x, pieces[index].y);
        index++;
    }

    Coordinates* most_moves = find_longest(pieces, choices);

    int x = most_moves->x;
    int y = most_moves->y;
    
    free(choices);
    free(pieces);
    
    return (Coordinates){x,y};
}

int rate_move(Board* board, Move* move) {
    int move_distance = move->dx ? move->dx : move->dy;
    if (!move_distance) return 0;

    int rating = 0;
    //char start_piece = get_piece_at(board, move->x1, move->y1);
    char end_piece = get_piece_at(board, move->x2, move->y2);
    
    rating += piece_score(end_piece);


    rating += move_distance;

    return rating;
}
