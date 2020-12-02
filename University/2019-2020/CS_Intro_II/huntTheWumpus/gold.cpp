#include "gold.h"



string Gold::debug_name() const {
	return "G";
}


void Gold::percept(const Player& player) const {
	if (!player.has_collected_gold) {

		cout << "\nYou see a glimmer nearby." << endl;
	}
}


void Gold::encounter(Player& player) {

	if (!player.has_collected_gold) {

		cout << "\nSuccess! You gathered the cave's treasures." << endl;

	}

	player.has_collected_gold = true;
}