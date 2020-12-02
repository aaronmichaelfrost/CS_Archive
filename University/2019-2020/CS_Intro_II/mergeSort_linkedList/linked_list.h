#ifndef LINKED_LIST
#define LINKED_LIST

#include "node.h"

class Linked_List {
private:
	unsigned int length;
	Node* head; 

public:


	Linked_List();


	int get_length();


	void print(); 
	void clear(); 

	void push_front(int); 
	void push_back(int); 
	void insert(int val, unsigned int index); 

	void sort_ascending(); 

	void sort_descending(); 

	unsigned int count_prime(); 






	// you can add extra member variables or functions as desired


	void merge_sort(int l, int r, bool ascending);
	void merge(int l, int m, int r, bool ascending);

};




// Returns true if an integer is prime
bool is_prime(int n);


#endif