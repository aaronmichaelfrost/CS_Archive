#ifndef MONKEY_H
#define MONKEY_H

#include <iostream>
using namespace std;

#include "animal.h"


class Monkey : public Animal {
public:
	Monkey();

	static const int cost = 15000;
	static const int revenue = 1500;
	static const int birth_rate = 1;

};





#endif