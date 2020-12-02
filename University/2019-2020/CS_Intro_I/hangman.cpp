#include <iostream>
#include <string>

using namespace std;

void get_string(string *);
void set_replace_string(string, string *);
int get_search_replace(char, string, string &);
bool isDone(string);


int main(){
	string input;
	string output;

	get_string(&input);
	set_replace_string(input, &output);

	cout << output << endl;
	
	char guessLetter;

	while (!isDone(output)){

		cout << output;

		cout << "Guess a letter: "; cin >> guessLetter;
	
		cout << "This letter appears: " << get_search_replace(guessLetter, input, output) << " times." << endl;
	
	}


}

void get_string(string *input){
	cout << "Enter a word for hangman: ";
	getline(cin, *input);
}

void set_replace_string(string a, string *b){
	string c;
	for(int i = 0; i < a.length(); i++){
		if(a[i] != ' ')
			c += "-";
		else
			c += " ";
	}
	*b = c;
}

int get_search_replace(char a, string input, string &c){
	int x = 0; 
	
	for( int i = 0; i < input.length(); i++){
		if(input[i] == a){
			x++;
			c[i] = a;
		}
	}
	return x;
}

bool isDone(string s){
	for(int i = 0; i < s.length(); i++ ){
		if(s[i] == '-')
		      return false;	
	
	}
	return true;
}




