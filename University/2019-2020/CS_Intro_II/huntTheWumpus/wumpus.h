#ifndef  WUMPUS_H
#define WUMPUS_H

#include "event.h"


class Wumpus : public Room_Event {

public:


	void encounter(Player&);
	void percept(const Player& player) const;

	string debug_name() const;

};


#endif 