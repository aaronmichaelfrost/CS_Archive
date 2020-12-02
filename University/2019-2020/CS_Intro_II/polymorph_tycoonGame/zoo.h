#ifndef ZOO_H
#define ZOO_H


#include <iostream>
using namespace std;


#include "animal.h"

#include "sloth.h"
#include "monkey.h"
#include "seaotter.h"


class Zoo {
private:



	Monkey* monkeys;
	SeaOtter* seaotters;
	Sloth* sloths;

	int num_monkeys;
	int num_seaotters;
	int num_sloths;
	int base_food_cost;
	int bank;
	int month;


public:

	Zoo();
	~Zoo();

	Monkey* get_monkeys();
	SeaOtter* get_seaotters();
	Sloth* get_sloths();
	int get_num_monkeys();
	int get_num_seaotters();
	int get_num_sloths();
	
	int get_base_food_cost();

	int get_bank();
	int get_month();

	void set_monkeys(Monkey*);
	void set_seaotters(SeaOtter*);
	void set_sloths(Sloth*);
	void set_num_monkeys(int);
	void set_num_seaotters(int);
	void set_num_sloths(int);

	void set_base_food_cost(int);

	void set_bank(int);
	void set_month(int);

	int get_quantity();

	bool has_animals();
	bool done_playing();

	int num_animals();


	void display_all_animals();
	void display_month();
	void display_monthly_info();
	void display_num_animals();
	bool try_buy_animals();
	void display_bank();
	void collect_revenue();

	bool try_start_new_month();

	void sick_monkey(int);
	void sick_seaotter(int);
	void sick_sloth(int);

	void monkey_death(int);
	void seaotter_death(int);
	void sloth_death(int);


	int num_adults();
	int num_adult_monkeys();
	int num_adult_seaotters();
	int num_adult_sloths();

	int num_babies();
	int num_baby_monkeys();
	int num_baby_seaotters();
	int num_baby_sloths();

	void monkey_birth();
	void seaotter_birth();
	void sloth_birth();

	bool try_special_event();
	void sick_event();
	void birth_event();
	void attendance_event();

	bool bankrupt();
	void increment_ages();
	void increment_zoo_info();

	void sell_animal();
	void sell_monkey(int);
	void sell_seaotter(int);
	void sell_sloth(int);

	void buy_monkeys(int);
	void buy_seaotters(int);
	void buy_sloths(int);


	bool try_feeding();

	void main_loop();

};





#endif