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
	void (*handle_event)();
} UIScreen;

UIScreen* ui_screen_new(LIST* views,void (*handle_event)());
UIView* ui_view_new(UIRect* rect, UIFont* font,void (*render)(UIView* view));
void ui_view_clear(UIView* view);
void ui_view_draw_rect(UIView* view, UIRect rect, unsigned int color);
#endif
