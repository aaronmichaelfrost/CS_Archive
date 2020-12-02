#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>
using namespace std;


class Animal {
protected:
	int age; // months old
	int price;
	int food_cost;
	int revenue;


public:

	Animal();

	int get_age();
	int get_price();
	int get_food_cost();
	int get_revenue();

	void set_age(int);
	void set_price(int);
	void set_food_cost(int);
	void set_revenue(int);


	void increment_age();

};


#endif