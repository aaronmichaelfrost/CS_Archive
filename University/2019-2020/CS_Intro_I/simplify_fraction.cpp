/***************************
 * Program: fraction.cpp
 * Author: Aaron Frost
 * Date: 2/13/2020
 * Description: Simplifies fractions to lowest terms by finding greatest common denominator
****************************/


#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdlib>

using namespace std;


bool is_int(string);
bool to_lowest_terms(int&, int&);
void get_int_input(int&, string);
int gcd(int numerator, int denominator);
void Simplify();

int main(){

	Simplify();

}

void Simplify(){
	
	int num, den;

	get_int_input(num, "Enter numerator: ");
	get_int_input(den, "Enter denominator: ");
	
	while(den == 0){
		get_int_input(den, "Denominator cannot be 0. Try again: ");
	}

	if(to_lowest_terms(num, den)){
	
	
	}else{
		cout << "Could not divide by zero!" << endl;
	}		

 	cout << "The lowest terms of your fraction: " << num << " / " << den << endl;

	string input;

	cout << "Do another conversion? (0-no, 1-yes): "; cin >> input;

	if(input == "1")
		Simplify();
	else
		cout << "Terminating program." << endl;
}



bool to_lowest_terms(int &numerator, int &denominator){

	if(denominator == 0)
		return false;
	else{
		// Find the GCD
		int _gcd = gcd(numerator, denominator);

		// Divide by GCD
		numerator /= _gcd;
		denominator /= _gcd;
	}
}

int gcd(int num1, int num2){
	
	int _num1 = abs(num1);
	int _num2 = abs(num2);

	// Everything can be divided by zero.
	if(_num1 == 0)
		return num2;
	if(_num2 == 0)
		return num1;


	// When the numbers are the same, that is the greatest common denominator
	if(_num1 == _num2)
		return num1;


	// Subtract the lower number from the higher number while the lower number remains the same 
	if(_num1 > _num2)
		return gcd(_num1 - _num2, _num2);

	return gcd(_num1, _num2 - _num1);
} 

void get_int_input(int &x, string prompt){
	string s;
	cout << prompt; cin >> s;
       	if(!is_int(s)){
		cout << "Invalid input. " << endl;
		get_int_input(x, prompt);
	}
	else{
		x = atoi(s.c_str());
	}
}



bool is_int(string n){
	for(int i = 0; i < n.length(); i++){
		char c = n[i];

		if(i == 0 && c == '-')
			continue;
		else if (c < '0' || c > '9')
			return false;
	}

	return true;
}
