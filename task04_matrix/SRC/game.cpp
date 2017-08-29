#include "SDL.h"
#include "game.h"

#include "screen1.h"

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

void game_update()
{
    if (is_update_screen1){
        update_screen1();
    }
}

void game_render(SDL_Renderer *renderer, SDL_Texture *screenTexture)
{
        SDL_RenderClear(renderer);
        //
        if (active_screen){
            LIST_ELEMENT *e = active_screen->views->head;
        	while (e != NULL){
        		UIView* v = (UIView*)e->data;
        		//clear view pixels and rerender
                v->render(v);
        		SDL_UpdateTexture(screenTexture, v->pixelRect, v->pixels, v->pixelRect->w* sizeof(unsigned int));
        		e = e->next;
        	}
        }
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        return;
}
