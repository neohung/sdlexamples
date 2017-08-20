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
    //
    LIST* l;
    int a1 =1;
    int a2 =2;
    int a3 =3;
    
    LIST* ll = list_new();
    printf("%p\n",ll);
    list_add(ll, NULL, &a1);

    //list_add(ll, list_tail(ll), &a2);
    //list_add(ll, list_tail(ll), &a3);
    list_add(ll, NULL, &a2);
    list_add(ll, NULL, &a3);
    list_print_int(ll);
    
    list_destory(ll);

    
    
    
    //
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

/*
//Render the Screen from UIScreen
void render_screen(SDL_Renderer *renderer, SDL_Texture *screenTexture, UIScreen *screen)
{
	// Render views from back to front for the current screen
    if (screen == NULL){
    	printf("Can't get any Screen to Render!\n");
    	SDL_RenderClear(renderer);
    	SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    	SDL_RenderPresent(renderer);
    	return;
    }
    if (screen->views == NULL){
        printf("Can't get any View in Screen to Render!\n");
    	SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        return;
    }
    ListElement *e = list_head(screen->views);
	//printf("head=%p\n",e);
    while (e != NULL) {
    	UIView *v = (UIView *)list_data(e);
    	console_clear(v->console);
        v->render(v->console);
    	SDL_UpdateTexture(screenTexture, v->pixelRect, v->console->pixels, v->pixelRect->w * sizeof(u32));
    	e = list_next(e);
    	//SDL_Delay(1000);
    }
	//printf("tail=%p\n",e);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}


global_variable bool gameIsRunning = true;
void quit_game() {
	gameIsRunning = false;
}

// test
#define MENU_LEFT	7//50
#define MENU_TOP	5//28
#define MENU_WIDTH	24
#define MENU_HEIGHT	10

void render_menu_view(Console *console) 
{
    UIRect rect = {0, 0, MENU_WIDTH, MENU_HEIGHT};
    //view_draw_rect(console, &rect, 0x363247FF, 0, 0xFFFFFFFF);
    view_draw_rect(console, &rect, 0x363247FF, 1, 0xFF000088);
    console_put_string_at(console, (char*)"Start a (N)ew game", 2, 3, 0xbca285FF, 0x00000000);
    console_put_string_at(console, (char*)"View (R)anking", 2, 6, 0xbca285FF, 0x00000000);
}

void render_bg_view(Console *console)  
{

    // We should load and process the bg image only once, not on each render
    BitmapImage *bgImage = NULL;
    AsciiImage *aiImage = NULL;
    if (bgImage == NULL) {
        bgImage = image_load_from_file((char*)"./launch.png");
        aiImage = asciify_bitmap(console, bgImage);
    }

    if (asciiMode) {
    //	printf("asciiMode=%d\n",asciiMode);
        view_draw_ascii_image_at(console, aiImage, 0, 0);
    } else {
   // 	printf("asciiMode=%d\n",asciiMode);
        view_draw_image_at(console, bgImage, 0, 0);
    }

    //console_put_string_at(console, "Dark Caverns", 52, 18, 0x556d76FF, 0x00000000);
    console_put_string_at(console, (char*)"Dark Caverns", 2, 18, 0x556d76FF, 0x00000000);

}

//#include "screen_show_ranking.cpp"
//#include "screen_show_in_game.cpp"
bool currentlyInGame = false;
extern UIScreen *screen_show_in_game();
extern UIScreen *screen_show_ranking();
void handle_event_launch(UIScreen *activeScreen, UIEvent event)
{
    if (event.type == UI_KEYDOWN) {
		UIKeycode key = KEYSYM(event);
		switch (key) {
			case SDLK_n: {
				// Start a new game and transition to in-game screen
				game_new();
				ui_set_active_screen(screen_show_in_game());
				currentlyInGame = true;
				}
				break;
			case SDLK_r: {
				ui_set_active_screen(screen_show_ranking());
				}
				break;
			case SDLK_ESCAPE: {
				//quit_game();
				}
				break;
			default:
				break;
		}
	}
}

UIScreen *test_screen = NULL;

UIScreen *screen_test(void)
{
	if (test_screen != NULL)
			return test_screen;
	//
	List *testViews = list_new(NULL);
	//--------------------------------------------------------------------------------------------------------------
	UIRect menuRect = {(16 * MENU_LEFT), (16 * MENU_TOP), (16 * MENU_WIDTH), (16 * MENU_HEIGHT)};
	UIView *menuView = view_new(menuRect, MENU_WIDTH, MENU_HEIGHT,(char*)"./terminal16x16.png", 0, render_menu_view);
	list_insert_after(testViews, NULL, menuView);
	//--------------------------------------------------------------------------------------------------------------
    UIRect bgRect = {0, 0, (16 * BG_WIDTH), (16 * BG_HEIGHT)};
    UIView *bgView = view_new(bgRect, BG_WIDTH, BG_HEIGHT, (char*)"./terminal16x16.png", 0, render_bg_view);
    list_insert_after(testViews, NULL, bgView);
	//--------------------------------------------------------------------------------------------------------------
	return screen_new(testViews, menuView, handle_event_launch);
}
//

int main(int argc, char* argv[]) {

	srand((unsigned)time(NULL));
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0);  //SDL_WINDOW_OPENGL

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    //Create Render
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Before running loop, init activeScreen
    ui_set_active_screen(screen_test());
    while (gameIsRunning) {
        
        playerTookTurn = false;

    	SDL_Event event;
    	u32 timePerFrame = 1000 / FPS_LIMIT;
    	u32 frameStart = 0;
    	while (SDL_PollEvent(&event) != 0) {
    		frameStart = SDL_GetTicks();
    		if (event.type == SDL_QUIT) {
    			printf("Quit\n");
    			quit_game();
    			break;
    		}
    		if (event.type == SDL_KEYDOWN) {
    			SDL_Keycode key = event.key.keysym.sym;
    			switch (key) {
    				case SDLK_ESCAPE: {
    					quit_game();
    					}
    			    	break;
    				case SDLK_t: {
    					asciiMode = !asciiMode;
    					//printf("Press t, asciiMode=%d\n",asciiMode);
    					}
    					break;
    				case SDLK_SPACE: {
    					printf("Press SPACE\n");
    					}
    					break;
    				default:
    					break;
    			}
    			// Send the event to the currently active screen for handling
    			UIScreen *screenForInput = ui_get_active_screen();
    			if ((screenForInput != NULL) && (screenForInput->handle_event != NULL))
    				screenForInput->handle_event(screenForInput, event);
    		}
    	}
    	if (currentlyInGame) {
    		game_update();
    	}
    	//Render active screen
    	render_screen(renderer, screenTexture, ui_get_active_screen());
    	//
    	i32 sleepTime = timePerFrame - (SDL_GetTicks() - frameStart);
    	if (sleepTime > 0) {
    		SDL_Delay(sleepTime);
    	}
    }

    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example
    // Close and destroy the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();
    return 0;
}
*/
