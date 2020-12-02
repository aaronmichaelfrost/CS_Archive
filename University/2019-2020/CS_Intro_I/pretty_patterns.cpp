/************************************************
 * Program: patterns.cpp
 * Author: Aaron Frost
 * Date: 2/13/2020
 * Description: Creates pretty patterns output to console using iteration
*************************************************/



#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;


void StartProg();
void Asterisk(int rows);
void Number();

int main(){
	StartProg();
}

void StartProg(){

	string input1;
	cout << "Pattern of asterisks? (1), or pattern of numbers? (2): "; cin >> input1;



	if(input1 == "1"){
		string input2; 
		int rows;

		cout << "Enter number of rows (positive and odd please): "; cin >> input2;
		rows = atoi(input2.c_str());
		Asterisk(rows);
	}
	else if (input1 == "2"){
		Number();
	}
	else{
		cout << "Invalid input." << endl;
	        StartProg();	
	}
	
	string input;
	cout << "Print another pattern? (0-no, 1-yes): "; cin >> input;

	if(input == "1")
		StartProg();
	else
		cout << "Terminating program." << endl;


}


void Asterisk(int rows){

	// Finds the center row index
	int midpoint = (rows-1)/2 + 1;

	for(int i = 1; i <= rows; i++){
		
		string s = "";

		// Calculate spaces and asterisks
		int spaces = abs(midpoint - i);
		int asterisks = rows - (abs(midpoint - i) * 2);
		
		// Add spaces and asterisks to string
		for(int j = 0; j < spaces; j++)
			s += " ";
		for(int k = 0; k < asterisks; k++)
			s += "*";
	
		// Print row
		cout << s << endl;
	}
}
void Number(){

	for(int row = 1; row < 10; row++){
		string s = "";

		// Calculate spaces and digits
		int spaces = abs(5 - row);
		int digits = 9 - (abs(5 - row) * 2);

		// Add spaces
		for(int j = 0; j < spaces; j++)
			s += " ";

		// Add digits
		for(int k = 0; k < digits; k++){
			int x = k+1;

			// Integer to string conversion
			stringstream out;
			out << x;
			string t = out.str();

			s += t;
		}	
		cout << s << endl;	
	}
}
