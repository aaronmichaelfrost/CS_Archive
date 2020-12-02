/*
 * This file is where you should implement your dynamic array.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Aaron Frost
 * Email: frostaa@oregonstate.edu
 */

#include <stdlib.h>

#include "dynarray.h"



struct dynarray {
  void** data;
  int size;
  int capacity;
};


struct dynarray* dynarray_create() {

  struct dynarray* arr = malloc(sizeof(struct dynarray*));

  arr->data = malloc(2 * sizeof(void*));

  arr->capacity = 2;
  arr->size = 0;

  return arr;
}


void dynarray_free(struct dynarray* da) {

  free(da->data);
  
  free(da);

  return;
}


int dynarray_size(struct dynarray* da) {
  return da->size;
}


void dynarray_insert(struct dynarray* da, void* val) {
  int x = 1;

  if(da->capacity == da->size){


    void** temp = da->data;

    void** new_data = malloc(da->size * 2 * sizeof(void*));

    for(int i = 0; i < da->size; i++)
      new_data[i] = temp[i];

    da->data = new_data;

    free(temp);

    da->capacity *= 2;
    
  }


  da->data[da->size] = val;

  da->size++;
  

  return;
}


void dynarray_remove(struct dynarray* da, int idx) {

  if(idx >= 0 && idx < da->size){
    
    for(idx; idx < da->size - 1; idx++)
      da->data[idx] = da->data[idx + 1];
    
    da->size--;

  }
 
  return;
}


void* dynarray_get(struct dynarray* da, int idx) {

  if(idx >= 0 && idx < da->size)
    return da->data[idx];

  return NULL;
}


void dynarray_set(struct dynarray* da, int idx, void* val) {

  if(idx >= 0 && idx < da->size)
    da->data[idx] = val;
  
  return;
}
