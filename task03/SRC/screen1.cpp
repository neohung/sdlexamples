#include "screen1.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

static BitmapImage* image;
static unsigned int x=SCREEN_WIDTH/4,y=SCREEN_HEIGHT/4;
static LIST* messageList;
bool is_show_i_windows = false;

typedef struct _message{
	char* msg;
	unsigned int fgcolor;
	unsigned int bgcolor;
} message;

#define map_w 39
#define map_h 23

static bool game_map[map_w][map_h];
static LIST* rooms;
//If check_rooms is null also return true
bool is_in_rooms(LIST* check_rooms, UIRect rect)
{
	if (check_rooms == NULL)
		return true;
	LIST_ELEMENT *e = check_rooms->head;
	while (e != NULL){
		UIRect* r = (UIRect* )e->data;
		if (rect.x > r->x) && (rect.x ){
			return true;
		}
		e = e->next;
	}
	return false;
}

void map_generate()
{
	int i,j;
	for(i=0;i<map_w;i++){
		for(j=0;j<map_h;j++){
			game_map[i][j] =  false; //True means can not walk	
		}
	}
	/*
	int ii = rand() % 2;
			
			if (ii == 0)
			game_map[i][j] =  true; //True means can walk
			else
			game_map[i][j] =  false; //True means can not walk
			*/
	//Random x,y
	int rooms_limit = 4;
	while(rooms_limit > 0){
	int room_x = rand() % map_w + 1;
	int room_y = rand() % map_h + 1;
	//game_map[x][y] =  true;
	int room_w = rand() % 10 + 5;
	int room_h = rand() % 10 + 5;
	if (((room_x + room_w) >= map_w ) || ((room_y + room_h) >= map_h )){
		continue;
	}
	UIRect rect = {room_x,room_y,room_w,room_h};
	if (is_in_rooms(rooms, rect)){
		continue;
	}else{
		printf("Got \n");
	}
	for(i=room_x;i<room_x+room_w;i++){
		for(j=room_y;j<room_y+room_h;j++){
			game_map[i][j] =  true;
		}
	}
	//game_map[room_x][room_y] =  true;
	rooms_limit--;
	} 


}

void main_render_test(UIView* view)
{
	if (view == NULL){
		printf("NULL\n");
		return;
	}
	view_clear(view);

	UIRect rect = {10,10,SCREEN_WIDTH/4,SCREEN_HEIGHT/4};
	view_draw_rect(view, rect, 0xFF000055);
	UIRect rect2 = {20,20,SCREEN_WIDTH/8,SCREEN_HEIGHT/8};
	view_draw_rect(view, rect2, 0x0000FFFE);
	int i,j;
	unsigned char ch = '#';
	for(i=0;i<map_w;i++){
		for(j=0;j<map_h;j++){
			ch = '#';
			
			//view_put_char_at(UIView* view, unsigned char ch, int x, int y, unsigned int fgcolor, unsigned int bgcolor)
			if (game_map[i][j] ) //True means can walk
				ch = '.';
			view_put_char_at(view, ch, i*view->font->cellWidth, j*view->font->cellHeight, 0x777777FE, 0x222222FE);
		}
	}
}

void message_render_test(UIView* view)
{
	if (view == NULL){
			printf("NULL\n");
			return;
		}
		view_clear(view);
		view_put_rect_at(view,0, 0, view->pixelRect->w/view->font->cellWidth,view->pixelRect->h/view->font->cellHeight,1,0x00FFFF, 0x77777722);
		LIST_ELEMENT *e = messageList->head;
		int i=0;
		while (e !=NULL){
			i++;
			message* msg = (message*)e->data;
			if (msg != NULL){
				view_put_string_at(view , (char*)msg->msg, 16,i*view->font->cellHeight, msg->fgcolor, msg->bgcolor, -2);
			}
			e = e->next;
		}
}

void i_render_test(UIView* view)
{
	view->pixelRect->x = x;
	view->pixelRect->y = y;
	view_draw_image_at(view, image, 0, 0);
	//
	view_put_string_at(view , (char*)"Test Window", view->pixelRect->w/2-70,32, 0xFF00FFFF, 0x00000000, -2);
}

#include <string.h>
void add_message(LIST* list,char* str, unsigned int fgcolor, unsigned int bgcolor)
{
	unsigned int lines_limit = 8;
	message* m = (message*) malloc(sizeof(message));
	//  m = {.msg = t, .color=0xFFFFFFFF};
	if (m == NULL){
		printf("%s: Fail to add message\n",__func__);
		return;
	}
	unsigned int len = strlen(str);
	m->msg = (char*)malloc((len+1)*sizeof(char));
	memcpy(m->msg,str,len);
	m->msg[len] = '\0';
	m->fgcolor = fgcolor;
	m->bgcolor = bgcolor;
	if (list->size >= lines_limit){
		//Remove the element, and need to release msg memory
		char* need_free_data = (char*)list_remove(list, NULL);
		free(need_free_data);
	}
	list_add(list, list->tail,(void*)m);
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
	         		 add_message(messageList,(char*)"You press UP", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_DOWN: {
	         		 y+=1;
	         		 add_message(messageList,(char*)"You press DOWN", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_LEFT: {
	         		 x-=1;
	         		 add_message(messageList,(char*)"You press LEFT", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_RIGHT: {
	         		 x+=1;
	         		 add_message(messageList,(char*)"You press RIGHT", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_i: {
	         		is_show_i_windows = !is_show_i_windows;
	         		if (is_show_i_windows){
	         			 UIScreen* active_sreen = game_get_active_screen();
	         			 if (active_sreen){
	         			 UIRect  i_rect = {0,0,image->width,image->height};
	         			 UIFont* f = ui_font_new("terminal16x16.png", 16, 16, 0);
	         			 UIView* i_v = ui_view_new(&i_rect, f,i_render_test);
	         			 list_add(active_sreen->views, active_sreen->views->tail,(void*)i_v);
	         			 }
	         		}else{
	         			 UIScreen* active_sreen = game_get_active_screen();
	         			if (active_sreen){
	         				UIView* need_free_data = (UIView*)list_remove(active_sreen->views, active_sreen->views->tail);
	         			}
	         		}
	         		 add_message(messageList,(char*)"You press i", 0xFFFFFFFF,0x00000000);

	         	 }
	         	 break;
	         	 case SDLK_g:{
	         		memset(game_map, 1, map_w*map_h);
	         	 }
	         	 break;
	         }
	 }
}

UIScreen* create_screen1(void)
{
	
	image = ui_load_image("test.png");
    UIRect main_rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT-100};
    UIRect message_rect = {0,SCREEN_HEIGHT-100, SCREEN_WIDTH, 100};
    //UIFont* f = ui_font_new("font10x10.png", 10, 10, 0);
    UIFont* f = ui_font_new("terminal16x16.png", 16, 16, 0);
    UIView* main_v = ui_view_new(&main_rect, f,main_render_test);
    UIFont* f2 = ui_font_new("font10x10.png", 10, 10, 0);
    UIView* message_v = ui_view_new(&message_rect, f2,message_render_test);

    LIST* viewList = list_new(free);
    list_add(viewList, NULL,(void*)message_v);
    list_add(viewList, NULL,(void*)main_v);

    messageList = list_new(free);

    UIScreen *s = ui_screen_new(viewList,keyevent);
    //
    rooms = list_new(free);
    map_generate();
    //
    return s;
}
