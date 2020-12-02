/*********************************************************************
** Program Filename: driver.cpp
** Author: Aaron Frost
** Date: 5/24/2020
** Description: Hunt the wumpus main driver
** Input: none
** Output: begins main game loop found in game.cpp
*********************************************************************/






#include "game.h"




int main(int argc, char** argv) {
	Game g;

	g.set_up_game(argc, argv);

	g.main_loop();


}