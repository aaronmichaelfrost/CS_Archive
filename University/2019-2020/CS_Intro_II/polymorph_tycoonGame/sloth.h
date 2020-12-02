#ifndef SLOTH_H
#define SLOTH_H

#include <iostream>
using namespace std;

#include "animal.h"


class Sloth : public Animal {
public:
	Sloth();

	static const int cost = 2000;
	static const int revenue = 100;
	static const int birth_rate = 5;

};


#endif