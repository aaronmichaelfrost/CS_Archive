#include <iostream>
using namespace std;

#include "zoo.h"

#include "tools.h"
#include <stdlib.h>
#include <time.h>




/*************************************************************************************************
FUNCTION NAME: main_loop
FUNCTION DESCRIPTION: the main month by month loop for the zoo
INPUTS: none
OUTPUTS: continues until bankrupt
**************************************************************************************************/
void Zoo::main_loop() {


	if (this->month > 1) {
		if (!try_start_new_month())
			return;
	}
	else {
		cout << "\nWelcome to Zoo Tycoon!\n";

		/* initialize random seed: */
		srand(time(NULL));
	}


	if (!try_special_event())
		return;


	display_monthly_info();


	if (!try_buy_animals())
		return;


	display_all_animals();

	
	sell_animal();
	

	if (!try_feeding())
		return;


	this->month++;
	main_loop();
	

}

/*************************************************************************************************
FUNCTION NAME: try_feeding
FUNCTION DESCRIPTION: this function preforms feeding payments
INPUTS: none
OUTPUTS: returns true if you could pay for feeding costs
**************************************************************************************************/
bool Zoo::try_feeding() {

	
	cout << "\nThis month's base food cost: $" << this->base_food_cost << endl;

	int total_food_price = this->base_food_cost * num_animals();

	cout << "\nTOTAL DEDUCTED FOR FOOD: -$" << total_food_price << endl;

	this->bank -= total_food_price;

	if (bank < 0)
		return false;

	/* generate random number between 75 and 125: */
	float change = rand() % 50 + 75;

	this->base_food_cost *= change / 100;

	return true;
}

/*************************************************************************************************
FUNCTION NAME: display_all_animals
FUNCTION DESCRIPTION: displays a numbered list of all animals and some details
INPUTS: none
OUTPUTS: prints list to console
**************************************************************************************************/
void Zoo::display_all_animals() {
	int j = 1;

	for (int i = 0; i < this->num_monkeys; i++)
	{
		cout << "\n(" << j++ << ") Monkey, " << this->monkeys[i].get_age() << " months, for " << Monkey::cost/2 << endl;
	}
	for (int i = 0; i < this->num_seaotters; i++)
	{
		cout << "\n(" << j++ << ") Seaotter, " << this->seaotters[i].get_age() << " months, for " << SeaOtter::cost / 2 << endl;
	}
	for (int i = 0; i < this->num_sloths; i++)
	{
		cout << "\n(" << j++ << ") Sloth, " << this->sloths[i].get_age() << " months, for " << Sloth::cost / 2 << endl;
	}
}

/*************************************************************************************************
FUNCTION NAME: sell_animal
FUNCTION DESCRIPTION: prompt to sell an animal
INPUTS: none
OUTPUTS: may sell an animal
**************************************************************************************************/
void Zoo::sell_animal() {

	if (has_animals()) {

		bool sell = get_yes_no_input("SELL ANIMAL?");


		if (sell) {
			int animal_index = get_natural_input("\nWhich animal?: ");

			if (animal_index > 0 && animal_index < num_animals() + 1) {


				if (animal_index <= this->num_monkeys)
					sell_monkey(animal_index - 1);
				else if (animal_index <= this->num_monkeys + this->num_seaotters)
					sell_seaotter(animal_index - this->num_monkeys - 1);
				else
					sell_sloth(animal_index - this->num_monkeys - this->num_seaotters - 1);
			}
			else {
				cout << "Animal choice out of range." << endl;
				sell_animal();
			}
		}
	}
}

/*************************************************************************************************
FUNCTION NAME: sell_monkey
FUNCTION DESCRIPTION: removes monkey and adds profits
INPUTS: the index of the monkey we want to sell
OUTPUTS: removes monkey and adds profits
**************************************************************************************************/
void Zoo::sell_monkey(int index) {
	cout << "Selling monkey index: " << index << endl;
	monkey_death(index);

	this->bank += Monkey::cost / 2;
	cout << "\nSold 1 monkey for +$" << Monkey::cost / 2 << endl;
}

/*************************************************************************************************
FUNCTION NAME: sell_seaotter
FUNCTION DESCRIPTION: removes seaotter and adds profits
INPUTS: the index of the seaotter we want to sell
OUTPUTS: removes seaotter and adds profits
**************************************************************************************************/
void Zoo::sell_seaotter(int index) {
	cout << "Selling seaotter index: " << index << endl;
	seaotter_death(index);

	this->bank += SeaOtter::cost / 2;
	cout << "\nSold 1 seaotter for +$" << SeaOtter::cost / 2 << endl;
}

/*************************************************************************************************
FUNCTION NAME: sell_sloth
FUNCTION DESCRIPTION: removes sloth and adds profits
INPUTS: the index of the sloth we want to sell
OUTPUTS: removes sloth and adds profits
**************************************************************************************************/
void Zoo::sell_sloth(int index) {
	cout << "Selling sloth index: " << index << endl;
	sloth_death(index);

	this->bank += Sloth::cost / 2;
	cout << "\nSold 1 sloth for +$" << Sloth::cost / 2 << endl;
}

/*************************************************************************************************
FUNCTION NAME: try_start_new_month
FUNCTION DESCRIPTION: if user wants to keep playing, increment basic info
INPUTS: none
OUTPUTS: returns true if player wants to play
**************************************************************************************************/
bool Zoo::try_start_new_month() {

	// Continue playing?
	if (done_playing())
		return false;

	increment_zoo_info();

	display_month();

	return true;
}

/*************************************************************************************************
FUNCTION NAME: display_month
FUNCTION DESCRIPTION: displays the current month
INPUTS: none
OUTPUTS: prints month to console
**************************************************************************************************/
void Zoo::display_month() {
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~\n~~~~~~~~~~~~~~~~~~~~~~~~~~~\n        MONTH: " << this->month << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~\n~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

/*************************************************************************************************
FUNCTION NAME: increment_zoo_info
FUNCTION DESCRIPTION: increments ages and collects revenue
INPUTS: none
OUTPUTS: increments ages and collects revenue
**************************************************************************************************/
void Zoo::increment_zoo_info() {
	increment_ages();
	collect_revenue();
}

/*************************************************************************************************
FUNCTION NAME: display_monthly_info
FUNCTION DESCRIPTION: displays the bank and animals
INPUTS: none
OUTPUTS: prints bank and animals to console
**************************************************************************************************/
void Zoo::display_monthly_info() {
	display_bank();
	display_num_animals();
}

/*************************************************************************************************
FUNCTION NAME: done_playing
FUNCTION DESCRIPTION: returns false if the user wants to keep playing with 2 step verification
INPUTS: none
OUTPUTS: returns true if user is done playing
**************************************************************************************************/
bool Zoo::done_playing() {
	if (!get_yes_no_input("Continue?"))
		if (get_yes_no_input("Quit game?"))
			return true;
	return false;
}

/*************************************************************************************************
FUNCTION NAME: try_special_event
FUNCTION DESCRIPTION: returns true if user could pay for special event
INPUTS: none
OUTPUTS: runs special event, returns if we went bankrupt
**************************************************************************************************/
bool Zoo::try_special_event() {
	if (has_animals()) {


		/* generate random number between 1 and 4: */
		int event_choice = rand() % 4 + 1;

		switch (event_choice)
		{
		case 1:
			sick_event();
			break;
		case 2:
			if (num_adults() > 0)
				birth_event();
			else
				return try_special_event();

			break;
		case 3:
			attendance_event();
			break;
		default:
			cout << "Normal month." << endl;
			break;
		}
	}

	return !bankrupt();
}

/*************************************************************************************************
FUNCTION NAME: has_animals
FUNCTION DESCRIPTION: check if there are any animals in this zoo
INPUTS: none
OUTPUTS: returns true if animals > 0
**************************************************************************************************/
bool Zoo::has_animals() {
	if (this->num_monkeys > 0 || this->num_seaotters > 0 || this->num_sloths > 0)
		return true;
	return false;
}

/*************************************************************************************************
FUNCTION NAME: num_animals
FUNCTION DESCRIPTION: count number of animals in zoo
INPUTS: none
OUTPUTS: returns number of animals in this zoo
**************************************************************************************************/
int Zoo::num_animals() {
	return this->num_monkeys + this->num_seaotters + this->num_sloths;
}

/*************************************************************************************************
FUNCTION NAME: monkey_death
FUNCTION DESCRIPTION: remove a monkey from this zoo
INPUTS: the index of the monkey to remove
OUTPUTS: removes a monkey
**************************************************************************************************/
void Zoo::monkey_death(int index) {
	this->num_monkeys--;

	Monkey* new_monkeys = new Monkey[this->num_monkeys];

	int j = 0;

	for (int i = 0; i < this->num_monkeys + 1; i++)
	{
		if (i != index)
			new_monkeys[j++] = this->monkeys[i];
	}


	delete[] this->monkeys;
	this->monkeys = new_monkeys;

	if (this->num_monkeys == 0)
		this->monkeys = NULL;
}

/*************************************************************************************************
FUNCTION NAME: seaotter_death
FUNCTION DESCRIPTION: remove a seaotter from this zoo
INPUTS: the index of the seaotter to remove
OUTPUTS: removes a seaotter
**************************************************************************************************/
void Zoo::seaotter_death(int index) {
	this->num_seaotters--;

	SeaOtter* new_seaotters = new SeaOtter[this->num_seaotters];

	int j = 0;

	for (int i = 0; i < this->num_seaotters + 1; i++)
	{
		if (i != index)
			new_seaotters[j++] = this->seaotters[i];
	}


	delete[] this->seaotters;
	this->seaotters = new_seaotters;

	if (this->num_seaotters == 0)
		this->seaotters = NULL;
		
}

/*************************************************************************************************
FUNCTION NAME: sloth_death
FUNCTION DESCRIPTION: remove a sloth from this zoo
INPUTS: the index of the sloth to remove
OUTPUTS: removes a sloth
**************************************************************************************************/
void Zoo::sloth_death(int index) {
	this->num_sloths--;

	Sloth* new_sloths = new Sloth[this->num_sloths];

	int j = 0;

	for (int i = 0; i < this->num_sloths + 1; i++)
	{
		if (i != index)
			new_sloths[j++] = this->sloths[i];
	}


	delete[] this->sloths;
	this->sloths = new_sloths;

	if (this->num_sloths == 0)
		this->sloths = NULL;
}

/*************************************************************************************************
FUNCTION NAME: sick_monkey
FUNCTION DESCRIPTION: makes user pay recovery cost or else monkey dies
INPUTS: index of sick monkey
OUTPUTS: deducts recovery cost or kills monkey
**************************************************************************************************/
void Zoo::sick_monkey(int index) {
	cout << "\nSick monkey!" << endl;

	int recovery_cost = Monkey::cost / 2;

	if (this->monkeys[index].get_age() < 6)
		recovery_cost *= 2;

	if (this->bank >= recovery_cost) {
		this->bank -= recovery_cost;
		cout << "\nCured. -$" << recovery_cost << endl;
	}
	else {
		cout << "\nCouldn't afford monkey. Monkey died.\n";
		monkey_death(index);
	}
}

/*************************************************************************************************
FUNCTION NAME: sick_seaotter
FUNCTION DESCRIPTION: makes user pay recovery cost or else seaotter dies
INPUTS: index of sick seaotter
OUTPUTS: deducts recovery cost or kills seaotter
**************************************************************************************************/
void Zoo::sick_seaotter(int index) {
	cout << "\nSick seaotter!" << endl;

	int recovery_cost = SeaOtter::cost / 2;

	if (this->monkeys[index].get_age() < 6)
		recovery_cost *= 2;

	if (this->bank >= recovery_cost) {
		this->bank -= recovery_cost;
		cout << "\nCured. -$" << recovery_cost << endl;
	}
	else {
		cout << "\nCouldn't afford seaotter. Seaotter died.\n";
		seaotter_death(index);
	}
}

/*************************************************************************************************
FUNCTION NAME: sick_sloth
FUNCTION DESCRIPTION: makes user pay recovery cost or else sloth dies
INPUTS: index of sick sloth
OUTPUTS: deducts recovery cost or kills sloth
**************************************************************************************************/
void Zoo::sick_sloth(int index) {
	cout << "\nSick sloth!" << endl;

	int recovery_cost = Sloth::cost / 2;

	if (this->sloths[index].get_age() < 6)
		recovery_cost *= 2;

	if (this->bank >= recovery_cost) {
		this->bank -= recovery_cost;
		cout << "\nCured. -$" << recovery_cost << endl;
	}
	else {
		cout << "\nCouldn't afford sloth. Sloth died.\n";
		sloth_death(index);
	}
}

/*************************************************************************************************
FUNCTION NAME: sick_event
FUNCTION DESCRIPTION: makes a random animal sick
INPUTS: none
OUTPUTS: calls either sick_monkey, sick_seaotter, or sick_sloth at random
**************************************************************************************************/
void Zoo::sick_event() {

	cout << "\nSick event!\n";

	/* generate random number between 0 and number of animals: */
	int sick_animal = rand() % num_animals();


	if (sick_animal < this->num_monkeys)
		sick_monkey(sick_animal);
	else if (sick_animal < this-> num_monkeys + this->num_seaotters)
		sick_seaotter(sick_animal - this->num_monkeys);
	else
		sick_sloth(sick_animal - this->num_monkeys - this->num_seaotters);

}

/*************************************************************************************************
FUNCTION NAME: num_adults
FUNCTION DESCRIPTION: count the number of adults in this zoo
INPUTS: none
OUTPUTS: returns the number of adult animals
**************************************************************************************************/
int Zoo::num_adults() {
	return num_adult_monkeys() + num_adult_seaotters() + num_adult_sloths();
}

/*************************************************************************************************
FUNCTION NAME: num_adult_monkeys
FUNCTION DESCRIPTION: count number of adult monkeys
INPUTS: none
OUTPUTS: returns the number of adult monkeys
**************************************************************************************************/
int Zoo::num_adult_monkeys() {
	int num_adults = 0;

	for (int i = 0; i < this->num_monkeys; i++)
	{
		if (this->monkeys[i].get_age() >= 24)
			num_adults++;
	}

	return num_adults;
}

/*************************************************************************************************
FUNCTION NAME: num_adult_seaotters
FUNCTION DESCRIPTION: count number of adult seaotters
INPUTS: none
OUTPUTS: returns the number of adult seaotters
**************************************************************************************************/
int Zoo::num_adult_seaotters() {
	int num_adults = 0;

	for (int i = 0; i < this->num_seaotters; i++)
	{
		if (this->seaotters[i].get_age() >= 24)
			num_adults++;
	}

	return num_adults;
}

/*************************************************************************************************
FUNCTION NAME: num_adult_sloths
FUNCTION DESCRIPTION: count number of adult sloths
INPUTS: none
OUTPUTS: returns the number of adult sloths
**************************************************************************************************/
int Zoo::num_adult_sloths() {
	int num_adults = 0;

	for (int i = 0; i < this->num_sloths; i++)
	{
		if (this->sloths[i].get_age() >= 24)
			num_adults++;
	}

	return num_adults;
}

/*************************************************************************************************
FUNCTION NAME: monkey_birth
FUNCTION DESCRIPTION: adds a newborn monkey
INPUTS: none
OUTPUTS: adds newborn to this zoo
**************************************************************************************************/
void Zoo::monkey_birth() {
	cout << "\nMonkey gave birth!\n";

	this->num_monkeys += Monkey::birth_rate;

	Monkey* new_monkeys = new Monkey[this->num_monkeys];

	if (this->num_monkeys - Monkey::birth_rate > 0) {
		for (int i = 0; i < this->num_monkeys - Monkey::birth_rate; i++)
		{
			new_monkeys[i] = this->monkeys[i];
		}
	}

	delete[] this->monkeys;

	this->monkeys = new_monkeys;

	for (int i = 0; i < Monkey::birth_rate; i++)
	{
		this->monkeys[num_monkeys - 1 - i].set_age(0);
	}
}

/*************************************************************************************************
FUNCTION NAME: seaotter_birth
FUNCTION DESCRIPTION: adds a newborn seaotter
INPUTS: none
OUTPUTS: adds newborn to this zoo
**************************************************************************************************/
void Zoo::seaotter_birth() {
	cout << "\nSeaotter gave birth!\n";

	this->num_seaotters += SeaOtter::birth_rate;

	SeaOtter* new_seaotters = new SeaOtter[this->num_seaotters];

	if (this->num_seaotters - SeaOtter::birth_rate > 0) {
		for (int i = 0; i < this->num_seaotters - SeaOtter::birth_rate; i++)
		{
			new_seaotters[i] = this->seaotters[i];
		}
	}

	delete[] this->seaotters;

	this->seaotters = new_seaotters;

	for (int i = 0; i < SeaOtter::birth_rate; i++)
	{
		this->seaotters[num_seaotters - 1 - i].set_age(0);
	}
}

/*************************************************************************************************
FUNCTION NAME: sloth_birth
FUNCTION DESCRIPTION: adds a newborn sloth
INPUTS: none
OUTPUTS: adds newborn to this zoo
**************************************************************************************************/
void Zoo::sloth_birth() {
	cout << "\nSloth gave birth!\n";

	this->num_sloths += Sloth::birth_rate;

	Sloth* new_sloths = new Sloth[this->num_sloths];

	if (this->num_sloths - Sloth::birth_rate > 0) {
		for (int i = 0; i < this->num_sloths - Sloth::birth_rate; i++)
		{
			new_sloths[i] = this->sloths[i];
		}
	}

	delete[] this->sloths;

	this->sloths = new_sloths;

	for (int i = 0; i < Sloth::birth_rate; i++)
	{
		this->sloths[num_sloths - 1 - i].set_age(0);
	}
}

/*************************************************************************************************
FUNCTION NAME: birth_event
FUNCTION DESCRIPTION: makes a random adult animal give birth
INPUTS: none
OUTPUTS: adds an animal to this zoo
**************************************************************************************************/
void Zoo::birth_event() {
	cout << "\nBirth event!\n";

	
	/* generate random number between 1 and number of adult animals: */
	int pregnant_animal = rand() % num_adults() + 1;



	cout << "Pregnant animal index: " << pregnant_animal << endl;

	if (pregnant_animal <= num_adult_monkeys())
		monkey_birth();

	else if (pregnant_animal <= num_adult_monkeys() + num_adult_seaotters())
		seaotter_birth();

	else
		sloth_birth();

}

/*************************************************************************************************
FUNCTION NAME: attendance_event
FUNCTION DESCRIPTION: a boom of attendance for one month
INPUTS: none
OUTPUTS: adds bonus to zoo's bank
**************************************************************************************************/
void Zoo::attendance_event() {
	cout << "\nAttendance event!\n";

	int total_bonus_revenue = 0;

	for (int i = 0; i < this->num_monkeys; i++)
	{
		/* generate random number between 300 and 700: */
		int bonus_revenue = rand() % 400 + 300;
		cout << "Bonus revenue for monkey " << i + 1 << " = $" << bonus_revenue << endl;
		total_bonus_revenue += bonus_revenue;
	}

	cout << "\n Total Bonus Revenue earned: +$" << total_bonus_revenue << endl;
	this->bank += total_bonus_revenue;
}

/*************************************************************************************************
FUNCTION NAME: collect_revenue
FUNCTION DESCRIPTION: adds the revenues to the bank
INPUTS: none
OUTPUTS: adds appropriate revenue to bank
**************************************************************************************************/
void Zoo::collect_revenue() {
	bool high_attendance = false;

	int revenue = 0;

	revenue += Monkey::revenue * this->num_monkeys;
	revenue += SeaOtter::revenue * this->num_seaotters;
	revenue += Sloth::revenue * this->num_sloths;
	

	this->bank += revenue;
	cout << "\nLAST MONTH'S REVENUES: +$" << revenue << "\n";
}

/*************************************************************************************************
FUNCTION NAME: increment ages
FUNCTION DESCRIPTION: increment ages of all animals
INPUTS: none
OUTPUTS: increment age of all animals by one month
**************************************************************************************************/
void Zoo::increment_ages() {
	int m = this->num_monkeys;
	int so = this->num_seaotters;
	int s = this->num_sloths;


	if (m > 0) {
		for (int i = 0; i < m; i++)
		{
			this->monkeys[i].increment_age();
		}
	}
	if (so > 0) {
		for (int i = 0; i < so; i++)
		{
			this->seaotters[i].increment_age();
		}
	}
	if (s > 0) {
		for (int i = 0; i < s; i++)
		{
			this->sloths[i].increment_age();
		}
	}

}

/*************************************************************************************************
FUNCTION NAME: bankrupt
FUNCTION DESCRIPTION: check if zoo ran out of money
INPUTS: none
OUTPUTS: returns true if zoo has less that $0
**************************************************************************************************/
bool Zoo::bankrupt() {

	if (this->bank < 0) {
		cout << "\nBANKRUPT! GAME OVER...\n";
		return true;
	}
	else {
		return false;
	}
}

/*************************************************************************************************
FUNCTION NAME: num_baby_monkeys
FUNCTION DESCRIPTION: count the number of baby monkeys
INPUTS: none
OUTPUTS: returns number of monkeys under 6 months old
**************************************************************************************************/
int Zoo::num_baby_monkeys() {
	int num_babies = 0;

	for (int i = 0; i < this->num_monkeys; i++)
	{
		if (this->monkeys[i].get_age() < 6)
			num_babies++;
	}

	return num_babies;
}

/*************************************************************************************************
FUNCTION NAME: num_baby_seaotters
FUNCTION DESCRIPTION: count the number of baby seaotters
INPUTS: none
OUTPUTS: returns number of seaotters under 6 months old
**************************************************************************************************/
int Zoo::num_baby_seaotters() {
	int num_babies = 0;

	for (int i = 0; i < this->num_seaotters; i++)
	{
		if (this->seaotters[i].get_age() < 6)
			num_babies++;
	}

	return num_babies;
}

/*************************************************************************************************
FUNCTION NAME: num_baby_sloths
FUNCTION DESCRIPTION: count the number of baby sloths
INPUTS: none
OUTPUTS: returns number of sloths under 6 months old
**************************************************************************************************/
int Zoo::num_baby_sloths() {
	int num_babies = 0;

	for (int i = 0; i < this->num_sloths; i++)
	{
		if (this->sloths[i].get_age() < 6)
			num_babies++;
	}

	return num_babies;
}

/*************************************************************************************************
FUNCTION NAME: num_babies
FUNCTION DESCRIPTION: count the total number of baby animals
INPUTS: none
OUTPUTS: returns number of baby animals 
**************************************************************************************************/
int Zoo::num_babies() {
	return num_baby_monkeys() + num_baby_seaotters() + num_baby_sloths();
}

/*************************************************************************************************
FUNCTION NAME: display_num_animals
FUNCTION DESCRIPTION: show the number of adults and babies of each species
INPUTS: none
OUTPUTS: prints quantities of adults and babies of each species to console
**************************************************************************************************/
void Zoo::display_num_animals() {
	cout << "\nYOU OWN:\n";
	cout << this->num_monkeys << " monkeys,\n";
	cout << this->num_seaotters << " seaotters,\n";
	cout << this->num_sloths << " sloths.\n";

	cout << "\n\nThere are " << num_adults() << " adult animals.\n";
	cout << num_adult_monkeys() << " adult monkeys, " << num_adult_seaotters() << " seaotters, " << num_adult_sloths() << " sloths\n";


	cout << "\n\nThere are " << num_babies() << " baby animals.\n";
	cout << num_baby_monkeys() << " baby monkeys, " << num_baby_seaotters() << " seaotters, " << num_baby_sloths() << " sloths\n";

}

/*************************************************************************************************
FUNCTION NAME: try_buy_animals
FUNCTION DESCRIPTION: buy animals, if you run out of money it returns false
INPUTS: none
OUTPUTS: buys/adds animals and deducts from bank
**************************************************************************************************/
bool Zoo::try_buy_animals() {
	bool buy_more = get_yes_no_input("BUY ANIMALS?");

	if (buy_more) {
		int animal_type = get_natural_input("Monkey (1), SeaOtter (2), Sloth (3), or Dont Buy (4): ");

		switch (animal_type)
		{	
		case 1: // Buy monkey
			buy_monkeys(get_quantity());
			display_bank();
			break;

		case 2: // Buy seaotter
			buy_seaotters(get_quantity());
			display_bank();
			break;

		case 3: // Buy sloth
			buy_sloths(get_quantity());
			display_bank();
			break;

		default: // buy nothing
			cout << "\nNo longer buying this month!\n";
			break;
		}
	}

	return !bankrupt();
}

/*************************************************************************************************
FUNCTION NAME: get_quantity
FUNCTION DESCRIPTION: returns the number to buy as either 1 or 2
INPUTS: none
OUTPUTS: returns 1 or 2
**************************************************************************************************/
int Zoo::get_quantity() {
	if (get_yes_no_input("Would you like to buy two?"))
		return 2;
	return 1;
}

/*************************************************************************************************
FUNCTION NAME: buy_monkeys
FUNCTION DESCRIPTION: buy a certain number of adult monkeys
INPUTS: the quantity to buy
OUTPUTS: adds a certain number of monkeys and deducts from bank
**************************************************************************************************/
void Zoo::buy_monkeys(int quantity) {
	cout << "\nBOUGHT " << quantity << " monkeys! -$" << Monkey::cost * quantity << endl;

	this->bank -= Monkey::cost * quantity;

	this->num_monkeys += quantity;

	Monkey* new_monkeys = new Monkey[this->num_monkeys];


	if (this->num_monkeys - quantity > 0) {
		// copy over old monkeys to new larger array
		for (int i = 0; i < this->num_monkeys - quantity; i++)
		{
			new_monkeys[i] = this->monkeys[i];
		}
	}
	
	delete[] this->monkeys;


	this->monkeys = new_monkeys;


	this->monkeys[num_monkeys - 1].set_age(24);

	if (quantity == 2)
		this->monkeys[num_monkeys - 2].set_age(24);

}

/*************************************************************************************************
FUNCTION NAME: buy_seaotters
FUNCTION DESCRIPTION: buy a certain number of adult seaotters
INPUTS: the quantity to buy
OUTPUTS: adds a certain number of seaotters and deducts from bank
**************************************************************************************************/
void Zoo::buy_seaotters(int quantity) {
	cout << "\nBOUGHT " << quantity << " seaotters! -$" << SeaOtter::cost * quantity << endl;

	this->bank -= SeaOtter::cost * quantity;

	this->num_seaotters += quantity;


	SeaOtter* new_seaotters = new SeaOtter[this->num_seaotters];


	if (this->num_seaotters - quantity > 0) {
		// copy over old seaotters to new larger array
		for (int i = 0; i < this->num_seaotters - quantity; i++)
		{
			new_seaotters[i] = this->seaotters[i];
		}
	}

	delete[] this->seaotters;


	this->seaotters = new_seaotters;

	this->seaotters[num_seaotters - 1].set_age(24);


	if (quantity == 2)
		this->seaotters[num_seaotters - 2].set_age(24);
}

/*************************************************************************************************
FUNCTION NAME: buy_sloths
FUNCTION DESCRIPTION: buy a certain number of adult sloths
INPUTS: the quantity to buy
OUTPUTS: adds a certain number of sloths and deducts from bank
**************************************************************************************************/
void Zoo::buy_sloths(int quantity) {
	cout << "\nBOUGHT " << quantity << " sloths! -$" << Sloth::cost * quantity << endl;

	this->bank -= Sloth::cost * quantity;

	this->num_sloths += quantity;

	Sloth* new_sloths = new Sloth[this->num_sloths];


	if (this->num_sloths - quantity > 0) {
		// copy over old sloths to new larger array
		for (int i = 0; i < this->num_sloths - quantity; i++)
		{
			new_sloths[i] = this->sloths[i];
		}
	}

	delete[] this->sloths;


	this->sloths = new_sloths;


	this->sloths[num_sloths - 1].set_age(24);

	if (quantity == 2)
		this->sloths[num_sloths - 2].set_age(24);
}

/*************************************************************************************************
FUNCTION NAME: display_bank
FUNCTION DESCRIPTION: show the amount of money currently in bank
INPUTS: none
OUTPUTS: prints the zoo's bank account amount of money to console
**************************************************************************************************/
void Zoo::display_bank() {
	cout << "\nBANK $$$: " << this->bank << "\n";
}





// ************ CONSTRUCTORS ************ //

Zoo::Zoo() {
	this->bank = 0;
	this->month = 1;
	this->base_food_cost = 0;

	this->monkeys = NULL;
	this->seaotters = NULL;
	this->sloths = NULL;

	this->num_monkeys = 0;
	this->num_seaotters = 0;
	this->num_sloths = 0;

}



// ************ DECONSTRUCTOR ************ //

Zoo::~Zoo() {

	delete[] this->monkeys;
	this->monkeys = NULL;

	delete[] this->seaotters;
	this->seaotters = NULL;

	delete[] this->sloths;
	this->sloths = NULL;

}



// ************ ACCESSORS ************ //

Monkey* Zoo::get_monkeys() {
	return this->monkeys;
}


SeaOtter* Zoo::get_seaotters() {
	return this->seaotters;
}


Sloth* Zoo::get_sloths() {
	return this->sloths;
}


int Zoo::get_num_monkeys() {
	return this->num_monkeys;
}


int Zoo::get_num_seaotters() {
	return this->num_seaotters;
}


int Zoo::get_num_sloths() {
	return this->num_sloths;
}


int Zoo::get_base_food_cost() {
	return this->base_food_cost;
}


int Zoo::get_bank(){
	return this->bank;
}


int Zoo::get_month() {
	return this->month;
}




// ************ MUTATORS ************ //

void Zoo::set_monkeys(Monkey* monkeys) {
	this->monkeys = monkeys;
}


void Zoo::set_seaotters(SeaOtter* seaotters) {
	this->seaotters = seaotters;
}


void Zoo::set_sloths(Sloth* sloths) {
	this->sloths = sloths;
}


void Zoo::set_num_monkeys(int num_monkeys) {
	this->num_monkeys = num_monkeys;
}


void Zoo::set_num_seaotters(int num_seaotters) {
	this->num_seaotters = num_seaotters;
}


void Zoo::set_num_sloths(int num_sloths) {
	this->num_sloths = num_sloths;
}


void Zoo::set_base_food_cost(int base_food_cost) {
	this->base_food_cost = base_food_cost;
}


void Zoo::set_bank(int bank) {
	this->bank = bank;
}


void Zoo::set_month(int month) {
	this->month = month;
}

