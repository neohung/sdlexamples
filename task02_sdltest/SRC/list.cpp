#include "list.h"
#include <stdio.h>
#include <stdlib.h> //malloc
#include <assert.h>
LIST* list_new()
{
	LIST* newlist = (LIST*) malloc(sizeof(LIST));
	if (newlist == NULL){
		printf("%s: fail to malloc\n",__func__);
		return NULL;
	}
	newlist->size = 0;
	newlist->head = NULL;
	newlist->tail = NULL;
	return newlist;
}

unsigned int list_size(LIST* list)
{
	return list->size;
}

void list_destory(LIST* list)
{
	free(list);
}

LIST_ELEMENT* list_tail(LIST* list)
{
	return list->tail;
}

bool list_add(LIST* list, LIST_ELEMENT* element_after, void* data)
{
	//assert(list == NULL);
	//Create LIST_ELEMENT
	LIST_ELEMENT* new_element =  (LIST_ELEMENT*) malloc(sizeof(LIST_ELEMENT));
	if (new_element == NULL){
		printf("%s: fail to malloc\n",__func__);
		return false;
	}
	new_element->data = data;
	if (element_after == NULL){
		new_element->next = list->head;
		new_element->prev = NULL;
		// Add to Head
		if (list_size(list) == 0) {
			//means tail is null
			list->tail = new_element;
		} else {
			list->head->prev = new_element;
		}
		list->head = new_element;
	}else{
		//
		if (element_after->next == NULL) {
			//means element_after is tail
			list->tail = new_element;
		}
		new_element->next = element_after->next;
		new_element->prev = element_after;
		element_after->next = new_element;
	}
	return true;
}

void list_print_int(LIST* list)
{
	LIST_ELEMENT* e = list->head;
	while (e != NULL)
	{
		printf("%d\n", *(int*)e->data);
		e = e->next;
	}
}