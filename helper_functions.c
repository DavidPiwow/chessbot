#include "game_logic.h"
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
    case 'C':
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
    return (toupper(piece) == 'C' || toupper(piece) == 'P' || toupper(piece) == 'K' || toupper(piece) == 'Q');
}

int can_move_horizontal(char piece) {
    return (toupper(piece) == 'C' || toupper(piece) == 'K' || toupper(piece) == 'Q');
}

int can_move_diagonal(char piece) {
    return (toupper(piece) == 'B' || toupper(piece) == 'K' || toupper(piece) == 'Q');
}


MoveHistory* push_history(Move* move, MoveHistory* last, char piece, char captured);
MoveHistory* pop_history(MoveHistory* last);


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
        return 1;
        break;
    case 'N':
        return 2;
        break;
    case 'B':
    case 'C':
        return 3;
        break;
    case 'Q':
        return 4;
        break;
    default:
        break;
    }
    return 0;
}


void move_piece(Board* board, Move* move) {
    char* old_pos = board->grid + move->x1 + (move->y1*BOARD_SIZE);
    char* new_pos = board->grid + move->x2 + (move->y2*BOARD_SIZE);
    char piece = get_piece_at(board, move->x1, move->y1);
    char capture = get_piece_at(board, move->x2, move->y2);
    *new_pos = piece;
    *old_pos = EMPTY;
   // MoveHistory* hist = push_history(move, board->history, piece, capture);
    piece_swap(board->state, move, islower(piece));
   /* if (board->history) {
        hist->last = board->history; 
    } else {
        board->start_of_history = hist;
    }
    board->history = hist;*/
}
MoveHistory* push_history(Move* move, MoveHistory* last, char piece, char captured) {
    MoveHistory* mv = (MoveHistory*)malloc(sizeof(MoveHistory));
    if (!mv) {
        return NULL;
    }
    mv->piece = piece;
    mv->captured = captured;
    mv->where = move;
    mv->last = last;
    last->next = mv;
    return mv;
}

MoveHistory* pop_history(MoveHistory* last) {
    MoveHistory* to_return = last->last;
    free(last);

    return to_return;
}

void free_history(MoveHistory* history) {
    while (history) {
        MoveHistory* temp = history;
        history = history->last;
        free(temp);
    }
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
