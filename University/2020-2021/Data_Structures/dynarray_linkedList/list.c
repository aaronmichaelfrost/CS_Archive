/*
 * This file is where you should implement your linked list.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Aaron Frost
 * Email: frostaa@oregonstate.edu
 */

#include <stdlib.h>

#include "list.h"



struct link {
  void* val;
  struct link* next;
};


struct list {
  struct link* head;
};


struct list* list_create() {
  struct list* l = malloc(sizeof(struct list));
  
  return l;
}


void list_free(struct list* list) {
  
  struct link* temp;

  while(list->head){
    

    temp = list->head;

    list->head = list->head->next;

    free(temp);

  }


  free(list);

  return;
}


void list_insert(struct list* list, void* val) {

  struct link* temp = malloc(sizeof(struct link));

  temp->val = val;

  temp->next = list->head;

  list->head = temp;

  return;
}


void list_remove(struct list* list, void* val, int (*cmp)(void* a, void* b)) {
  
  struct link* temp = list->head;
  struct link* prev;

  while(temp){
    
    if(!cmp(val, temp->val)){

      if(prev)
        prev->next = temp->next;
      else
        list->head = temp->next;

      free(temp);
      break;
	}

    prev = temp;
    temp = temp->next;
  }


  return;
}


int list_position(struct list* list, void* val, int (*cmp)(void* a, void* b)) {

  struct link* temp = list->head;

  int index = 0;

  while(temp != NULL){

    if(!cmp(val, temp->val))
      return index;
    
    temp = temp->next;
    index++;

  }

  return -1;

}


void list_reverse(struct list* list) {

  struct link* curr = list->head;
  struct link* prev = NULL;
  


  while(curr){
    struct link* temp = curr->next;
    curr->next = prev;
    prev = curr;

    if(!temp)
      list->head = curr;
	
    curr = temp;
    
  }

  return;
}