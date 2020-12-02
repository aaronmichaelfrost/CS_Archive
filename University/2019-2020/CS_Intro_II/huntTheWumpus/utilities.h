#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
using namespace std;


string get_string_input(string);
bool is_int(string);
int get_int_input(string);
int get_natural_input(string);
string to_lower(string);
bool get_yes_no_input(string);

int get_int_in_range(string prompt, int low, int high);




#endif