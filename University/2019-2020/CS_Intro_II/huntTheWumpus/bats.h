#ifndef  BATS_H
#define BATS_H

#include "event.h"


class Bats : public Room_Event {

public:


	void encounter(Player&);
	void percept(const Player& player) const;

	string debug_name() const;

};


#endif 
