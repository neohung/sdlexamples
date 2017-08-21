#include "ui.h"
#include <stdio.h>
#include <stdlib.h> //NULL
#include <assert.h>

bool system_is_little_endian() {
    unsigned int x = 0x76543210;
    char *c = (char*) &x;
    if (*c == 0x10) { return true; }
    return false;
}

#define SWAP_INTDATA(x) (((x) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8) | ((x) << 24))

UIScreen* ui_screen_new(LIST* views,void (*handle_event)())
{
	UIScreen* newscreen = (UIScreen*) malloc(sizeof(UIScreen));
	if (newscreen == NULL){
		printf("%s: Fail to malloc SCREEN\n", __func__);
		return NULL;
	}
	newscreen->views = views;
	newscreen->handle_event = handle_event;
	return newscreen;
}

UIView* ui_view_new(UIRect* rect, UIFont* font,void (*render)(UIView* view))
{
	UIView* newview = (UIView*) malloc(sizeof(UIView));
	if (newview == NULL){
		printf("%s: Fail to malloc VIEW\n", __func__);
		return NULL;
	}
	newview->pixelRect = rect;
	unsigned int* pixels = (unsigned int*) malloc(newview->pixelRect->h*newview->pixelRect->w*sizeof(unsigned int));
	if (pixels == NULL){
		printf("%s: Fail to malloc pixels\n", __func__);
		free(newview);
		return NULL;
	}
	newview->pixels = pixels;
	newview->font = font;
	newview->render = render;
	return newview;
}

#include <string.h>
void ui_view_clear(UIView* view)
{
	memset(view->pixels,0,view->pixelRect->h*view->pixelRect->w*sizeof(unsigned int));
}
void ui_view_draw_rect(UIView* view, UIRect rect, unsigned int color)
{
	 assert((rect.x > view->pixelRect->x) && (rect.y > view->pixelRect->y));
	 assert((rect.x+rect.w < view->pixelRect->w) && (rect.y+rect.h < view->pixelRect->h));
	 int i,j;
	 //unsigned char alpha = 0xFF;
	 //unsigned int color = 0xFF << 24 | 0x00 << 16 | 0x00 << 8 | alpha;
	 for (i=0;i< rect.w;i++){
		 for (j=0;j<rect.h;j++){
			 if (ALPHA(color) == 0){
			 	//Do nothing
			 }else if (ALPHA(color) == 0xFF){
				 //No blending need
				 view->pixels[rect.x+i+(j+rect.y)*view->pixelRect->w] = color;
			 }else{
				 //blending
				 unsigned int srcColor = view->pixels[rect.x+i+(j+rect.y)*view->pixelRect->w];
				 unsigned char colorAlpha = (ALPHA(srcColor)) * (ALPHA(color) / 255.0);
				 if (ALPHA(srcColor) == 0){
					 //Just overwrite
					 view->pixels[rect.x+i+(j+rect.y)*view->pixelRect->w] = color;
				 }else{
					 if (ALPHA(srcColor) == 255){
						 srcColor = color;
					 }
					 float srcA = ALPHA(srcColor) / 255.0;
					 float invSrcA = (1.0 - srcA);
					 float destA = ALPHA(color) / 255.0;
					 float outAlpha = srcA + (destA * invSrcA);
					 unsigned char fRed = ((RED(srcColor) * srcA) + (RED(color) * destA * invSrcA)) / outAlpha;
					 unsigned char fGreen = ((GREEN(srcColor) * srcA) + (GREEN(color) * destA * invSrcA)) / outAlpha;
					 unsigned char fBlue = ((BLUE(srcColor) * srcA) + (BLUE(color) * destA * invSrcA)) / outAlpha;
					 unsigned char fAlpha = outAlpha * 255;
					 view->pixels[rect.x+i+(j+rect.y)*view->pixelRect->w] = COLOR_FROM_RGBA(fRed, fGreen, fBlue, fAlpha);
				 }
			 }
		 }
	 }
}
