/*******************************
 * Program: Assignment2.cpp
 * Author: Aaron Frost
 * Date 2/2/2020
 * Description: Tests library of Helper and Error Handling Functions
 * Input: function test input values
 * Output: function test output results
********************************/




#include <iostream>
#include <string>
#include <cmath>

using namespace std;



/// Function Declarations ///


bool check_range(int lowBound, int highBound, int testValue = 5);
bool is_capital(char);
bool is_even(int);
bool is_odd(int);
int equality_test(int, int);
bool float_is_equal(float, float, float precision);
bool is_int(string);
bool numbers_present(string);
bool letters_present(string);
bool contains_sub_string(string s, string subs);
int word_count(string);
string to_upper(string);
string to_lower(string);




int main(){
	cout << "Testing check_range: " << check_range(1, 5, 0) << endl;	
	cout << "Testing is_capital: " << is_capital('a') << endl;
	cout << "Testing is_even: " << is_even(4) << endl;
	cout << "Testing is_odd: " << is_odd(3) << endl;
	cout << "Testing equality_test: " << equality_test(1, 3) << endl;
	cout << "Testing float_is_equal: " << float_is_equal(1.01, 1.02, .02) << endl;
	cout << "Testing for is_int: " << is_int("Test1") << endl;
	cout << "Testing for numbers_present: " << numbers_present("1") << endl;
	cout << "Testing for letters_present: " << letters_present("Test") << endl;
	cout << "Testing for contains_sub_string: " << contains_sub_string("Hello There", "The") << endl;
	cout << "Testing for word_count: " << word_count("The apple was there in 1 . man.") << endl;
	cout << "Testing for to_upper: " << to_upper("This will be uppercase!") << endl;
	cout << "Testing for to_lower: " << to_lower("This Will be LOWERCASE!") << endl;

	string input;
	cout << "Testing get_int, enter a string: "; cin >> input;
	cout << "Int is " << get_int(input) << endl;
}



/// Function Definitions ///



/*
 * Function: check_range()
 * Description: Indicate if given value is within a givin range
 * Paramaters: int lowBound, int highBound, int testValue
 * Pre-conditions: take parameters
 * Post-conditions: return a value to tell if given int is within range
*/

bool check_range(int lowBound, int highBound, int value){
	if(value >= lowBound && value <= highBound)
		return true;
	else
		return false;
}


/*
 * Function: is_capital()
 * Description: Indicate if given character is a capitalized letter
 * Paramaters: char letter
 * Pre-conditions: take char
 * Post-conditions: return a value to tell if given letter is a capital letter
*/

bool is_capital(char letter){
	if (letter >= 65 && letter <= 90)
		return true;
	else
		return false;
}

/*
 * Function: is_even()
 * Description: Returns true if given integer is even
 * Paramaters: int num
 * Pre-conditions: take int
 * Post-conditions: return a value to tell if given int is even
*/

bool is_even(int num){
	if (num % 2 == 0)
		return true;
	else
		return false;
}

/*
 * Function: is_odd()
 * Description: Returns true if given integer is odd
 * Paramaters: int num
 * Pre-conditions: take int
 * Post-conditions: return a value to tell if given int is odd
*/

bool is_odd(int num){
	if (is_even(num))
		return false;
	else
		return true;
}

/*
 * Function: equality_test()
 * Description: Returns -1 if num1 > num2, 0 if num1 == num2, 1 if num1 > num2
 * Paramaters: int num1, int num2
 * Pre-conditions: take parameters
 * Post-conditions: return a value to compare two numbers
*/

int equality_test(int num1, int num2){
	if(num1 < num2)
		return -1;
	else if (num1 == num2)
		return 0;
	else if (num1 > num2)
		return 1;
}

/*
 * Function: float_is_equal()
 * Description: Returns true if the difference between two numbers is less than or equal to a certain precision
 * Paramaters: float num1, float num2, float precision
 * Pre-conditions: take parameters
 * Post-conditions: return a value to determine float near equality
*/

bool float_is_equal(float num1, float num2, float precision){
	if(abs(num1 - num2) <= precision)
		return true;
	else
		return false;
}

/*
 * Function: is_int()
 * Description: Returns true if string is an integer
 * Paramaters: string input
 * Pre-conditions: take parameters
 * Post-conditions: return a value to determine if string is an integer
*/

bool is_int(string s){

	for(int i; i < s.length(); i++){
		char c = s[i];

		if(i == 0 && c == '-')
			continue;
		else if(c < '0' || c > '9')
			return false;	
	}
	return true;
}

/*
 * Function: numbers_present()
 * Description: Returns true if string contains a number
 * Paramaters: string input
 * Pre-conditions: take parameters
 * Post-conditions: return a value to determine if string contains a number
*/

bool numbers_present(string s){
	for(int i; i < s.length(); i++){
		char c = s[i];

		if(c >= '0' && c <= '9')
			return true;
	}
	return false;
}

/*
 * Function: letters_present()
 * Description: Returns true if string contains a letter
 * Paramaters: string input
 * Pre-conditions: take parameters
 * Post-conditions: return a value to determine if string contains a letter
*/

bool letters_present(string s){
	for(int i; i < s.length(); i++){
		char c = s[i];

		if((c < 65 || c > 90) && (c < 97 || c > 122))
			return false;
	}
	return true;
}

/*
 * Function: contains_sub_string()
 * Description: Returns true if string contains specified sub string
 * Paramaters: string s, string subs
 * Pre-conditions: take parameters
 * Post-conditions: return a value to determine if string contains substring
*/

bool contains_sub_string(string s, string subs){
	int j = 0;

	for(int i = 0; i < s.length(); i++){
		if(s[i] == subs[j])
			j++;
		else if (j != subs.length())
			j = 0;
	}

	if(j == subs.length())
		return true;
 	else
		return false;
}

/*
 * Function: word_count()
 * Description: Returns number of words in a string
 * Paramaters: string s
 * Pre-conditions: take parameters
 * Post-conditions: return a value to determine number of words
*/

int word_count(string s){
	int words = 0;
	bool lastCharWasSpace = true;
	for(int i = 0; i < s.length(); i++){
		if(s[i] != ' ' && lastCharWasSpace){
			lastCharWasSpace = false;
			words++;
		}
			
		else if (s[i] == ' ')
			lastCharWasSpace = true;
		else
			lastCharWasSpace = false;			
	}
	return words;
}

/*
 * Function: to_upper()
 * Description: Returns string with each letter capitalized
 * Paramaters: string s
 * Pre-conditions: take parameters
 * Post-conditions: return a string with all capital letters
*/

string to_upper(string s){

	string _s = "";
	for(int i = 0; i < s.length(); i++){
		if(s[i] <= 'z' && s[i] >= 'a'){
			_s += (char)((int)s[i] - 32);
		}
		else{
			_s += s[i];
		}

	}
	return _s;

}

/*
 * Function: to_lower()
 * Description: Returns string of lowercase letters 
 * Paramaters: string s
 * Pre-conditions: take parameters
 * Post-conditions: return a string in all lowercase
*/

string to_lower(string s){
	string _s = "";
	for(int i = 0; i < s.length(); i++){
		if(s[i] <= 'Z' && s[i] >= 'A'){
			_s += (char)((int)s[i] + 32);
		}
		else{
			_s += s[i];
		}

	}
	return _s;
}


