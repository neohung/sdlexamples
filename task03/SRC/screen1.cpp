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
static int player_x;
static int player_y;

static LIST* monsters;


bool is_update_screen1;

bool can_walk(bool map[][map_h], int x, int y)
{
	return map[x][y];
}

//If check_rooms is null also return true
bool is_in_rooms(bool map[][map_h], UIRect rect)
{
	int i,j;
	for(i=rect.x-1;i<rect.x+rect.w+1;i++){
		for(j=rect.y-1;j<rect.y+rect.h+1;j++){
			if (map[i][j] ==  true){
				return true;
			}
		}
	}
	return false;
}

void get_random_point_from_rect(UIRect rect, int* x, int* y)
{
	int size = rect.w*rect.h;
	int index = rand()%size;
	*x = rect.x + (index % rect.w);
	*y = rect.y + (index / rect.w);
}

// A Simple way to generate a simple maze
void map_generate(bool map[][map_h])
{
	int i,j;
	for(i=0;i<map_w;i++){
		for(j=0;j<map_h;j++){
			map[i][j] =  false; //false means can not walk
		}
	}
	//
	//Random x,y

	int rooms_limit = 5;

	while(rooms_limit > 0){

		int room_x = rand() % map_w + 1;
		int room_y = rand() % map_h + 1;
		int room_w = rand() % 10 + 5;
		int room_h = rand() % 10 + 5;
		if (((room_x + room_w) >= map_w ) || ((room_y + room_h) >= map_h )){
			continue;
		}
		UIRect rect = {room_x,room_y,room_w,room_h};
		//Check if is full wall not cross the room
		if (is_in_rooms(game_map, rect)){
			continue;
		}
		//
		for(i=rect.x;i<rect.x+rect.w;i++){
			for(j=rect.y;j<rect.y+rect.h;j++){
				game_map[i][j] =  true;
			}
		}
		UIRect* room_rect = (UIRect*)malloc(sizeof(UIRect));
		if (room_rect == NULL){
			printf("%s: Fail to malloc room_rect\n", __func__);
			return;
		}
		room_rect->x = rect.x;
		room_rect->y = rect.y;
		room_rect->h = rect.h;
		room_rect->w = rect.w;
		list_add(rooms,NULL,room_rect);
		rooms_limit--;
	} 
	// Gen Rooms finish
	LIST_ELEMENT *e = rooms->head;
	while(e != NULL){
		UIRect* r1 = (UIRect*)e->data;
		if (e->next == NULL){
			break;
		}
		UIRect* r2 = (UIRect*)e->next->data;

		int srcX,srcY,dstX,dstY;
		get_random_point_from_rect(*r1, &srcX, &srcY);
		get_random_point_from_rect(*r2, &dstX, &dstY);

		int xx = dstX;
		int yy = srcY;
		if (srcX > dstX){
					int tmp = dstX;
					dstX = srcX;
					srcX = tmp;
					xx = srcX;
		}
		for(i=srcX;i<=dstX;i++){
				map[i][yy] =  true; //True means can walk
		}

		if (srcY > dstY){
					int tmp = dstY;
					dstY = srcY;
					srcY = tmp;
		}
		for(j=srcY;j<=dstY;j++){
			map[xx][j] =  true; //True means can walk
		}
		e = e->next;
	}

}

void main_render_test(UIView* view)
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
	int i,j;
	unsigned char ch = '#';
	for(i=0;i<map_w;i++){
		for(j=0;j<map_h;j++){
			ch = '#';
			unsigned int fg = 0x777777FE;
			//view_put_char_at(UIView* view, unsigned char ch, int x, int y, unsigned int fgcolor, unsigned int bgcolor)
			if (game_map[i][j] ) //True means can walk
			{
				ch = '.';
				fg = 0x2F2F2F22;
			}
			view_put_char_at(view, ch, i*view->font->cellWidth, j*view->font->cellHeight, fg, 0x222222FE);
		}
	}
	ch = '@';
	view_put_char_at(view, ch, player_x*view->font->cellWidth, player_y*view->font->cellHeight, 0x00FF00FE, 0x222222FE);
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
	         		if (can_walk(game_map, player_x, player_y-1))
	         			player_y-=1;
	         		 add_message(messageList,(char*)"You press UP", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_DOWN: {
	         		if (can_walk(game_map, player_x, player_y+1))
	         		player_y+=1;
	         		 add_message(messageList,(char*)"You press DOWN", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_LEFT: {
	         		if (can_walk(game_map, player_x-1, player_y))
	         		player_x-=1;
	         		 add_message(messageList,(char*)"You press LEFT", 0xFFFFFFFF,0x00000000);
	         	 }
	         	 break;
	         	 case SDLK_RIGHT: {
	         		if (can_walk(game_map, player_x+1, player_y))
	         		player_x+=1;
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

typedef enum {
	POSITION,
	VISIBILITY,
	PHYSICALITY,
	NUM_ATTR
} Attributes;
//-----------------------------------

#define MAX_OSJECTS 10000
#define UNUSED	-1   // For id

typedef struct _Position
{
	int objId;
	int x;
	int y;
	char layer;
} Position;

typedef struct _Visibility
{
	int objId;
	char glyph;
	char* name;
	unsigned int fgcolor;
	unsigned int bgcolor;
	bool canBeenSeen;
} Visibility;

typedef struct _GameObject
{
	int id;
	void* attr[NUM_ATTR];
} GameObject;

GameObject game_object[MAX_OSJECTS];

//Visibility 
LIST* game_vis_list;

//------------------------------------

void gameobject_init(GameObject* go)
{
	int i,j;
	for (i=0 ; i < MAX_OSJECTS ; i++){
		go[i].id = UNUSED;
		for (j=0 ; j < NUM_ATTR ; j++){
			go[i].attr[j] = NULL;
		}
	}
}

//Take UNUSED object and gen a id
GameObject* gameobject_take(GameObject* go)
{
	int i,j;
	for (i=0 ; i < MAX_OSJECTS ; i++){
		if (go[i].id == UNUSED){
			go[i].id = i;
			for (j=0 ; j < NUM_ATTR ; j++){
				go[i].attr[j] = NULL;
			}
			return &go[i];
		}
	}
	return NULL;
}

void init()
{
    rooms = list_new(free);
    map_generate(game_map);

    LIST_ELEMENT *e = rooms->head;
    get_random_point_from_rect(*(UIRect*)e->data, &player_x, &player_y);

    monsters = list_new(free);

    game_vis_list = list_new(free);

    gameobject_init(game_object);

    GameObject* g = gameobject_take(game_object);

    Position* pos = (Position*) malloc(sizeof(Position));
    g->attr[POSITION] = pos;
    pos->objId = g->id;
    pos->x = 1;
	pos->y = 2;

	Visibility* vis = (Visibility*) malloc(sizeof(Visibility));
	g->attr[POSITION] = pos;
	vis->objId = g->id;
	vis->name = (char*)"mon1";
	vis->glyph = '$';
	vis->fgcolor = 0xFFFFFFFF;
	vis->bgcolor = 0x00000000;
	vis->canBeenSeen = true;

	is_update_screen1 = true;
}

void update_screen1()
{
	printf("t\n");

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
    init();
    //
    return s;
}
