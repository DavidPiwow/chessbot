#include "bitboard.h"

// welcome to binary place, nonbinaries beware....... 
// if any bugs exist it is because of that not because of anything else

// helper function to print a bit board
void print_num(BITBOARD* board) {
    // grab the most significant digit of the board
    uint64_t  i = (uint64_t)1 << (SQUARE_COUNT - 1);

    int c = 0;

    while (i > 0) {
        // print out the digit using &
        printf("%d", (*board & i) ? 1 : 0);
        // move one to the right
        i = i >> 1;
        c++;
        // row spacing please
        if (c % BOARD_SIZE == 0) printf("\n");
    }
    printf("\n");
}


// (SQUARE_COUNT - 1) because the first bit is set so we are one ahead
// subtract by x to get to the right column position
// subtract by y * row size to get to the correct row
inline static BITBOARD make_mask(int x, int y) {
    return (uint64_t)1 << ((SQUARE_COUNT - 1 - x) - (BOARD_SIZE  * y));
}


// OUT: moves the original mask by x and y, be aware it CHANGES THE ORIGINAL MASK
static void shift_mask(BITBOARD* original_mask, int x, int y) {
    int x_shift = abs(x);
    int y_shift = abs(y) * BOARD_SIZE;
    // negative shifts aren't allowed so this is the best i got
    if (x > 0) {
        *original_mask = *original_mask >> x_shift;
    }
    else {
        *original_mask = *original_mask << x_shift;
    }

    if (y > 0) {
        *original_mask = *original_mask >> y_shift;
    }
    else {
        *original_mask = *original_mask << y_shift;
    }
}


// start at x1 and make a mask of the steps between x1 to x2
static BITBOARD create_move_mask(struct _move* move) {
    int x = move->x1;
    int y = move->y1;
    int c = abs(move->dx) ? abs(move->dx) : abs(move->dy);
    int dx, dy;
    dx = dy = 0;
    if (abs(move->dx) > 0)
        dx = move->dx > 0 ? 1 : -1;
    if (abs(move->dy) > 0)
        dy = move->dy > 0 ? 1 : -1;

    // copy the mask and then shift it by
    // the normalized direction and use
    // bitwise OR to stitch them together
    BITBOARD mask = make_mask(x + dx, y + dy);
    BITBOARD copy = mask;
    while (--c > 1) {
        shift_mask(&copy, dx, dy);
        mask = mask | copy;
    }
    return mask;
}

// check if there is already a piece at that position on 
// the team's board
inline static int has_piece(BITBOARD* board, Move* move) {
    BITBOARD mask = create_move_mask(move);

    if (*board & mask) return 1;

    return 0;
}

int piece_swap(BoardState* game_state, struct _move* move, int white_team) {
    BITBOARD team_board = white_team ? game_state->white : game_state->black;
    BITBOARD piece_location = make_mask(move->x1, move->y1);

    if (!(team_board & piece_location)) return 0;

    BITBOARD enemy_board = white_team ? game_state->black : game_state->white;

    // remove the old location from the board
    team_board = team_board ^ piece_location;

    // move piece location mask to the new position
    shift_mask(&piece_location, move->dx, move->dy);

    // if its there on the enemy board, we capture it
    if (enemy_board & piece_location)
        enemy_board = enemy_board ^ piece_location;

    // update the team board to have the new location
    team_board = team_board | piece_location;

    // set the boards in the state
    if (white_team) {
        game_state->white = team_board;
        game_state->black = enemy_board;
    }
    else {
        game_state->white = enemy_board;
        game_state->black = team_board;
    }
    return 1;
}

int attempt_move(BoardState* game_state, struct _move* move, int white_team) {
    BITBOARD team_board = white_team ? game_state->white  : game_state->black;
    BITBOARD piece_location = make_mask(move->x1, move->y1);

    if (!(team_board & piece_location)) return 0;

    BITBOARD enemy_board = white_team ? game_state->black : game_state->white;

    team_board = team_board ^ piece_location;

    shift_mask(&piece_location, move->dx, move->dy);

    if (enemy_board & piece_location) enemy_board = enemy_board ^ piece_location;


    BITBOARD total_board = team_board | enemy_board;

    // create a mask of the pieces BETWEEN the ends of the move
    BITBOARD move_mask = create_move_mask(move);

    if (total_board & move_mask) {
        return 0;
    }

    return 1;
}
