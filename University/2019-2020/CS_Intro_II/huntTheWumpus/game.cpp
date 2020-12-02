/*********************************************************************
** Program Filename: game.cpp
** Author: Aaron Frost
** Date: 5/24/2020
** Description: Contains the main game function definitions
** Input: none
** Output: all game outputs
*********************************************************************/









#include "game.h"
#include <stdlib.h> // for atoi
#include <time.h> // for psuedo-random number generation
#include <cmath>








void Game::main_loop() {

	percepts();

	print_grid();

	make_action(); // Move or shoot

	encounter_room(); // This just changes is player bools


	if (this->player.killed || this->player.escaped) {

		if (get_yes_no_input("Play Again?"))
			play_again();
		else
			return;

	}
	else {
		main_loop();
	}

}


void Game::print_grid() const {
	cout << "\n-----------------\n\n";

	if(this->debug_mode)
		cout << "Game Board (" << this->size << "X" << this->size << ")" << endl;

	for (int r = 0; r < this->size; r++)
	{

		for (int c = 0; c < this->size; c++)
		{
			// Print debug character if event isnt null

			if (this->player.row == r && this->player.col == c)
				cout << "|*";
			else {

				if (this->debug_mode == true && this->grid[r][c].room_event != NULL)
					cout << "|" << this->grid[r][c].room_event->debug_name();
				else
					cout << "| ";
			}
		}


		cout << "|" << endl;
	}

	cout << "\n-----------------\n\n";
}


void Game::percepts() const {

	cout << "\n\n\n\n";


	vector< vector<Room> > g = this->grid;

	int p_r = this->player.row;
	int p_c = this->player.col;
	int s = this->size;


	for (int r = 0; r < s; r++)
	{
		for (int c = 0; c < s; c++)
		{
			if ((abs(p_r - r) == 1 && p_c == c) || (abs(p_c - c) == 1) && p_r == r) 
				if (this->grid[r][c].room_event != NULL)
					this->grid[r][c].room_event->percept(this->player);
		}

	}

}


void Game::play_again() {


	if (get_yes_no_input("Use new cave?")) {
		this->grid.clear();
		create_grid();
		save_grid();
	}
	else
		load_grid();

	spawn_player();
	
	main_loop();

}










/// ACTIONS ///



void Game::make_action() {

	string c = get_string_input("\nMove(w) (a) (s) (d) , or fire ( w) ( a) ( s) ( d) : ");

	// If input is invalid, try again
	if (to_lower(c) != " w" && to_lower(c) != " d" && to_lower(c) != " s"
		&& to_lower(c) != " a" && to_lower(c) != "w" && to_lower(c) != "d"
		&& to_lower(c) != "s" && to_lower(c) != "a") {
		make_action();
	}
	else {
		if (c.length() == 1)
			move(c);
		else
			fire(c);
	}
}



// MOVING


void Game::move(string dir) {


	if (dir == "w") {
		if (this->player.row == 0)
			out_of_bounds();
		else
			this->player.row--;

	}
	else if (dir == "a") {
		if (this->player.col == 0)
			out_of_bounds();
		else
			this->player.col--;

	}
	else if (dir == "s") {
		if (this->player.row == this->size - 1)
			out_of_bounds();
		else
			this->player.row++;

	}
	else if (dir == "d") {
		if (this->player.col == this->size - 1)
			out_of_bounds();
		else
			this->player.col++;
	}
}


void Game::out_of_bounds() {
	cout << "We've reached the cave wall, lets try a different direction." << endl;
	make_action();
}


void Game::encounter_room() {
	if (this->grid[this->player.row][this->player.col].room_event != NULL)
		this->grid[this->player.row][this->player.col].room_event->encounter(this->player);

	if (this->player.bat_swarmed)
		bat_swarm();
}


void Game::bat_swarm() {
	vector<int> c = random_cords();

	this->player.row = c[0];
	this->player.col = c[1];

	this->player.bat_swarmed = false;

	encounter_room();
}


vector<int> Game::random_cords() const {
	vector<int> cords;

	int row, col;

	row = rand() % this->size;
	col = rand() % this->size;

	cords.push_back(row);
	cords.push_back(col);

	return cords;
}


void Game::set_player_position(int row, int col) {
	this->player.row = row;
	this->player.col = col;
}







// FIRING ARROWS


bool Game::is_wumpus(int row, int col) const {
	return this->grid[row][col].room_event != NULL 
		&& to_lower(this->grid[row][col].room_event->debug_name()) == "w";
}


void Game::kill_wumpus() {
	this->player.has_killed_wumpus = true;
	cout << "\nYou hear the excruciating reverberated scream of the wumpus as your arrow pierces through its heart." << endl;
}


void Game::hit_wall(bool& has_hit) {
	has_hit = true;
	cout << "Arrow hit wall" << endl;
}


void Game::arrow_travel(int x, int y) {
	bool arrow_hit_wall = false;

	int rooms_travelled = 0;

	if(this->debug_mode)
		cout << "\nDEBUGGING Arrow Coordinates:" << endl;

	while (++rooms_travelled < 4 && !arrow_hit_wall)
	{
		int current_row = this->player.row - (rooms_travelled * y);
		int current_col = this->player.col + (rooms_travelled * x);

		if(this->debug_mode == true)
			cout << "\nx " << current_col << " y " << current_row << endl;

		if (current_col == -1 || current_col == this->size 
			|| current_row == -1 || current_row == this->size) {
			hit_wall(arrow_hit_wall);
		}
		else {
			if (!this->player.has_killed_wumpus && is_wumpus(current_row, current_col)) {
				kill_wumpus();
				break;
			}
		}
	}
}


void Game::fire_north() {

	if (this->player.row == 0)
		out_of_bounds();
	else {
		cout << "Firing North" << endl;

		arrow_travel(0, 1);

		this->player.arrows--;



		if (!this->player.has_killed_wumpus)
			arrow_miss();

	}
}


void Game::fire_west() {
	

	if (this->player.col == 0)
		out_of_bounds();
	else {
		cout << "Firing West" << endl;

		arrow_travel(-1, 0);

		this->player.arrows--;



		if (!this->player.has_killed_wumpus)
			arrow_miss();

	}
}


void Game::fire_south() {
	if (this->player.row == this->size - 1)
		out_of_bounds();
	else {
		cout << "Firing South" << endl;

		arrow_travel(0, -1);

		this->player.arrows--;



		if (!this->player.has_killed_wumpus)
			arrow_miss();

	}
}


void Game::fire_east() {
	if (this->player.col == this->size - 1)
		out_of_bounds();
	else {
		cout << "Firing East" << endl;

		arrow_travel(1, 0);

		this->player.arrows--;



		if (!this->player.has_killed_wumpus)
			arrow_miss();

	}
}


void Game::fire(string dir) {
	
	if (player.arrows > 0) {

		if (dir == " w") 
			fire_north();	

		else if (dir == " a") 
			fire_west();

		else if (dir == " s") 
			fire_south();
		
		else if (dir == " d") 
			fire_east();
		

	}
	else 
		out_of_arrows();
	
}


void Game::arrow_miss() {


	int x = rand() % 100 + 1;

	if (this->debug_mode)
		cout << "\nDEBUGGING Wumpus move chance:\nIf x <= 75, wumpus will move to empty room. x: " << x << endl;

	if (x <= 75) {

		move_wumpus(random_empty_cords());
	}
	
		
}


void Game::move_wumpus(vector<int> cords) {
	


	// Set old room event to null
	for (int r = 0; r < this->size; r++)
	{
		for (int c = 0; c < this->size; c++)
		{

			if (this->grid[r][c].room_event != NULL)
				if (this->grid[r][c].room_event->debug_name() == "W")
					this->grid[r][c].room_event = NULL;
		}
	}

	// Place wumpus in new location
	this->grid[cords[0]][cords[1]].room_event = this->wumpus;
}


void Game::out_of_arrows() {
	cout << "We've ran out of arrows, all we can do is move..." << endl;
	make_action();
}










/// GAME SET UP ///


void Game::spawn_player() {
	reset_player_states();

	// Move player back to spawn
	for (int r = 0; r < this->size; r++)
	{

		for (int c = 0; c < this->size; c++)
		{

			if (this->grid[r][c].room_event != NULL)
				if (this->grid[r][c].room_event->debug_name() == "E")
					set_player_position(r, c);
		}
	}
}


void Game::set_up_game(int argc, char** argv) {

	/* initialize random seed: */
	srand(time(NULL));



	


	if (argc != 3 || !size_valid(argv[1]) || !debug_mode_valid(argv[2])) 
		retry_arguments();
	else {

		this->size = atoi(argv[1]);
		this->debug_mode = string_to_bool(argv[2]);
	}


	cout << "\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n~~~~~~~   HUNT THE WUMPUS   ~~~~~~~\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n";

	if (this->debug_mode)
		cout << "\nDEBUG MODE ENABLED (Wumpus: W, Bats: B, Pit: P, Gold: G, Escape Rope: E)\n";



	create_grid();
	save_grid();

	spawn_player();
}


void Game::reset_player_states() {
	this->player.killed = false;
	this->player.escaped = false;
	this->player.has_killed_wumpus = false;
	this->player.has_collected_gold = false;
	this->player.bat_swarmed = false;
	this->player.arrows = ARROWS;
}


void Game::load_grid() {

	// Set events rooms back to original state
	this->grid = this->saved_grid;

}


void Game::save_grid() {

	this->saved_grid.clear();

	for (int r = 0; r < this->size; r++)
	{
		vector<Room> row_vec;

		for (int c = 0; c < this->size; c++)
		{
			row_vec.push_back(this->grid[r][c]);
		}

		this->saved_grid.push_back(row_vec);
	}
}


void Game::retry_arguments() {
	cout << endl;
	cout << "You did not enter the expected arguments. Please enter below." << endl;
	cout << endl;

	cout << "Max size: " << MAX_SIZE << endl;
	cout << "Min size: " << MIN_SIZE << endl;
	cout << endl;

	int size = get_int_in_range("Size: ", MIN_SIZE, MAX_SIZE);

	this->size = size;

	this->debug_mode = get_yes_no_input("Debug(y/n): ");
}


bool Game::size_valid(char* arg) const {
	return is_int(arg) && atoi(arg) >= MIN_SIZE && atoi(arg) <= MAX_SIZE;
}


bool Game::debug_mode_valid(char* arg) const {

	string s_arg = arg;

	return s_arg == "true" || s_arg == "false";
}


bool Game::string_to_bool(char* arg) const {

	string s_arg = arg;

	return s_arg == "true";

}


void Game::create_grid() {

	// Initialize the board
	for (int r = 0; r < this->size; r++)
	{
		vector<Room> row_vec;

		for (int c = 0; c < this->size; c++)
		{
			Room new_room;

			row_vec.push_back(new_room);
		}

		this->grid.push_back(row_vec);
	}

	place_events();
}


void Game::place_events() {

	place_event_randomly('W', 1);
	place_event_randomly('E', 1);
	place_event_randomly('G', 1);

	place_event_randomly('B', 2);
	place_event_randomly('P', 2);

}


void Game::place_event_randomly(char event_type, int quantity) {

	for (int i = 0; i < quantity; i++)
	{
		vector<int> cords = random_empty_cords();

		int row = cords[0];
		int col = cords[1];

		// PLACE

		switch (event_type)
		{

		case 'W':

			this->grid[row][col].room_event = this->wumpus;

			break;

		case 'E':

			this->grid[row][col].room_event = this->escape;

			break;

		case 'G':

			this->grid[row][col].room_event = this->gold;

			break;

		case 'B':

			this->grid[row][col].room_event = this->bats;

			break;

		case 'P':

			this->grid[row][col].room_event = this->pit;

			break;

		default:
			break;
		}
	}

}


vector<int> Game::random_empty_cords() const {

	vector<int> cords;


	// generate a new row and column while the current row/column are occupied

	int row, col;

	do
	{
		row = rand() % this->size;
		col = rand() % this->size;

	} while (this->grid[row][col].room_event != NULL);


	cords.push_back(row);
	cords.push_back(col);

	return cords;

}




/// CONSTRUCTOR & DECONSTRUCTOR ///


Game::Game() {

	this->size = 0;

	Player new_player;
	this->player = new_player;

	this->debug_mode = false;

	this->wumpus = new Wumpus();
	this->gold = new Gold();
	this->escape = new Escape();

	this->bats = new Bats();
	this->pit = new Pit();

	reset_player_states();
}


Game::~Game() {
	delete this->wumpus;
	delete this->gold;
	delete this->escape;

	delete this->bats;
	delete this->pit;
}




/// MUTATORS AND ACCESSORS ///


Player Game::get_player() const {
	return this->player;
}


void Game::set_player(Player p) {
	this->player = p;
}


int Game::get_size() const {
	return this->size;
}


void Game::set_size(int size) {
	this->size = size;
}


bool Game::get_debug_mode() const {
	return this->debug_mode;
}


void Game::set_debug_mode(bool debug_mode) {
	this->debug_mode = debug_mode;
}