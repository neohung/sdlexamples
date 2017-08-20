#ifndef _LIST_H_
#define _LIST_H_

typedef struct _LIST_ELEMENT{
	void *data;
	_LIST_ELEMENT *prev;
	_LIST_ELEMENT *next;
} LIST_ELEMENT;


typedef struct _LIST{
	//void *data;
	unsigned int size;
	LIST_ELEMENT *head;
	LIST_ELEMENT *tail;
} LIST;

LIST* list_new();
void list_destory(LIST* list);
unsigned int list_size(LIST* list);
LIST_ELEMENT* list_tail(LIST* list);
bool list_add(LIST* list, LIST_ELEMENT* element_after, void* data);
void list_print_int(LIST* list);
//
//ListElement * list_item_at(List *list, u32 index);
//void * list_remove(List *list, ListElement *element) ;
//void list_remove_element_with_data(List *list, void *data);
//ListElement * list_search(List *list, void * data);
//
#endif