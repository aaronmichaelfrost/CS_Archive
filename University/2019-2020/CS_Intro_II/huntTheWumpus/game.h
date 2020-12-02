#ifndef GAME_H
#define GAME_H


#define MIN_SIZE 4
#define MAX_SIZE 20
#define ARROWS 3


#include "player.h"
#include "room.h"

#include "bats.h"
#include "wumpus.h"
#include "pit.h"
#include "gold.h"
#include "escape.h"



#include <iostream>

#include "utilities.h"
#include <vector>
using std::vector;
using namespace std;


class Game {
private:
	Player player;
	int size;
	vector< vector<Room> > grid;
	vector< vector<Room> > saved_grid;
	bool debug_mode;


	// Since polymorphism/dynamic binding requires the use of pointers
	Wumpus* wumpus;
	Gold* gold;
	Escape* escape;
	Bats* bats;
	Pit* pit;
	

public:
	Game();

	~Game();



	// Mutators/accessors
	Player get_player() const;
	void set_player(Player);

	int get_size() const;
	void set_size(int);

	bool get_debug_mode() const;
	void set_debug_mode(bool);
	


	// Set up //


	/*********************************************************************
	** Function: retry_arguments()
	** Description: prompts for size and debug bool
	** Parameters: none
	** Pre-Conditions: main args were bad
	** Post-Conditions: fixes arguments
	*********************************************************************/
	void retry_arguments();


	/*********************************************************************
	** Function: size_valid(char*) 
	** Description: checks if the input character is in range for the grid
	** Parameters: char* the argument
	** Pre-Conditions: takes an argument
	** Post-Conditions: returns true if size is in range
	*********************************************************************/
	bool size_valid(char*) const;


	/*********************************************************************
	** Function: debug_mode_valid(char*)
	** Description: checks if debug mode is acceptable
	** Parameters: char* the argument
	** Pre-Conditions: takes an argument
	** Post-Conditions: returns true if the argument is either "true" or "false"
	*********************************************************************/
	bool debug_mode_valid(char*) const;


	/*********************************************************************
	** Function: string_to_bool(char*)
	** Description: checks whether a string == "true"
	** Parameters: char* the argument
	** Pre-Conditions: takes an argument
	** Post-Conditions: returns true if the argument == "true"
	*********************************************************************/
	bool string_to_bool(char*) const;


	/*********************************************************************
	** Function: create_grid()
	** Description: sets up a 2d vector of rooms
	** Parameters: none
	** Pre-Conditions: size set
	** Post-Conditions: creates this->grid
	*********************************************************************/
	void create_grid();


	/*********************************************************************
	** Function: save_grid()
	** Description: sets this->original_grid to current this->grid
	** Parameters: none
	** Pre-Conditions: this->grid exists
	** Post-Conditions: updates this->original grid
	*********************************************************************/
	void save_grid();


	/*********************************************************************
	** Function: load_grid()
	** Description: sets this->grid to this->original grid 
	** Parameters:
	** Pre-Conditions:
	** Post-Conditions:
	*********************************************************************/
	void load_grid();


	/*********************************************************************
	** Function: spawn_player()
	** Description: reset player states and move player to escape room
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: player has reset states and is in spawn point
	*********************************************************************/
	void spawn_player();


	/*********************************************************************
	** Function: place_events()
	** Description: places all events randomly
	** Parameters: none
	** Pre-Conditions: grid exists
	** Post-Conditions: fills some random grid rooms with events
	*********************************************************************/
	void place_events();


	/*********************************************************************
	** Function: place_event_randomly(char event_type, int quantity)
	** Description: places a number of events throughout the grid
	** Parameters: event_type, a character used to distinguish what event to place, 
	**				quantity: determines the amount to place
	** Pre-Conditions: grid exists and there is empty room
	** Post-Conditions: grid now contains a number of new events
	*********************************************************************/
	void place_event_randomly(char event_type, int quantity);


	/*********************************************************************
	** Function: random_empty_cords()
	** Description: returns a 2d vector of random coordinates existing 
	**			on this->grid in which the room's event == NULL
	** Parameters: none
	** Pre-Conditions: grid exists
	** Post-Conditions: returs random empty room coordinates on grid
	*********************************************************************/
	vector<int> random_empty_cords() const;



	/*********************************************************************
	** Function: set_up_game(int argc, char** argv)
	** Description: verifies arguments, sets up grid
	** Parameters: argc, argv
	** Pre-Conditions: none
	** Post-Conditions: main_loop() eventually called
	*********************************************************************/
	void set_up_game(int argc, char** argv);
	











	// Gameplay //


	/*********************************************************************
	** Function: random_cords()
	** Description: generates a random 2d vector that exists as coordinates 
	**				on this->grid
	** Parameters: none
	** Pre-Conditions: grid exists
	** Post-Conditions: returns coordinates
	*********************************************************************/
	vector<int> random_cords() const;


	/*********************************************************************
	** Function: print_grid()
	** Description: prints the grid debugging mode and normal mode
	** Parameters: none
	** Pre-Conditions: grid exists
	** Post-Conditions: outputs grid to console
	*********************************************************************/
	void print_grid() const;


	/*********************************************************************
	** Function: set_player_position(int row, int col)
	** Description: assigns new coordinates to this->player
	** Parameters: row, col from the current grid
	** Pre-Conditions: grid exists, this->player != NULL
	** Post-Conditions: updates this->player
	*********************************************************************/
	void set_player_position(int row, int col);


	/*********************************************************************
	** Function: percepts()
	** Description: calls the precept functions in adjacent rooms
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions:calls the precept functions in adjacent rooms
	*********************************************************************/
	void percepts() const;


	/*********************************************************************
	** Function: make_action()
	** Description: makes player either walk or shoot
	** Parameters: none
	** Pre-Conditions: player is spawned and grid is created
	** Post-Conditions: player moves or fires
	*********************************************************************/
	void make_action();


	/*********************************************************************
	** Function: out_of_bounds()
	** Description: called when player tries to move outside of the cave
	** Parameters: none
	** Pre-Conditions: player tried to move against the cave wall
	** Post-Conditions: make_action()
	*********************************************************************/
	void out_of_bounds();


	/*********************************************************************
	** Function: out_of_arrows()
	** Description: called when player tries to shoot when out of arrows
	** Parameters: none
	** Pre-Conditions: player tried to shoot when out of arrows
	** Post-Conditions: make_action()
	*********************************************************************/
	void out_of_arrows();


	/*********************************************************************
	** Function: fire_north()
	** Description: arrow is attempted fired
	** Parameters: none
	** Pre-Conditions: player has arrows
	** Post-Conditions: wumpus dead or wumpus moves or nothing
	*********************************************************************/
	void fire_north();


	/*********************************************************************
	** Function: fire_west()
	** Description: arrow is attempted fired
	** Parameters: none
	** Pre-Conditions: player has arrows
	** Post-Conditions: wumpus dead or wumpus moves or nothing
	*********************************************************************/
	void fire_west();


	/*********************************************************************
	** Function: fire_south()
	** Description: arrow is attempted fired
	** Parameters: none
	** Pre-Conditions: player has arrows
	** Post-Conditions: wumpus dead or wumpus moves or nothing
	*********************************************************************/
	void fire_south();


	/*********************************************************************
	** Function: fire_east()
	** Description: arrow is attempted fired
	** Parameters: none
	** Pre-Conditions: player has arrows
	** Post-Conditions: wumpus dead or wumpus moves or nothing
	*********************************************************************/
	void fire_east();


	/*********************************************************************
	** Function: fire(string dir)
	** Description: if we have arrows, process direction, then call fire
	** Parameters: dir distinguishes direction: " w", " a", " s", or " d"
	** Pre-Conditions: player is on grid
	** Post-Conditions: arrow potentially fired
	*********************************************************************/
	void fire(string dir);


	/*********************************************************************
	** Function: move(string dir)
	** Description: player's position is adjusted if direction is not out of bounds
	** Parameters: dir distinguishes direction "w", "a", "s", or "d"
	** Pre-Conditions: player is on grid
	** Post-Conditions: player states may change
	*********************************************************************/
	void move(string dir);



	/*********************************************************************
	** Function: bat_swarm()
	** Description: moves player to random room, then calls encounter
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: randomizes this->player.row and this->player.col 
	**					+ encounter() is called
	*********************************************************************/
	void bat_swarm();


	/*********************************************************************
	** Function: encounter_room()
	** Description: called the encounter function of the player's current 
	**				location room
	** Parameters: none
	** Pre-Conditions: player location exists on grid
	** Post-Conditions: may move player and change player states
	*********************************************************************/
	void encounter_room();


	/*********************************************************************
	** Function: reset_player_states()
	** Description: changes this->players states back to default/start values
	** Parameters: none
	** Pre-Conditions: this->player isnt null
	** Post-Conditions: this->player values defaulted
	*********************************************************************/
	void reset_player_states();



	/*********************************************************************
	** Function: is_wumpus(int row, int col)
	** Description: checks if this->grid[row][col] has a wumpus event
	** Parameters: the coords to check
	** Pre-Conditions: none
	** Post-Conditions: returns true if wumpus is in the specified room
	*********************************************************************/
	bool is_wumpus(int row, int col) const;


	/*********************************************************************
	** Function: kill_wumpus()
	** Description: sets this->player.has_killed_wumpus to true
	** Parameters: none
	** Pre-Conditions: player hit wumpus with arrow
	** Post-Conditions: this->player.has_killed_wumpus == true
	*********************************************************************/
	void kill_wumpus();


	/*********************************************************************
	** Function: hit_wall(bool&)
	** Description: changes referenced bool to true, and prints to console
	** Parameters: referenced bool to update the state of the arrow
	** Pre-Conditions: arrow is out of range
	** Post-Conditions: changes referenced bool to true, and prints to console
	*********************************************************************/
	void hit_wall(bool&);


	/*********************************************************************
	** Function: arrow_travel(int x, int y)
	** Description: sends an arrow in the specified direction for 3 rooms
	**				until it hits a wall or kills the wumpus
	** Parameters:  x: set this to 1 to travel right, -1 for left
	**				y: set this to 1 to travel up, -1 for down
	** Pre-Conditions: arrow has been prompted to be shot
	** Post-Conditions: kill_wumpus() or hit_wall() may be called
 	*********************************************************************/
	void arrow_travel(int x, int y);


	/*********************************************************************
	** Function: arrow_miss()
	** Description: called when an arrow shot didnt hit wumpus
	** Parameters: none
	** Pre-Conditions: arrow shot, did not kill wumpus
	** Post-Conditions: move_wumpus() called
	*********************************************************************/
	void arrow_miss();


	/*********************************************************************
	** Function: move_wumpus(vector<int> cords)
	** Description: 75% chance to move the wumpus to a random empty room
	** Parameters: randomly generated empty coordinates on grid
	** Pre-Conditions: arrow_miss() called, arrow has been missed, random coords generated
	** Post-Conditions: 75% chance to move the wumpus to a random empty room
	*********************************************************************/
	void move_wumpus(vector<int> cords);


	/*********************************************************************
	** Function: main_loop()
	** Description: the main looping part of the game
	** Parameters: none
	** Pre-Conditions: must be called from set_up_game()
	** Post-Conditions: none
	*********************************************************************/
	void main_loop();


	/*********************************************************************
	** Function: play_again()
	** Description: user prompted to play again with the same or different grid
	** Parameters: none
	** Pre-Conditions: main_loop() has been ran at least once
	** Post-Conditions: new cave generated, or old cave reloaded and player reset
	*********************************************************************/
	void play_again();

};

#endif // !GAME_H
