#include "game_logic.h"
#include "helper_functions.h"
#include "bitboard.h"

static inline int bad_coords(int x, int y) {
    return (x < 0) || (y < 0) ||
        (x >= BOARD_SIZE) || (y >= BOARD_SIZE) ||
        (x == EMPTY || y == EMPTY);
}

char get_piece_at(Board* board, int x, int y) {
    if (bad_coords(x, y)) return EMPTY;
    return *(board->grid + x + (y * BOARD_SIZE));
}

int get_piece_number(char piece) {
    int to_return = EMPTY;
    switch (toupper(piece)) {
    case 'K':
        to_return = 6;
        break;
    case 'Q':
        to_return = 1;
        break;
    case 'B':
        to_return = 2;
        break;
    case 'N':
        to_return = 3;
        break;
    case 'R':
        to_return = 4;
        break;
    case 'P':
        to_return = 5;
        break;

    default:
        break;
    }
    if (islower(piece)) to_return *= -1;
    return to_return;
    
}

int can_move_vertical(char piece) {
    return (toupper(piece) == 'R' || toupper(piece) == 'P' || toupper(piece) == 'K' || toupper(piece) == 'Q');
}

int can_move_horizontal(char piece) {
    return (toupper(piece) == 'R' || toupper(piece) == 'K' || toupper(piece) == 'Q');
}

int can_move_diagonal(char piece) {
    return (toupper(piece) == 'B' || toupper(piece) == 'K' || toupper(piece) == 'Q');
}



int correct_direction(Move* move, char piece) {
    if (!move || (move->dx == 0 && move->dy == 0)) return 0;

    // make sure the piece is allowed to move in the way the move says
    if ((move->dx == 0 && move->dy != 0) && can_move_vertical(piece)) {
        return 1;
    }
    else if ((move->dx != 0 && move->dy == 0) && can_move_horizontal(piece)) {
        return 1;
    }
    else if ((move->dx == move->dy) && can_move_diagonal(piece)) {
        return 1;
    }

    return 0;
}

// u better be on the board >:(( and REALLLLL
int out_of_bounds(Move* move) {
        return bad_coords(move->x1, move->y1) ||(bad_coords(move->x2, move->y2)) 
            || (move->dx == 0 && move->dy == 0);
}


int is_same_team(char piece, char piece2) {
    if (isupper(piece))
        return isupper(piece2);
    
    if (islower(piece))
        return islower(piece2);

    return 0;
}

int piece_score(char piece) {
    switch (piece)
    {
    case 'P':
        return 10;
        break;
    case 'N':
        return 11;
        break;
    case 'B':
    case 'R':
        return 12;
        break;
    case 'Q':
        return 13;
        break;
    case 'K':
        return 100;
        break;
    default:
        break;
    }
    return 0;
}

Coordinates* find_piece(Coordinates* coords, int x, int y) {
    for (int i = 0; i <= PIECE_COUNT; i++) {
        if (!coords) break;
        if (coords->x == x && coords->y == y) return coords;
        coords++;
    }
    return NULL;
}

MoveHistory* move_piece(Board* board, Move* move) {
    char* old_pos = board->grid + move->x1 + (move->y1*BOARD_SIZE);
    char* new_pos = board->grid + move->x2 + (move->y2*BOARD_SIZE);

    char piece = get_piece_at(board, move->x1, move->y1);
    Coordinates* piece_cache = isupper(piece) ? board->upper_pieces : board->lower_pieces;
    Coordinates* enemy_cache = isupper(piece) ? board->lower_pieces : board->upper_pieces;

    piece_cache = find_piece(piece_cache, move->x1, move->y1);
    char capture = get_piece_at(board, move->x2, move->y2);
    enemy_cache = find_piece(piece_cache, move->x2, move->y2);
    if (enemy_cache) enemy_cache->x = enemy_cache->y = EMPTY;
    piece_cache->x = move->x2;
    piece_cache->y = move->y2;
    *new_pos = piece;
    *old_pos = EMPTY;

    MoveHistory* hist = push_history(move, piece, capture);
    piece_swap(board->state, move, islower(piece));
    if (board->history != NULL) {
        hist->last = board->history; 
    } 
    
    board->history = hist;

    return hist;
}

MoveHistory* push_history(Move* move, char piece, char captured) {
    MoveHistory* mv = (MoveHistory*)malloc(sizeof(MoveHistory));
    if (!mv) {
        return NULL;
    }
    mv->piece = piece;
    mv->captured = captured;
    mv->where = move;
    mv->last = NULL;
    
    return mv;
}

MoveHistory* pop_history(MoveHistory* last) {
    MoveHistory* to_return = last->last;
    free(last);

    return to_return;
}

void free_history(MoveHistory* history) {
    MoveHistory* cur = history;
    MoveHistory* prev = NULL;
    while (cur != NULL) {
        prev = cur;
        cur = cur->last;
        free(prev);
    }
    cur = NULL;
    prev = NULL;
}

int num_from_char(char c) {
    if (c >= 'A' && c <= 'H') {
        return c- 'A';
    }
    if (c >= '0' && c <= '8') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'h') {
        return c- 'a';
    }
    return EMPTY;
}

Coordinates* get_coordinates(Board* board, int up) {
    Coordinates *positions = malloc(sizeof(Coordinates) * 17);
    if (!positions) {
        printf("FAIL");
        return NULL;
    }

    int pos_count = 0;
    char piece;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (pos_count >= 16) 
                break;

            piece = get_piece_at(board, x, y);

            if (piece == EMPTY) 
                continue;

            if ((up && !isupper(piece)) || (!up && !islower(piece)))
                continue;

            positions[pos_count] = (Coordinates){x, y};
            pos_count++;
        }
    }

    return positions;
}