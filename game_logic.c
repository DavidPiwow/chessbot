#include "helper_functions.h"

Board* create_board(void) {
    Board *temp = (Board *)malloc(sizeof(Board));
    if (!temp) {
        printf("FAIL");
        return NULL;
    }

    // 64 places after the start - all sequential, get end of board
    char *board_start = &temp->grid[0];
    temp->board_end = &temp->grid[BOARD_SIZE * BOARD_SIZE];

    // set this to a null pointer for start of chain reasons
    temp->history = NULL;

    char pieces_u[16] = {'C', 'N', 'B', 'Q', 'K', 'B', 'N', 'C',
                         'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};
    char pieces_l[16] = {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
                         'c', 'n', 'b', 'q', 'k', 'b', 'n', 'c'};


    // copy the above arrays into memory and set the rest to chosen EMPTY char
    memset(temp->grid, EMPTY, BOARD_SIZE * BOARD_SIZE);
    memcpy(board_start, pieces_u, sizeof(pieces_u));
    memcpy(temp->board_end - (BOARD_SIZE * 2), pieces_l, sizeof(pieces_l));


    BITBOARD black_pieces = (uint64_t)(0xFFFF) << (64 - 16);
    BITBOARD white_pieces = (uint64_t)(0xFFFF);

    temp->state = (BoardState*)malloc(sizeof(BoardState));

    if (temp->state) {
        temp->state->white = white_pieces;
        temp->state->black = black_pieces;
    }

    return temp;
}

void free_board(Board* board) {
    if (board) {
        if (board->state) 
            free(board->state);
        if (board->history) free_history(board->history);
    }

    free(board);
}

static inline int pawn_is_at_start(char piece, int y) {
    // either at row 1 or 6 
    return (isupper(piece) && (y == 1)) || ((islower(piece) && (y == (BOARD_SIZE - 2))));
}

static inline int pawn_right_direction(char piece, int dy) {
    // uppercase can move "down" the board (pos y)
    // lowercase can move  "up"  the board (neg y)
    return (isupper(piece) && (dy > 0)) || (islower(piece) && (dy < 0));
}

static int verify_pawn(Board *board, Move *move) {
    int dx = move->dx;
    int dy = move->dy;
    // pawns can only ever move 1 in the x or 2 in the y
    if (abs(dx) > 1 || abs(dy) > 2)
        return 0;

    int x1 = move->x1;
    int x2 = move->x2;
    int y1 = move->y1;
    int y2 = move->y2;

    char piece = get_piece_at(board, x1, y1);
    if (!pawn_right_direction(piece, dy)) return 0;

    char goal = get_piece_at(board, x2, y2);
    if (is_same_team(piece, goal)) return 0;

    // if moving forward
    if (goal == EMPTY) {
        if (dx != 0) return 0;
        if (abs(dy) == 2)
            return pawn_is_at_start(piece, y1);
        else if (abs(dy) != 1)
            return 0;
    }
    else {
        if (abs(dx) != 1 || abs(dy) != 1) return 0;
    }

    return 1;
}


int verify_horse(Move* move) {
    // pieces between do not matter
    // horses can move 2 in one direction and 1 in the other
    if (abs(move->dx) == 2) {
        if (abs(move->dy) != 1) return 0;
    }
    else if (abs(move->dy) == 2) {
        if (abs(move->dx) != 1) return 0;
    }

    return 1;
}

int verify_move(Board* board, Move* move) {
    if (out_of_bounds(move)) return 0;

    char piece = get_piece_at(board, move->x1, move->y1);
    char goal = get_piece_at(board, move->x2, move->y2);

    // make sure the pieces arent besties xoxo
    if (piece == '\0' || piece == EMPTY || is_same_team(piece, goal))
        return 0;

    // check if pawn, knight, or wrong direction
    if (toupper(piece) == 'P') {
        if (! verify_pawn(board, move)) return 0;
    }
    else if (toupper(piece) == 'N') {
        if (! verify_horse(move)) return 0;
    }
    else if (!correct_direction(move, piece)) {
        return 0;
    }

    // King can only move one piece at a time (save castling but thats not in here rn xx)
    if (toupper(piece) == 'K') {
        if (abs(move->dx) > 1 || abs(move->dy) > 1) return 0;
    }

    // return if it is empty between or not
    return  attempt_move(board->state, move, islower(piece));
}

int count_in_direction(Board *board, Move *move, int dx, int dy) {
    // Get starting position
    int cur_x = move->x1;
    int cur_y = move->y1;

    int count = 0;
   
    for (int i = 1; i <= BOARD_SIZE; i++) {
        // take a step in the chosen direction
        cur_x += dx;
        cur_y += dy;

        move->dx = dx * i;
        move->dy = dy * i;
		move->x2 = cur_x;
		move->y2 = cur_y;

        //check if it is valid or stop it here
        if (verify_move(board, move)) {
            count++;
        } else {
            break;
        }
    }
    return count;
}
 
int count_horse_moves(Board* board, int x1, int y1) {
    // create a move at current position
    Move mv = (Move){x1, 0, y1, 0, 0 ,0};
    int x_list[8] = {1, -1, -2, 2, -2, 2, 1, -1};
    int y_list[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    int count = 0;
    for (int i = 0; i < 8; i++) {
        mv.x2 = x1 + x_list[i];
        mv.y2 = y1 + y_list[i];
        mv.dx = x_list[i];
        mv.dy = y_list[i];
        if (verify_move(board, &mv)) {
            count++;
        }
    }
    return count;
}

Coordinates get_horse_move(Board* board, int x1, int y1) {
    // create a move at current position
    Move mv = (Move){ x1, 0, y1, 0, 0 ,0 };

    // these are all the possible horse moves (i checked)
    int x_list[8] = { 1, -1, -2, 2, -2, 2, 1, -1 };
    int y_list[8] = { 2, 2, 1, 1, -1, -1, -2, -2 };
    int p = 0;
    int horse_choice = 0;
    for (int i = 0; i < 8; i++) {
        mv.x2 = x1 + x_list[i];
        mv.y2 = y1 + y_list[i];
        mv.dx = x_list[i];
        mv.dy = y_list[i];
        if (!verify_move(board, &mv)) {
            x_list[i] = 0;
            y_list[i] = 0;
        }
    }
    for (; p < 8; p++) {
        if (x_list[p] != 0) {
            horse_choice = p;
        }
    }

    return (Coordinates) { x1 + x_list[horse_choice], y1 + y_list[horse_choice] };
}


// d = down, u = up, .... 
// pu = positive up, pd = positive down,
// nu = positive up, nd = positive down
MoveChoices get_valid_moves(Board *board, int x1, int y1) {
    int d_u, d_l, d_d, d_r, d_pu, d_pd, d_nu, d_nd;
    d_u = d_l = d_d = d_r = d_pu = d_pd = d_nu = d_nd = 0;
    char piece = get_piece_at(board, x1, y1);
    int any = 0;
    Move move = {x1, x1, y1, y1, 0, 0};
    if (toupper(piece) == 'P') {
        if (isupper(piece)) {
            // upper pawns can move down only
            d_d = count_in_direction(board, &move, 0, 1);
            d_pd = count_in_direction(board, &move, 1, 1);
            d_nd = count_in_direction(board, &move, -1, 1);
        } else if (islower(piece)) {
            // lower pawns can move up only
            d_u = count_in_direction(board, &move, 0, -1);
            d_pu = count_in_direction(board, &move, 1, -1);
            d_nu = count_in_direction(board, &move, -1, -1);
        }
    }else if (toupper(piece) == 'N') { 
        any = count_horse_moves(board, x1, y1);
    } else {
        // get steps in each direction
        if (can_move_vertical(piece)) {
            d_d = count_in_direction(board, &move, 0, 1);
            d_u = count_in_direction(board, &move, 0, -1);
        }
        if (can_move_horizontal(piece)) {
            d_r = count_in_direction(board, &move, 1, 0);
            d_l = count_in_direction(board, &move, -1, 0);
        }
        if (can_move_diagonal(piece)) {
            d_pd = count_in_direction(board, &move, 1, 1);
            d_pu = count_in_direction(board, &move, 1, -1);
            d_nd = count_in_direction(board, &move, -1, 1);
            d_nu = count_in_direction(board, &move, -1, -1);
        }
    }
    // add them all up
    any += d_d + d_u + d_r + d_l + d_pd + d_nd + d_pu + d_nu;
    // up, negative diagonals, and left moves need to be turned to negative
    // direction
    d_u *= -1;
    d_l *= -1;
    d_nu *= -1;
    d_nd *= -1;

    return (MoveChoices) {x1, y1, d_u, d_d, d_l, d_r, d_pu, d_pd, d_nu, d_nd, any};
}
