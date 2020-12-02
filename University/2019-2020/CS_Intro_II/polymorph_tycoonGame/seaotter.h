#ifndef SEAOTTER_H
#define SEAOTTER_H

#include <iostream>
using namespace std;

#include "animal.h"


class SeaOtter : public Animal {
public:
	SeaOtter();

	static const int cost = 5000;
	static const int revenue = 250;
	static const int birth_rate = 2;

};




#endif