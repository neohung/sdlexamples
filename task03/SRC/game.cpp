#include "SDL.h"
#include "game.h"

#include <stdio.h>

static UIScreen* active_screen = NULL;

UIScreen* game_get_active_screen()
{
	return active_screen;
}

void game_set_active_screen(UIScreen* s)
{
	active_screen = s;
}

static SDL_Rect screen_rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
//static char screen_pixels[SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(unsigned int)];
//static unsigned int screen_pixels[SCREEN_WIDTH*SCREEN_HEIGHT];

void game_update()
{

}

/*
unsigned int* game_get_pixels()
{
    return screen_pixels;
}
*/

void game_clear_with_color(unsigned char red, unsigned char green, unsigned char blue)
{
    int i,j;
    unsigned char alpha = 0xFF;
    unsigned int color = red << 24 | green << 16 | blue << 8 | alpha;
    //printf("color=0x%X\n",color);
    for (i=0;i<SCREEN_WIDTH;i++){
        for (j=0;j<SCREEN_HEIGHT;j++){
            //screen_pixels[i+j*SCREEN_WIDTH] = color;
        }       
    }
}

void game_render(SDL_Renderer *renderer, SDL_Texture *screenTexture)
{
      //if (screen == NULL){
      //  printf("Can't get any Screen to Render!\n");
        SDL_RenderClear(renderer);
        //
        //game_clear_with_color(0x77, 0x77, 0x77);
        if (active_screen){
            LIST_ELEMENT *e = active_screen->views->head;
        	while (e != NULL){
        		UIView* v = (UIView*)e->data;
        		//clear view pixels and rerender
                v->render(v);
        		//v->pixelRect = &r;
        		//SDL_UpdateTexture(screenTexture,  v->pixelRect,tmp,v->pixelRect->w);
        		SDL_UpdateTexture(screenTexture, v->pixelRect, v->pixels, v->pixelRect->w* sizeof(unsigned int));
        		e = e->next;
        	}
        }
        //memset(screen_pixels, 0x000000FF,SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(unsigned int));
        //SDL_UpdateTexture(screenTexture, &screen_rect, (void*)screen_pixels, SCREEN_WIDTH * sizeof(unsigned int));
        //
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        return;
    //}

}
