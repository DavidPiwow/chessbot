#include "print_functions.h"

#define green "\033[1;32m"
#define red "\033[1;31m"
#define purple "\033[1;35m"
#define whte "\033[1;37m"
#define grey "\033[1;30m"

static inline int is_special(int x, int y, int _x, int _y) {
    return (x - BOARD_SIZE * y == _x) && (y == _y);
}

void print_move(Board* board, Move* move) {
    Coordinates start = (Coordinates){ move->x1, move->y1 };
    Coordinates end = (Coordinates){ move->x2, move->y2 };
    print_coords(&start);
    printf(" -> ");
    print_coords(&end);
    char c = get_piece_at(board, move->x2, move->y2);
    if (c != EMPTY) {
		printf(" Captured: %c", c);
    }
    printf("\n");
}


void print_coords(Coordinates* c) {
    printf("(%c, %c) ", c->x + 'A', BOARD_SIZE - (c->y) + '0');
}




void print_board(Board* board) {
   // system("cls");
    char* cur_pos = board->grid;
    printf("%s\t    A\t    B\t    C\t    D\t    E\t    F\t    G\t    H\n\n8%s",purple, whte);

    int row = 7;
    int xl, yl, _x, _y, x, y;
    xl = yl =  _x =  _y = -1;
    x = y = 0;

    if (board->history) {
        MoveHistory* last = board->history;
        xl = last->where.x;
        yl = last->where.y;
        if (last->last) {
           _x = last->last->where.x;
           _y = last->last->where.y;
        }
    }
    
    while (cur_pos != board->board_end) {
        if (is_special(x, y, xl, yl) || is_special(x, y, _x, _y)) {
            if (isupper(*cur_pos)) {
                printf("%s\t| %s  %c%s",grey, red, *cur_pos, grey);
            } else {        
                printf("%s\t| %s  %c%s",grey, green, *cur_pos, grey);
            }
        } else {
            printf("%s\t|%s   %c%s",grey, whte, *cur_pos, grey);
        }
        
        cur_pos++;
        
        if ((cur_pos - board->grid) % BOARD_SIZE == 0) {
            printf(grey);
            if (row == 0) {
                printf(whte);
                continue;
            }
            printf("\t|\n\t-----------------------------------------------------------------\n");
            printf("%s%d%s", purple, row, whte);
            row--;
            y++;
        }
        
        x++;
    }
    printf("\t%s|\n%s",grey, whte);
}

Move get_move(void) {
    printf("\nEnter a move(xy -> xy): ");
    char move[20];
    int vars[4] = {0,0,0,0};
    int pos = 0;
    if (fgets(move,20,stdin)) {
        for (int i = 0; i <= 9; i++) {
            char cur = move[i];
            if (cur == 'x') exit(1);
            if (cur == '-' || cur == '>' || cur == ' ') {
                continue;
            }
            if (cur == '\0') {
                break;
            }
            
            int n = num_from_char(cur);
            if (n != EMPTY && pos <= 3) {
                vars[pos] = num_from_char(cur);
                pos++;
            } else if (pos > 3) {
                break;
            } else {
                printf("Invalid move. Try again");
                return get_move();
            }
        }
    }
    int y1, y2;
    y1 = BOARD_SIZE - vars[1];
    y2 = BOARD_SIZE - vars[3];
    // x1, x2, y1, y2, dx, dy
    Move x = {vars[0], vars[2], y1, y2,
    vars[2]-vars[0], y2-y1};
    return x;
}
