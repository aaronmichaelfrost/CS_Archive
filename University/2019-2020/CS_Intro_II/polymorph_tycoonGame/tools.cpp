
#include "tools.h"
#include <cstring>
#include <cstdlib>

#include <iostream>

using namespace std;


/*************************************************************************************************
FUNCTION NAME: get_yes_no_input
FUNCTION DESCRIPTION: use this to prompt for and get a yes or no input
INPUTS: prompt is the question that is displayed
OUTPUTS: returns true if input is y or Y, returns false if it is n or N
**************************************************************************************************/
bool get_yes_no_input(string prompt) {
	string inputString;

	cout << "\n";

	if (prompt != "")
		cout << prompt;

	cout << " (y/n): ";

	getline(cin, inputString);

	cout << "\n";

	if (to_lower(inputString) == "y")
	{
		return true;
	}
	else if (to_lower(inputString) == "n") {
		return false;
	}
	else {
		return get_yes_no_input(prompt);
	}

}


/*************************************************************************************************
FUNCTION NAME: get_string_input
FUNCTION DESCRIPTION: use this to prompt for and get string input
INPUTS: prompt is the message that will be displayed
OUTPUTS: string inputted by user
**************************************************************************************************/
string get_string_input(string prompt) {
	string inputString;

	if (prompt != "")
		cout << prompt;

	getline(cin, inputString);

	if (inputString.length() < 1)
		return get_string_input(prompt);
	else
		return inputString;
}


/*************************************************************************************************
FUNCTION NAME: to_lower
FUNCTION DESCRIPTION: use this to make all capital letters lowercase
INPUTS: input a string you wish to be made lowercase
OUTPUTS: returns a lowercase string
**************************************************************************************************/
string to_lower(string input) {
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] >= 65 && input[i] <= 90)
			input[i] += 32;
	}
	return input;
}


/*************************************************************************************************
FUNCTION NAME: is_int
FUNCTION DESCRIPTION: check if a string can be converted directly to an integer
INPUTS: input a string you want to check if it is an integer
OUTPUTS: returns true if the inputted string can be converted to integer
**************************************************************************************************/
bool is_int(string inputString) {
	for (int i = 0; i < inputString.length(); i++)
	{
		if (inputString[i] < '0' || inputString[i] > '9') {
			return false;
		}
	}
	return true;
}


/*************************************************************************************************
FUNCTION NAME: get_int_input
FUNCTION DESCRIPTION: use this to get an integer from the user
INPUTS: input a string to be used in prompting the user
OUTPUTS: returns an integer the user entered
**************************************************************************************************/
int get_int_input(string prompt) {
	string inputString;

	if (prompt != "")
		cout << prompt;

	getline(cin, inputString);

	if (inputString.length() < 1)
		return get_int_input(prompt);
	else {
		if (!is_int(inputString)) {
			cout << "Invalid input. Please enter positive integers only." << endl;
			return get_int_input(prompt);
		}
		else {
			return atoi(inputString.c_str());
		}
	}
}


/*************************************************************************************************
FUNCTION NAME: get_natural_input
FUNCTION DESCRIPTION: use this to get an integer greater than 0 from the user
INPUTS: input a string to be used in prompting the user
OUTPUTS: returns the integer the user entered if it is greater than 0
**************************************************************************************************/
int get_natural_input(string prompt) {
	string inputString;

	if (prompt != "")
		cout << prompt;

	getline(cin, inputString);

	if (inputString.length() < 1)
		return get_int_input(prompt);
	else {
		if (!is_int(inputString)) {
			cout << "Invalid input. Please enter positive integers only." << endl;
			return get_int_input(prompt);
		}
		else {

			if (atoi(inputString.c_str()) < 1) {
				cout << "Invalid input. Please enter positive integers greater than zero only." << endl;
				return get_int_input(prompt);
			}
			return atoi(inputString.c_str());
		}
	}
}