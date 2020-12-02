#ifndef  ESCAPE_H
#define ESCAPE_H

#include "event.h"


class Escape : public Room_Event {
public:


	void encounter(Player&);
	void percept(const Player& player) const;
	string debug_name() const;


};


#endif 
