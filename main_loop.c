#include "random_bot.h"
#include "display.h"
#include <time.h>
#include "print_functions.h"
#include "game_logic.h"
#include "file_handler.h"

int main(int argc, char* args[])
{
    //if (argc || args) printf("ok");
    if (SDL_Init(SDL_INIT_VIDEO) > 0) printf("FAIL");
    // initialize random and game board
    srand((unsigned int)time(NULL)); 
    
    Board* game_board = create_board();
    FILE* store = fopen("chess_history.txt","w");

    if (!game_board || !store) return 0;
    
    App* app = open_app(SCREEN_SIZE, SCREEN_SIZE);
    SDL_Texture* piece_sprites = load_image(app);
   // print_board(game_board);
    SDL_Event e; 
    
    int i = 0;
    
    int quit = 0;
    int attempts = 0;
    int round = 1;


    SDL_SetRenderDrawColor(app->renderer, 0,0,0, 255);
    int lower_turn = 1;
    UserMove input = { -1,-1,-1,-1};
    while (!quit && round <= 30){
        SDL_RenderClear(app->renderer);
        draw_board(game_board, app, piece_sprites);
        //SDL_RenderDrawRect(app->renderer, &r);

        SDL_RenderPresent(app->renderer);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && lower_turn) {
                int x = e.button.x;
                int y = e.button.y;
                x -= MARGIN;
                y -= MARGIN;
                x /= DISP_SQUARE_SIZE;
                y /= DISP_SQUARE_SIZE;
                if (input.x1 == -1) {
                    input.x1 = x;
                    input.y1 = y;
                }
                else if (input.x2 == -1) {
                    input.x2 = x;
                    input.y2 = y;


                    Move user_move = { input.x1, input.x2, 
                                    input.y1, input.y2, 
                                    input.x2 - input.x1, input.y2 - input.y1 };
                    if (verify_move(game_board, &user_move)) {

                        printf("%d %d %d %d\n", input.x1, input.y1, input.x2, input.y2);
                        if (store_move(store, move_piece(game_board, &user_move, lower_turn), i, round) == 'K')
                            quit = 1;
                        i++;
                        lower_turn = !lower_turn;

                        input.x1 = input.x2 = input.y1 = input.y2 = -1;

                        SDL_RenderClear(app->renderer);
                        draw_board(game_board, app, piece_sprites);
                        SDL_RenderPresent(app->renderer);

                    }
                    else {
                        input.x1 = input.x2 = input.y1 = input.y2 = -1;
                    }
                } else {
                    input.x1 = input.x2 = input.y1 = input.y2 = -1;
                }

            }

            if (i % 2 == 1) {

                Move mv = get_random_move(game_board, lower_turn);
                while (!verify_move(game_board, &mv) && attempts++ <= 4)
                    mv = get_random_move(game_board, lower_turn);

                if (attempts >= 4) quit = 1;
                SDL_Delay(500);
                if (store_move(store, move_piece(game_board, &mv, lower_turn), i, round) == 'K')
                    quit = 1;

                SDL_RenderClear(app->renderer);
                draw_board(game_board, app, piece_sprites);
                SDL_RenderPresent(app->renderer);


                i++;

                lower_turn = !lower_turn;
            }

        }

       
    }
    print_board(game_board);
    quit = 0;
    while (!quit) {

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                fclose(store);
                SDL_DestroyTexture(piece_sprites);
                free_board(game_board);
                close_app(app);
                store = NULL;
                game_board = NULL;
                piece_sprites = NULL;
                app = NULL;

                SDL_Quit();
                quit = 1;
            }

        }
    }

    return 0;
}
