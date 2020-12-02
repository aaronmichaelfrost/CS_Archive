#ifndef  PIT_H
#define PIT_H

#include "event.h"


class Pit : public Room_Event {

public:


	void encounter(Player&);
	void percept(const Player& player) const;

	string debug_name() const;

};


#endif 