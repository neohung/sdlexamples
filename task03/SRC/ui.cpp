#include "ui.h"
//To use stbi_load nedd to define this first
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h> //NULL
#include <assert.h>
#include <string.h>

bool system_is_little_endian() {
    unsigned int x = 0x76543210;
    char *c = (char*) &x;
    if (*c == 0x10) { return true; }
    return false;
}

#define SWAP_INTDATA(x) (((x) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8) | ((x) << 24))

UIScreen* ui_screen_new(LIST* views,void (*handle_event)(UIEvent event))
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

UIFont* ui_font_new(const char* filename, unsigned int cellWidth, unsigned int cellHeight,char firstCharAt)
{
	BitmapImage* fontdata = ui_load_image(filename);
	if (fontdata == NULL){
		printf("%s: Fail to load fontdata from [%s]\n",__func__, filename);
		return NULL;
	}
	UIFont* font = (UIFont*) malloc(sizeof(UIFont));
	if (font == NULL){
		printf("%s: Fail to malloc UIFont\n",__func__);
		return NULL;
	}
	font->firstCharAt = firstCharAt;
	font->cellWidth = cellWidth;
	font->cellHeight = cellHeight;
	font->totalImageWidth = fontdata->width;
	font->totalImageHeight = fontdata->height;
	int size = fontdata->width * fontdata->height * sizeof(unsigned int);
	font->pixels = (unsigned int*) malloc(size);
	if (font->pixels == NULL){
		printf("%s: Fail to malloc Font pixels\n",__func__);
		return NULL;
	}
	memcpy(font->pixels, fontdata->pixels, size);
	free(fontdata->pixels);
	free(fontdata);
	return font;
}

UIView* ui_view_new(UIRect* rect, UIFont* font,void (*render)(UIView* view))
{
	UIView* newview = (UIView*) malloc(sizeof(UIView));
	if (newview == NULL){
		printf("%s: Fail to malloc VIEW\n", __func__);
		return NULL;
	}

	UIRect* pixelRect =  (UIRect*) malloc(sizeof(UIRect));
	if (pixelRect == NULL){
			printf("%s: Fail to malloc UIRect\n", __func__);
			free(newview);
			return NULL;
	}
	newview->pixelRect = pixelRect;
	newview->pixelRect->x = rect->x;
	newview->pixelRect->y = rect->y;
	newview->pixelRect->w = rect->w;
	newview->pixelRect->h = rect->h;

	unsigned int* pixels = (unsigned int*) malloc(newview->pixelRect->h*newview->pixelRect->w*sizeof(unsigned int));
	if (pixels == NULL){
		printf("%s: Fail to malloc pixels\n", __func__);
		free(newview->pixelRect);
		free(newview);
		return NULL;
	}
	newview->pixels = pixels;
	newview->font = font;
	newview->render = render;
	return newview;
}

void view_clear(UIView* view)
{
	memset(view->pixels,0,view->pixelRect->h*view->pixelRect->w*sizeof(unsigned int));
}
void view_draw_rect(UIView* view, UIRect rect, unsigned int color)
{
	 //assert((rect.x > view->pixelRect->x) && (rect.y > view->pixelRect->y));
	 //assert((rect.x+rect.w < view->pixelRect->w) && (rect.y+rect.h < view->pixelRect->h));
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

//#include <stdbool.h>
BitmapImage* ui_load_image(const char* filename)
{
	int imgWidth, imgHeight, numComponents;
	unsigned char *imgData = stbi_load(filename,&imgWidth, &imgHeight,&numComponents, STBI_rgb_alpha);
	if (imgData == NULL){
		printf("%s: Fail to stbi_load [%s]\n",__func__, filename);
		return NULL;
	}
	//printf("%s: %dx%d, c=%d", filename, imgWidth, imgHeight, numComponents);
	unsigned size = imgWidth*imgHeight*numComponents;
	unsigned int *imageData = (unsigned int *)malloc(size);
	memcpy(imageData, imgData, size);
	if (system_is_little_endian()) {
		//For little endian, the data is BGR
        unsigned int pixelCount = imgWidth * imgHeight;
        for (unsigned int i = 0; i < pixelCount; i++) {
            imageData[i] = SWAP_INTDATA(imageData[i]);
        }
    }
	BitmapImage *bmi = (BitmapImage *)malloc(sizeof(BitmapImage));
	bmi->pixels = imageData;
	bmi->width = imgWidth;
	bmi->height = imgHeight;
	stbi_image_free(imgData);
	return bmi;
}

void view_draw_image_at(UIView* view, BitmapImage *image, int x, int y)
{
	 assert(x>=0 && y>=0);
	 assert( x+image->width <= view->pixelRect->w && y +image->height <= view->pixelRect->h);
	 unsigned int dstX = x;

	 for (unsigned int srcY = 0; srcY < image->height; srcY++) {
	        // Copy row of pixels from image to *pixels
	        unsigned int dstY = srcY + y;
	        memcpy(&view->pixels[(dstY * view->pixelRect->w) + dstX], &image->pixels[srcY * image->width], image->width * sizeof(unsigned int));
	    }
}

void view_put_char_at(UIView* view, unsigned char ch, int x, int y, unsigned int fgcolor, unsigned int bgcolor)
{
	unsigned int width = view->font->totalImageWidth;
	unsigned int height = view->font->totalImageHeight;
	unsigned int cellWidth = view->font->cellWidth;
	unsigned int cellHeight = view->font->cellHeight;
	unsigned int cols = width / cellWidth;
	unsigned int rows = height / cellHeight;
	unsigned int row = ch / cols;
	assert(row < rows);
	unsigned int col = ch % cols;
	unsigned int offset = (row*width + col )*cellWidth; 
	unsigned int i,j;
	unsigned int dstX = x;
	unsigned int dstY = y;
	//UIRect rect = {dstX, dstY, cellWidth, cellHeight};
	UIRect rect = {dstX, dstY, cellWidth, cellHeight};
	view_draw_rect(view, rect, bgcolor);
	for (i=0;i< cellWidth;i++){
		 for (j=0;j<cellHeight;j++){
		 	unsigned int val = view->font->pixels[j*width+i+offset];
		 	unsigned char is_pixels = RED(val) | GREEN(val) | BLUE(val);
		 	if (is_pixels)
		 	  view->pixels[i+(j+dstY)*view->pixelRect->w + dstX] = fgcolor;
		 }
	}

}

void view_put_string_at(UIView* view, char *string, int x, int y, unsigned int fgcolor, unsigned int bgcolor, int x_offset) 
{
    unsigned int len = strlen(string);
    for (int i = 0; i < len; i++) {
    	view_put_char_at(view, string[i] , x+(i*(view->font->cellWidth+x_offset)),  y, fgcolor, bgcolor);
    }
}

void view_put_rect_at(UIView* view,int dstX, int dstY, unsigned int cols, unsigned int rows, unsigned char borderWidth, unsigned int fgcolor, unsigned int bgcolor)
{
	for (int y = 0; y < rows; y++) {
		 for (int x = 0; x < cols; x++) {
		 	unsigned char c = 0;
		 	// 179 : 186;   (| , ||) Sides
		 	// 218 : 201 Top left corner
		 	// 191 : 187  Top right corner
		 	// 196 : 205  (-,=) Top border
		 	// 192 : 200  Bottom left corner
		 	// 217 : 188  Bottom right corner
		 	// 196 : 205  Bottom border
		 	// Sides 
		 	if (x==0 || x==(cols-1))
		 		c = (borderWidth == 1) ? 179 : 186;
		 	if (y==0){
		 		c = (borderWidth == 1) ? 196 : 205;
		 		if (x == 0){
		 			c = (borderWidth == 1) ? 218 : 201;
		 		}else if (x==(cols-1)){
		 			c = (borderWidth == 1) ? 191 : 187;
		 		}
		 	}

		 	if (y==rows-1){
		 		c = (borderWidth == 1) ? 196 : 205;
		 		if (x == 0){
		 			c = (borderWidth == 1) ? 192 : 200;
		 		}else if (x==cols-1){
		 			c = (borderWidth == 1) ? 217 : 188;
		 		}
		 	}	
    		view_put_char_at(view, c, dstX+(x*view->font->cellWidth),  dstY+(y*view->font->cellHeight), fgcolor, bgcolor);
		}
	}

}
