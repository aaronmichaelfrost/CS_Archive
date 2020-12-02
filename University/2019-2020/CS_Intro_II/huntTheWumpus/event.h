#ifndef EVENT_H
#define EVENT_H


#include "event.h"
#include "player.h"
#include <string>

#include <iostream>
using namespace std;

class Room_Event {
public:
	virtual void encounter(Player&) = 0;
	virtual void percept(const Player& player) const = 0;
	virtual string debug_name() const = 0;
};



#endif // !EVENT_H