#include "SDL.h"
#include "game.h"
#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>    //time()
#include <stdbool.h>
#include <stdint.h>

#include "screen1.h"

#ifdef __MINGW32__
#define OS "MINGW32"
#undef main /* Prevents SDL from overriding main() */
#else
#define OS ""
#endif

bool isLooping = true;

void quit_window()
{
    isLooping = false;    
}

#include "matrix.h"
int main()
{
    srand(unsigned(time(NULL)));
    SDL_Init(SDL_INIT_VIDEO);
    //SDL_CreateWindow( Title Name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0)
    SDL_Window *window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL){
        printf("Can not create window!\n");
        exit(-1);
    }
    //
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    //========================================================
    //UIScreen* s = create_screen1();
    //game_set_active_screen(s);
/*
    double X[]  = { 0, 1, 0,
       		        0, 0.1, 0.9,
   				    0.6, 0.4, 0};
*/
    double X[]  = { 0.65, 0.28, 0.07,
         		    0.15, 0.67, 0.18,
     				0.12, 0.36, 0.52};

    //double Y[]  = { 0.5, 0.2, 0.3};

    //double Y[]  = { 0.21, 0.68, 0.11};
    double Y[]  = { 0.31, 0.63, 0.06};
    Matrix* a = matrixNew(X, 3, 3);
    Matrix* b = matrixNew(Y, 1, 3);
    Matrix* c = matrixMul(b,a);

    int i;
    for(i=0;i<100;i++){
    	matrixIterMul(c,a);
    //	c = matrixMul(c,a);
    }
    matrixPrint(c);

    //========================================================
    unsigned int timePerFrame = 1000 / FPS ;
    unsigned int current_ticks = 0;
    while(isLooping){
        SDL_Event event;
        while(SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) {
                quit_window();
                break;
            }
            current_ticks = SDL_GetTicks();
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                switch (key) {
                    case SDLK_ESCAPE: {
                        quit_window();
                    }
                    break;
                }
            }
            UIScreen *active_screen = game_get_active_screen();
            if (active_screen){
            	if (active_screen->handle_event){
            		active_screen->handle_event(event);
            	}
            }
        }
        //Do update
        game_update();
        //
        game_render(renderer, screenTexture);
        //
        int delay_ticks = timePerFrame - (SDL_GetTicks() - current_ticks);
        if (delay_ticks > 0)
            SDL_Delay(delay_ticks);
    }
    //
    SDL_DestroyWindow(window);
    SDL_Quit();
  return 0;
}
