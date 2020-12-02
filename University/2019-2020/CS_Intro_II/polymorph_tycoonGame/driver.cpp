/*************************************************************************************************
PROGRAM NAME: Zoo Tycoon Game / Assignment 3
PROGRAM DESCRIPTION: Zoo Tycoon game that progresses by the month
AUTHOR: Aaron Frost
DATE: 5/10/2020
INPUTS: Player choices during runtime
OUTPUTS: Prompts to screen
**************************************************************************************************/



#include "zoo.h"

#include <iostream>
using namespace std;



/*************************************************************************************************
FUNCTION NAME: main
FUNCTION DESCRIPTION: creates a zoo, and initiates the game
INPUTS: none
OUTPUTS: console prompts
**************************************************************************************************/
int main() {

	Zoo z;
	z.set_bank(50000);
	z.set_base_food_cost(100);
	z.main_loop();
	cout << "Game Ended with $" << z.get_bank() << " remaining!\n\nWith " << z.get_num_monkeys() << " monkeys,\n" << z.get_num_seaotters() << " seaotters,\n" << z.get_num_sloths() << " sloths.\n\n";
	
	
	return 0;
}