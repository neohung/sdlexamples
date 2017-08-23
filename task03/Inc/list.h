#ifndef _LIST_H_
#define _LIST_H_

typedef struct _LIST_ELEMENT{
	void *data;
	_LIST_ELEMENT *prev;
	_LIST_ELEMENT *next;
} LIST_ELEMENT;


typedef struct _LIST{
	unsigned int size;
	LIST_ELEMENT *head;
	LIST_ELEMENT *tail;
	void (*destroy)(void* data);
} LIST;

LIST* list_new(void (*destroy)(void* data));
void list_destory(LIST* list);
unsigned int list_size(LIST* list);
LIST_ELEMENT* list_tail(LIST* list);
//element_after=NULL means add to head
bool list_add(LIST* list, LIST_ELEMENT* element_after, void* data);
//element=NULL means remove head element
void * list_remove(LIST *list, LIST_ELEMENT *element);
LIST_ELEMENT* list_get_element_at(LIST* list, unsigned int index);
void* list_remove_element_with_data(LIST *list, void *data);
LIST_ELEMENT* list_get_element_with_data(LIST *list, void *data);

#endif
