#include "escape.h"


string Escape::debug_name() const {
	return "E";
}


void Escape::encounter(Player& player) {
	if (player.has_collected_gold && player.has_killed_wumpus) {

		cout << endl;
		cout << "Congratulations! You escaped with the gold and killed the wumpus." << endl;
		cout << endl;

		player.escaped = true;
	}
}


void Escape::percept(const Player& player) const {

}