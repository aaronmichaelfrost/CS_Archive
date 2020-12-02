#ifndef  GOLD_H
#define GOLD_H

#include "event.h"


class Gold : public Room_Event {

public:


	void encounter(Player&);
	void percept(const Player& player) const;

	string debug_name() const;

};


#endif 