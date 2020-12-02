/******************************************************
** Program: linked_list.cpp
** Author: Aaron Frost
** Date: 6/7/2020
** Description: Read attached README.txt
** Input: test_linked_list.cpp
** Output: list functionalities
******************************************************/








#include <iostream>
#include "linked_list.h"







Linked_List::Linked_List() {

	this->length = 0;

}


int Linked_List::get_length() {

	return this->length;

}


void Linked_List::print() {

	Node* current = this->head; 

	
	for (int i = 0; i < this->length; i++)
	{
		std::cout << current->val << " ";
		current = current->next;
	}

	std::cout << std::endl;
}


void Linked_List::clear(){

	Node* current = this->head;

	Node* temp = current->next;

	for (int i = 0; i < this->length; ++i)
	{
		delete current;
		current = NULL;
		current = temp;
		temp = temp->next;
	}

	this->length = 0;
}


void Linked_List::push_front(int val) {

	insert(val, 0);

}


void Linked_List::push_back(int val) {

	insert(val, this->length);

}


void Linked_List::insert(int val, unsigned int index) {


	if (index <= this->length) {

		Node* new_node = new Node();
		new_node->val = val;


		if (index == 0) {

			new_node->next = this->head;

			this->head = new_node;
		}
		else {

			Node* current = this->head;

			for (int i = 0; i < index - 1; ++i)
				current = current->next;

			Node* temp = current->next;

			current->next = new_node;

			new_node->next = temp;
		}


		this->length++;

	}
	else 
		std::cout << "Insertion index out of range. (Note: Expected from test input)" << std::endl;

}


void Linked_List::sort_ascending() {
	
	merge_sort(0, this->length - 1, true);

}


void Linked_List::sort_descending() {

	merge_sort(0, this->length - 1, false);

}


void Linked_List::merge(int l, int m, int r, bool ascending)
{

	if (l < r) {


		/* node before the first node on the left half */
		Node* before_l = this->head;
		for (int i = 0; i < l - 1; i++)
			before_l = before_l->next;

		/* node before the first node on the right half */
		Node* before_r = this->head;
		for (int i = 0; i < m; i++)
			before_r = before_r->next;


		/* first node on the left half */
		Node* l1 = this->head;

		for (int i = 0; i < l; i++)
			l1 = l1->next;


		/* first node on the right half */
		Node* r1 = this->head;

		for (int i = 0; i < m+1; i++)
			r1 = r1->next;


		/* start an indexer on the left half */
		Node* current_l = l1;

		/* start an indexer on the right half */
		Node* current_r = r1;


		int l_sorted = 0;
		int l_elements = m - l + 1;

		int r_sorted = 0;
		int r_elements = r - m;
	


		for (int i = 0; i < r-l; i++)
		{

			bool swap = ascending ? current_l->val > current_r->val : current_l->val < current_r->val;

			Node* after_right = current_r->next;

			Node* after_left = current_l->next;



			if (swap) {
				

				Node* temp_left = current_l;

				current_l = current_r;

				current_r = temp_left;

				current_l->next = after_left;

				current_r->next = after_right;

				if(l>0)
					before_l->next = current_l;

				before_r->next = current_r;
					
				r_sorted++;

				
				if (r_sorted < r_elements) {
					before_r = current_r;
					current_r = current_r->next;
				}
				
			}
			else {
				l_sorted++;
				if (l_sorted < l_elements) {
					before_l = current_l;
					current_l = current_l->next;
				}

			}
		}

	}
	
	
}


void Linked_List::merge_sort(int l, int r, bool ascending)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;

		merge_sort(l, m, ascending);
		merge_sort(m + 1, r, ascending);


		merge(l, m, r, ascending);

	}
}


unsigned int Linked_List::count_prime() {

	int x;

	Node* current = this->head;


	for (int i = 0; i < this->length; i++)
	{
		if (is_prime(current->val))
			x++;

		current = current->next;
	}


	return x;
}


bool is_prime(int n)
{

	if (n <= 1)
		return false;


	for (int i = 2; i < n; i++)
		if (n % i == 0)
			return false;

	return true;
}