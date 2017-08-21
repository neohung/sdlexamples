#include "list.h"
#include <stdio.h>
#include <stdlib.h> //malloc
#include <assert.h>
LIST* list_new(void (*destroy)(void* data))
{
	LIST* newlist = (LIST*) malloc(sizeof(LIST));
	if (newlist == NULL){
		printf("%s: fail to malloc\n",__func__);
		return NULL;
	}
	newlist->size = 0;
	newlist->head = NULL;
	newlist->tail = NULL;
	newlist->destroy = destroy;
	return newlist;
}

unsigned int list_size(LIST* list)
{
	assert(list != NULL);
	return list->size;
}

LIST_ELEMENT* list_tail(LIST* list)
{
	assert(list != NULL);
	return list->tail;
}

bool list_add(LIST* list, LIST_ELEMENT* element_after, void* data)
{
	assert(list != NULL);
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
	list->size += 1;
	return true;
}

void* list_remove(LIST *list, LIST_ELEMENT *element)
{
	assert(list != NULL);
	void *data = NULL;
	if (list_size(list) == 0) {
		printf("%s: Nothing to be remove\n",__func__);
		return NULL;
	}
	if (element == NULL){
		//Remove Head element
		element = list->head;
		list->head = list->head->next;
		if (list->head != NULL){
			//Brcause this element is in head, set prev to null
			list->head->prev = NULL;
		}
	}else{
		LIST_ELEMENT *prevElement = element->prev;
		LIST_ELEMENT *nextElement = element->next;
		if ((prevElement != NULL) && (nextElement != NULL)){
			prevElement->next = nextElement;
			nextElement->prev = prevElement;
		}else{
			if (prevElement == NULL){
				//means in head
				if (nextElement != NULL) { nextElement->prev = NULL; }
				list->head = nextElement;
			}
			if (nextElement == NULL){
				//means in tail
				if (prevElement != NULL) { prevElement->next = NULL; }
				list->tail = prevElement;
			}
		}
	}
	data = element->data;
	free(element);
	list->size -= 1;
	return data;
}

void* list_remove_element_with_data(LIST *list, void *data)
{
	assert(list != NULL);
	if ((list_size(list) == 0) || (data == NULL)) {
			return NULL;
	}
	LIST_ELEMENT *e = list->head;
	while (e != NULL){
		if (e->data == data)
			break;
		e = e->next;
	}
	if (e != NULL) {
		list_remove(list, e);
		return data;
	}
	return NULL;
}

void list_destory(LIST* list)
{
	assert(list != NULL);
	while (list_size(list) > 0) {
			//list_remove would remove the data from list and free LIST_ELEMENT structure
			void *data = list_remove(list, NULL);
			if (data != NULL && list->destroy != NULL) {
				//try to free the dynamically allocated "data"
				list->destroy(data);
			}
	}
	free(list);
}

LIST_ELEMENT* list_get_element_at(LIST* list, unsigned int index)
{
	assert(list != NULL);
	unsigned int i = 0;
	LIST_ELEMENT* e = list->head;
	while(e != NULL){
		if (i == index){
			return e;
		}
		i++;
		e = e->next;
	}
	return NULL;
}

LIST_ELEMENT* list_get_element_with_data(LIST *list, void *data)
{
	assert(list != NULL);
	if ((list_size(list) == 0) || (data == NULL)) {
				return NULL;
	}
	LIST_ELEMENT *e = list->head;
	while (e != NULL){
		if (e->data == data)
			break;
			e = e->next;
	}
	return e;
}

