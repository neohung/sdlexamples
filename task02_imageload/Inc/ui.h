#ifndef _UI_H_
#define _UI_H_

#include "SDL.h"
#include "list.h"

#define RED(c) ((c & 0xFF000000) >> 24)
#define GREEN(c) ((c & 0x00FF0000) >> 16)
#define BLUE(c) ((c & 0x0000FF00) >> 8)
#define ALPHA(c) (c & 0xFF)
#define COLOR_FROM_RGBA(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)

#define UIRect SDL_Rect
#define UIEvent SDL_Event
#define UIKEYDOWN SDL_KEYDOWN
#define UIKeycode SDL_Keycode
#define KEYSYM(e) e.key.keysym.sym;
typedef struct _Font{
	UIRect* pixelRect;
	unsigned int* pixels;
} UIFont;


//typedef  UIView UIView;
struct UIView;
struct UIView{
	UIRect* pixelRect;
	unsigned int* pixels;
	UIFont* font;
	void (*render)(UIView* view);
};


typedef struct _Screen{
	LIST* views;
	void (*handle_event)(UIEvent event);
} UIScreen;

typedef struct _BitmapImage{
	unsigned int* pixels;
	unsigned int  width;
	unsigned int height;
} BitmapImage;

UIScreen* ui_screen_new(LIST* views,void (*handle_event)(UIEvent event));
UIView* ui_view_new(UIRect* rect, UIFont* font,void (*render)(UIView* view));
void view_clear(UIView* view);
void view_draw_rect(UIView* view, UIRect rect, unsigned int color);
BitmapImage* ui_load_image(const char* filename);
void view_draw_image_at(UIView* view, BitmapImage *image, int x, int y);
#endif
