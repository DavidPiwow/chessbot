#include "helper_functions.h"
#include "random_bot.h"

static inline int choose_random(int i1, int i2) {
    if (i1 == 0 || i2 == 0) return i1 ? i1 : i2;
    return (rand() % 10) % 2 == 0 ? i1 : i2;
}


Coordinates get_ran_pos(Board *board, int up) {
    Coordinates *positions = malloc(sizeof(Coordinates) * 17);
    if (!positions) {
        printf("FAIL");
        return ((Coordinates) { EMPTY, EMPTY });
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

    int pos = rand() % (pos_count);
    pos = pos > pos_count ? pos_count : pos;
    int x,y;
    x = positions[pos].x;
    y = positions[pos].y;
    Coordinates c = {x, y};
    free(positions);
    
    return c;
}


Move get_random_horse(Board *board, int x, int y) {
    Coordinates horse_pos = get_horse_move(board, x, y);
    int dx = horse_pos.x - x;
    int dy = horse_pos.y - y;

    return (Move) {x, horse_pos.x, y, horse_pos.y, dx, dy};
}

Move get_random_pawn(Board *board, int x, int y)
{
    int dy, rx, ry;
    dy = rx = ry = 0;
    char p = get_piece_at(board, x, y);

    if ((islower(p) && y == 6) || (isupper(p) && y == 1))
        dy = ((rand() % 10) % 2) == 0 ? 2 : 1;
    else
        dy = 1;

    if (p == 'p') {
        dy *= -1;
    }
    
    ry = y + dy;

    char goal = get_piece_at(board, x + 1,ry);
    if (goal != EMPTY && !is_same_team(p, goal)) {
        rx = 1;
    }
    goal = get_piece_at(board, x - 1,ry);
    if (goal != EMPTY && !is_same_team(p, goal)) {
        rx = choose_random(rx, -1);
    }

    Move mv;

    if ((rx > BOARD_SIZE - 1 || rx < 0) ||
        (ry > BOARD_SIZE - 1 || ry < 0) ||
        (is_same_team(get_piece_at(board, x, y), get_piece_at(board, rx, ry)))) 
    {
        mv = (Move){0, 0, 0, 0, 0, 0};
    } else {
        mv = (Move){x, x + rx, y, ry, rx, dy};
    }

    return mv;
}

Move get_random_move(Board *board, int up) {
    Coordinates pos = get_ran_pos(board, up);

    MoveChoices choices = get_valid_moves(board, pos.x, pos.y);
    char piece = get_piece_at(board, pos.x, pos.y);
    int tries = 0;

    
    while (!choices.any || toupper(piece) == 'N' || toupper(piece) == 'P' && tries++ <= 3) {
        if (toupper(piece) == 'N') {
            int tries = 0;
            Move mv = get_random_horse(board, pos.x, pos.y);

            while (!verify_move(board, &mv) && tries < 8) {
                mv = get_random_horse(board, pos.x, pos.y);
                tries++;
            } if (verify_move(board, &mv)) {
                return mv;
            } else {
                choices.any = 0;
            }
        } else if (toupper(piece) == 'P') {
            int tries = 0;
            Move mv = get_random_pawn(board, pos.x, pos.y);

            while (!verify_move(board, &mv) && tries < 8) {
                mv = get_random_pawn(board, pos.x, pos.y);
                tries++;
            } if (verify_move(board, &mv)) {
                return mv;
            } else {
                choices.any = 0;
            }
        }
        
        pos = get_ran_pos(board, up);
        choices = get_valid_moves(board, pos.x, pos.y);

        piece = get_piece_at(board, pos.x, pos.y);
    }

    if (tries > 3) return (Move){EMPTY, EMPTY, EMPTY, EMPTY};
    
    Move mv;

    if (can_move_horizontal(piece) || can_move_vertical(piece)) {
        
        mv = random_straight(board, &choices, &pos);
    } else {
        mv = random_diagonal(&choices);
    }
  

    return mv;
}

Move random_diagonal(MoveChoices* choices) {
    int dx, dy;
    dx = dy = 0;

    if (choices->d_pd || choices->d_pu) {
        dx = choose_random(choices->d_pd, choices->d_pu);

        if (dx != choices->d_pd) {
            dy = -1;
        } else {
            dy = 1;
        }
    }
    int flip_x;
    flip_x = dx;
    int d_dx, t_ty;
    if (choices->d_nd || choices->d_nu) {
        d_dx = choose_random(choices->d_nd, choices->d_nu);

        if (choices->d_nu == d_dx) {
            t_ty = 1;
        } else {
            t_ty = -1;
        }

        if (dx == 0) {
            dx = d_dx;
            dy = t_ty;
        } else {    
            dx = choose_random(dx, d_dx);
            dy = flip_x == dx ? dy : t_ty;
        }
    }

    int rx = dx;
    int ry = dx * dy;
    
    return (Move){choices->x,choices->x + dx, choices->y, choices->y + ry, rx, ry};
}

Move random_straight(Board* board, MoveChoices* choices, Coordinates* start) {
    int x, y;
    x = start->x; y = start->y;
    
    int dx =  choose_random(choices->d_r, choices->d_l);
    int dy =  choose_random(choices->d_u, choices->d_d);

    int d_choice = (rand() % 10) % 2;

    if (dx != 0 && dy != 0){
        if (d_choice) {
            dx = 0;
        } else {
            dy = 0;
        }
    }

    int x_rand = x + dx;
    int y_rand = y + dy;

    char piece = get_piece_at(board, x_rand, y_rand);
    char check = toupper(piece);

    int rx, ry;
    rx = dx;
    ry = dy;
    
    if (dx > 0) rx = (rand() % abs(dx)) + 1;
    if (dy > 0) ry = (rand() % abs(dy)) + 1;
    
    if (check == 'K') {
        if (rx != 0)
            rx = rx > 0 ? 1 : -1;
        if (ry != 0)
            ry = ry > 0 ? 1 : -1;
    } else if ((check != 'N') && (check != 'P')) {
            if (dx > 0)
                dx = dx > 0 ? rx : rx * -1;
            if (dy > 0)
                dy = dy > 0 ? ry : ry * -1;
    }

    return (Move){x, x+ rx, y, y + ry, rx, ry};
}
