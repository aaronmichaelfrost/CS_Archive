/*****************************************************************
 * Program name: Assignment4.cpp
 * Author: Aaron Frost
 * Date: 2/29/2020
 * Description: Functions preform operations on dynamic C style and C++ object strings
 * Inputs: String
 * Outputs: Return results from operations preformed on string
*******************************************************************/

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string>


using namespace std;

// Gets input line from the user and stores it as a c style string
void ReadLine(char* &, int, char*);

// The inital options function which is called until user wishes to terminate program.
void Start(char* &, int);

// Prints a comparison of the quantity of vowels versus consonants in a string.
void vowels_vs_consonants(char* &);

// Gets input on letter to be replaced, and letter to replace, and replaces in the original string.
void letter_swap(char* &);

// Prints version of string with all letters flipped.
void reverse_string(char*);

// Returns true if string argument is palindrone.
bool is_palindrome(char*);

// User enters an array of strings which are searched for inside the argument string, appearances of each word are printed
void word_frequency(char*);

// If character is uppercase, it is converted to lowercase, and vice versa. Inverted casing character is returned.
char inverse_char_casing(char);

// Removes spaces and non alphabetical characters from a c style string. Also makes all letters lowercase.
void purge_string(char*, char* &);

// Returns amount of times a specific substring appear
int word_quantity(char*, string);

// Converts character to lowercase if it is an uppercase character
char to_lowercase(char);




int main(){

	// Establish string for getting main input.
	int length = 1024;
	char *mainInput = new char[length];
	
	Start(mainInput, length - 1);

	delete []mainInput;

	return 0;
}

void Start(char* &input, int length){
	
	cin.clear();
	cin.ignore();
	
	char message[] = "Type quit to exit program. Type any other line to begin: ";
	ReadLine(input, length, message);

 	if(strstr(input, "quit")){
		cout << "Terminating program." << endl;
		exit(0);			
	} 

	cout << "You're string is: " << input << endl;

	
	cin.clear();

	int option;
	cout << "Choose option: (Vowels_Vs_Consonants (1), Letter_Swap (2), Flip_String (3), Palindrone_Detector(4), Words_Frequency(5)): ";

	
	cin >> option;

	switch(option){
		case 1:
			cout << "Vowels vs Consonants: " << endl;
			vowels_vs_consonants(input);
			break;
		case 2:	
			cout << "Letter Swap: " << endl;
			letter_swap(input);
			cout << "Our new string: " << input << endl;
			break;
		case 3:
			reverse_string(input);
			cout << "Original: " << input << endl;
			break;
		case 4:
			if(is_palindrome(input)){
				cout << "It is a palindrome." << endl;
			}
			else{
				cout << "It is not a palindrome." << endl;
			}	
			break;
		case 5:
			word_frequency(input);
			break;
		default:
			Start(input, length);
			break;
	}
	cin.clear();
	Start(input, length);
}

char to_lowercase(char c){
	if(c >= 'A' && c <= 'Z')
		return c + 32;
	return c;
}

void word_frequency(char* input){ 
	int N;
	
	cout << "How many words do you want to search for?: "; cin >> N;

	string* wordArray = new string[N];


	// Add words to array
	for(int i = 0; i < N; i++){
		cin.ignore();
		string word;
		cout << "Enter a word (casing matters!): "; cin >> word;
		wordArray[i] = word;
	}

	// Display total appearances for each word
	for(int i = 0; i < N; i++){
		cout << wordArray[i] << ": " << word_quantity(input, wordArray[i]) << endl; 	
	}
	delete []wordArray;
}

int word_quantity(char* original, string wordtofind){
	// Returns number of times that word appears in original
	
	int appearances = 0;
	int letterindex = 0;

	// Loop through each character in original string
	for(int i = 0; i < strlen(original); i++){

		// If a letter matches, increase index
		if(original[i] == wordtofind[letterindex]){
			letterindex++;
		}
		else{ // If it doesnt match, reset index
			letterindex = 0;
		}

		// If index matches length of word, add to appearances, and reset index
		if(letterindex == wordtofind.length()){
			appearances++;
			letterindex = 0;
		}	
	}
	return appearances;
}

bool is_palindrome(char* string){

	char* cleanstring;
	cleanstring = new char[strlen(string)];
	purge_string(string, cleanstring);
	
	int x = strlen(cleanstring) - 1;

	for(int i = 0; x >= i; i++){
		if(cleanstring[x] != cleanstring[i]){
			delete []cleanstring;
			return false;
		}
		x--;
	}
	delete []cleanstring;
	return true;
}

void purge_string(char* string, char* &stringnew){

	for(int i = 0; string[i] != '\0'; i++){
		if(string[i] > 64 && string[i] < 91){
			string[i] = inverse_char_casing(string[i]);
		}

		if(string[i] > 96 && string[i] < 123)
		       strncat(stringnew, &string[i], 1);		
	}
}

void ReadLine(char* &charArray, int length, char* message){

	cin.clear();

	cout << message; cin.getline(charArray, length);

	if(cin.fail()){
		cin.ignore(length,'\n');
		cin.clear();
		ReadLine(charArray, length, message);
	}

	cin.clear();
	cin.ignore();
}

void reverse_string(char* string){
	char* charArray;
	charArray = new char[strlen(string)];
	
	strcpy(charArray, string);

	cout << strlen(charArray) << endl;

	int x = 0;
	for(int i = strlen(charArray) - 1; i > -1; i--){
		charArray[i] = string[x];
		x++;
	}

	cout << "Reversed string: "<< charArray << endl;

	delete []charArray;
}

void letter_swap(char* &string){
	char c1;
	char c2; 

	char message[] = "What is the character we want to replace?: ";
	cout << message;
	cin >> c1; 

	cin.ignore();

	char message2[] = "What is the character we will replace it with?: ";
	cout << message2;
	cin >> c2;
  
	cin.ignore(); 

	if(c1 > 64 && c1 < 91)
		c1 += 32;

	// Replace characters in string
	for(int i = 0; string[i] != '\0'; i++){
		if(string[i] == c1){
			string[i] = c2;
		}
		if(string[i] == inverse_char_casing(c1)){
			string[i] = inverse_char_casing(c2);
		}
	}
}

char inverse_char_casing(char a){
	if(a > 96)
		return a - 32;
	else
		return a + 32;
}

void vowels_vs_consonants(char* &string){
	int vowels = 0;
	int consonants = 0;

	// For each character in string
	for(int i = 0; string[i] != '\0'; i++){

		char ch = string[i];

		// If character is alphabetical
		if((ch >=  'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
			
			// If character is a vowel
			if(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' || ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U'){
				vowels++;
			}
			else
				consonants++;
		}
	}

	// Print results
	if(vowels == consonants)
		cout << "Number of vowels = number of consonants." << endl;
	else if (vowels > consonants)
		cout << "Number of vowels > number of consonants." << endl;
	else
		cout << "Number of vowels < number of consonants." << endl;
}


