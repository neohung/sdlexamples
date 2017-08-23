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
	unsigned int totalImageWidth;
	unsigned int totalImageHeight;
	unsigned int cellWidth;
	unsigned int cellHeight;
	unsigned char firstCharAt; 
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
UIFont* ui_font_new(const char* filename, unsigned int cellWidth, unsigned int cellHeight,char firstCharAt);
void view_put_char_at(UIView* view, unsigned char ch, int x, int y, unsigned int fgcolor, unsigned int bgcolor);
void view_put_string_at(UIView* view, char *string, int x, int y, unsigned int fgcolor, unsigned int bgcolor, int x_offset);
void view_put_rect_at(UIView* view,int dstX, int dstY, unsigned int rows, unsigned int cols, unsigned char borderWidth, unsigned int fgcolor, unsigned int bgcolor);
#endif
