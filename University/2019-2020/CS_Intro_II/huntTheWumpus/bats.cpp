#include "bats.h"


string Bats::debug_name() const {
	return "B";
}


void Bats::percept(const Player& player) const {

	cout << "\nYou hear wings flapping." << endl;
}


void Bats::encounter(Player& player) {

	cout << "\nYou were taken to a random room by a swarm of bats." << endl;
	cout << endl;


	player.bat_swarmed = true;

}
