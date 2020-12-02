#include "wumpus.h"


string Wumpus::debug_name() const {
	return "W";
}


void Wumpus::percept(const Player& player) const {
	if (!player.has_killed_wumpus) {

		cout << "\nYou smell a terrible stench." << endl;
	}
	else {
		cout << "\nYou inhale the rotting corpse's stench." << endl;
	}
}


void Wumpus::encounter(Player& player) {

	if (!player.has_killed_wumpus) {
		cout << "\n\nYou were killed by wumpus." << endl;
		cout << endl;

		player.killed = true;
	}
	else {
		cout << "\n\nYou stand over the body..." << endl;
	}
}