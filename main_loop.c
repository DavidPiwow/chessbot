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
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN){
              //  quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
            //    quit = 1;
            }
        }

        SDL_RenderClear(app->renderer);
        draw_board(game_board, app, piece_sprites);
        //SDL_RenderDrawRect(app->renderer, &r);

        SDL_RenderPresent(app->renderer);
        SDL_SetRenderDrawColor(app->renderer, 15, 4, 26, 255);

        Move mv = get_random_move(game_board, i % 2 == 1);

        print_board(game_board);

        while (!verify_move(game_board, &mv) && attempts++ < 4)
            mv = get_random_move(game_board, i % 2 == 1);

        if (attempts >= 4) quit = 1;

        store_move(store,move_piece(game_board, &mv), i, round);
        //print_board(game_board);
        SDL_Delay(10);
        if (i % 2 == 1) round++;
        i++;
    }
    print_board(game_board);
    fclose(store);
    SDL_DestroyTexture(piece_sprites);
    free_board(game_board);
    close_app(app);
    store = NULL;
    game_board = NULL;
    piece_sprites = NULL;
    app = NULL;
    
    SDL_Quit();

    return 0;
}
