#include "animal.h"


/*************************************************************************************************
FUNCTION NAME: increment_age
FUNCTION DESCRIPTION: this function increments the animals age by one
INPUTS: none
OUTPUTS: increments age by one
**************************************************************************************************/
void Animal::increment_age() {
	this->age++;
}



// ************ CONSTRUCTORS ************ //

Animal::Animal() {
	this->age = 0;
	this->price = 0;
	this->food_cost = 0;
	this->revenue = 0;
}


// ************ ACCESSORS ************ //

int Animal::get_age() {
	return this->age;
}


int Animal::get_price() {
	return this->price;
}


int Animal::get_food_cost() {
	return this->food_cost;
}


int Animal::get_revenue() {
	return this->revenue;
}



// ************ MUTATORS ************ //




void Animal::set_age(int age) {
	this->age = age;
}


void Animal::set_price(int price) {
	this->price = price;
}


void Animal::set_food_cost(int food_cost) {
	this->food_cost = food_cost;
}


void Animal::set_revenue(int revenue) {
	this->revenue = revenue;
}