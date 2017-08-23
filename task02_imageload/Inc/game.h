#ifndef _GAME_H_
#define _GAME_H_

#include "ui.h"

#define FPS 30
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void game_update();
void game_render(SDL_Renderer *renderer, SDL_Texture *screenTexture);
unsigned int* game_get_pixels();
UIScreen* game_get_active_screen();
void game_set_active_screen(UIScreen* s);
#endif
