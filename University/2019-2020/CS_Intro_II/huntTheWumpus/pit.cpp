
#include "pit.h"

string Pit::debug_name() const {
	return "P";
}


void Pit::percept(const Player& player) const {

	cout << "\nYou feel a breeze." << endl;

}


void Pit::encounter(Player& player) {
	cout << endl;
	cout << "You fell into an endless abyss and died." << endl;
	cout << endl;

	player.killed = true;
}