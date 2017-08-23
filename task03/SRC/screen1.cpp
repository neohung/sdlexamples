#include "screen1.h"
#include "game.h"

static BitmapImage* image;
static unsigned int x=0,y=0;
void render_test(UIView* view)
{
	if (view == NULL){
		printf("NULL\n");
		return;
	}
	view_clear(view);
	//UIRect rect = {10,10,SCREEN_WIDTH/4,SCREEN_HEIGHT/4};
	//view_draw_rect(view, rect, 0xFF000055);
	//UIRect rect2 = {20,20,SCREEN_WIDTH/8,SCREEN_HEIGHT/8};
	//view_draw_rect(view, rect2, 0x0000FFFE);
	
	//view_draw_image_at(view, image, x, y);
    
    //view_put_char_at(view, 'A', SCREEN_WIDTH/2,SCREEN_HEIGHT/2, 0x00FFFF, 0x77777722);
    
    //view_put_string_at(view , (char*)"This is data", SCREEN_WIDTH/2,SCREEN_HEIGHT/2, 0x00FFFF, 0x77777722, -2); 
    //view_put_rect_at(view,SCREEN_WIDTH/2,SCREEN_HEIGHT/2, 7,15,2,0x00FFFF, 0x77777722);

}


void keyevent(UIEvent event)
{
	 if (event.type == UIKEYDOWN) {
		 UIKeycode key = KEYSYM(event);
	     	 switch (key) {
	         	 case SDLK_q: {
	         		 //quit_window();
	             }
	             break;
	         	 case SDLK_UP: {
	         		 y-=1;
	         	 }
	         	 break;
	         	 case SDLK_DOWN: {
	         		 y+=1;
	         	 }
	         	 break;
	         	 case SDLK_LEFT: {
	         		 x-=1;
	         	 }
	         	 break;
	         	 case SDLK_RIGHT: {
	         		 x+=1;
	         	 }
	         	 break;
	         }
	 }
}

UIScreen* create_screen1(void)
{
	
	image = ui_load_image("test.png");
    UIRect rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    //UIFont* f = ui_font_new("font10x10.png", 10, 10, 0);
    UIFont* f = ui_font_new("terminal16x16.png", 16, 16, 0);
    printf("f 0x%p\n", f);
    UIView* v = ui_view_new(&rect, f,render_test);
    printf("v 0x%p\n", v);
    LIST* viewList = list_new(free);
    list_add(viewList, NULL,v);
    UIScreen *s = ui_screen_new(viewList,keyevent);
     printf("s 0x%p\n", s);
    return s;
}