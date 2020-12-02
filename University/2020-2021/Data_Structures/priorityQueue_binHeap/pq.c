/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Aaron Frost
 * Email: frostaa@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"


/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* arr;
};


struct pq_node {
	void* value;
	int priority;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {

	struct pq* pq_ptr = malloc(sizeof(struct pq));
	pq_ptr->arr = dynarray_create();

	return pq_ptr;
}


int pq_node_parent_index(int index) {
	return (int)((index - 1) / 2);
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	int size = dynarray_size(pq->arr);

	for (size_t i = 0; i < size; i++)
		free(dynarray_get(pq->arr, i));

	dynarray_free(pq->arr);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if (dynarray_size(pq->arr) > 0)
		return 0;
	return 1;
}


void pq_node_swap(struct pq_node* node1, struct pq_node* node2) {
	struct pq_node temp_node = *node2;
	*node2 = *node1;
	*node1 = temp_node;
}


// Index refers to the index of the node that needs to percolate 
void pq_percolate_up(struct pq* pq, int index) {

	struct pq_node* parent_node_ptr = dynarray_get(pq->arr, pq_node_parent_index(index));
	struct pq_node* current_node_ptr = dynarray_get(pq->arr, index);

	while (pq_node_parent_index(index) >= 0 && current_node_ptr->priority < parent_node_ptr->priority)
	{
		pq_node_swap(current_node_ptr, parent_node_ptr);

		index = pq_node_parent_index(index);

		parent_node_ptr = dynarray_get(pq->arr, pq_node_parent_index(index));
		current_node_ptr = dynarray_get(pq->arr, index);
	}
}


int pq_node_smallest_child_index(struct pq* pq, int index) {

	int l_i = 2 * index + 1;
	int r_i = l_i + 1;
	int s = dynarray_size(pq->arr);

	struct pq_node* l;
	struct pq_node* r;

	int min = -1;

	if (l_i < s) {

		l = pq_get_node(pq, l_i);

		if (r_i < s && pq_get_node(pq, r_i)->priority < l->priority)
			min = r_i;
		else
			min = l_i;
	}

	return min;
}


void pq_percolate_down(struct pq* pq) {

	int cur_index = 0;
	int min_index = pq_node_smallest_child_index(pq, cur_index);

	struct pq_node* cur = pq_get_node(pq, cur_index);


	while (min_index > -1 && cur->priority > pq_get_node(pq, min_index)->priority) {
		pq_node_swap(cur, pq_get_node(pq, min_index));
		cur_index = min_index;
		cur = pq_get_node(pq, cur_index);
		min_index = pq_node_smallest_child_index(pq, cur_index);
	}
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	struct pq_node* node = malloc(sizeof(struct pq_node));
	node->value = value;
	node->priority = priority;

	dynarray_insert(pq->arr, node);

	pq_percolate_up(pq, dynarray_size(pq->arr) - 1);
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {

	return pq_get_node(pq, 0)->value;
}


struct pq_node* pq_get_node(struct pq* pq, int index) {

	return (struct pq_node*)(dynarray_get(pq->arr, index));
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {

	return pq_get_node(pq, 0)->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	
	void* first_value_ptr = pq_first(pq);

	pq_node_swap(pq_get_node(pq, 0), pq_get_node(pq, dynarray_size(pq->arr) - 1));

	void* x = dynarray_get(pq->arr, dynarray_size(pq->arr) - 1);

	free(x);

	// Remove last element
	dynarray_remove(pq->arr, dynarray_size(pq->arr) - 1);

	if(dynarray_size(pq->arr) > 1)
		pq_percolate_down(pq);

	return first_value_ptr;
}
