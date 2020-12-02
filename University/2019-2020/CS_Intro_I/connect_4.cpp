#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

// Deletes a dynamic 2d char array from memory
void delete_2darray(char**, int);

// Singleplayer game loop
void play_solo(char**, int, int);

// Multiplayer game loop
void play_multi(char**, int, int, int);

// AI place token
void ai_move(char**, char, int, int);

// Player place token
void p_move(char**, char, int, int);

// Returns the lowest empty row within a given column
int lowest_empty_row(char**, char, int, int, int);

// Returns true if there are four of the same tokens adjacent or diagonal, or if board is full
bool win_or_tie(char**, int rows, int cols);

// Gets input for dimnensions of board
void get_dimensions(int&, int&);




bool horizontal_check(char**, int, int);
bool vertical_check(char**, int, int);
bool diagonal_check(char**, int, int);

int main(int argc, char** argv) {

	srand(time(NULL));

	// Convert arguments to integers
	const char* p = argv[1];
	const char* r = argv[2];
	const char* c = argv[3];

	int players = atoi(p);
	int rows = atoi(r);
	int columns = atoi(c);

	// Handle dimensions and recover from bad values
	while(rows > 20 || columns > 20 || rows < 1 || columns < 1) 
		get_dimensions(rows, columns);
	

	// Create board
	char** board = new char*[rows];

	for(int i = 0; i < rows; i++){
		board[i] = new char[columns];
	}

	// Init board
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			board[i][j] = ' ';
		}
	}

	// Game loops
	if(players == 1)
	{
		play_solo(board, rows, columns);
	}
	else
	{
		play_multi(board, players, rows, columns);
	
	}

	delete_2darray(board, rows);
	
	return 0;
}


void play_solo(char** board, int rows, int cols){
	
	bool aiFirst;

	cout << "Ai first(1), or player first(0)?: "; cin >> aiFirst;

	if(aiFirst){
		ai_move(board, 'O', rows, cols);
	}

	do
	{
		p_move(board, 'X', rows, cols);

		if(!win_or_tie(board, rows, cols))
			ai_move(board, 'O', rows, cols);
		else
			break;
		
	}
	while(!win_or_tie(board, rows, cols));
}



void ai_move(char** board, char ch, int rows, int cols){
	int x = rand()%cols+1;


	// Make sure column isnt full
	if(board[0][x-1] != ' '){
		ai_move(board, ch, rows, cols);
		return;
	}

	cout << "Ai Placed chip at x:" << x << ", y:" << lowest_empty_row(board, ch, rows, cols, x-1) + 1 << endl;

	board[lowest_empty_row(board, ch, rows, cols, x-1)][x-1] = ch;

}

void p_move(char** board, char ch, int rows, int cols){
	// Handle input
	
	int x;
	cout << "What column would you like to drop your token in?, " << ch << "?: ";
	cin >> x;

	// If our placement is out of range, try again
	if(x > cols || x < 0){
		cout << "Out of range, try again." << endl;
		p_move(board, ch, rows, cols);
		return;
	}
	
	
	// If that column is full, try again
	if(board[0][x-1] != ' '){
		cout << "Column is full, try again." << endl;
		p_move(board, ch, rows, cols);
		return;
	}

	cout << "Placed chip at x:" << x << ", y:" << lowest_empty_row(board, ch, rows, cols, x-1) + 1 << endl;

	board[lowest_empty_row(board, ch, rows, cols, x-1)][x-1] = ch;

}

int lowest_empty_row(char** board, char ch, int rows, int cols, int x){

	// Find the lowest unnoccupied row
	int tick = -1;
	for(int i = 0; i < rows; i++){
		if(board[i][x] == ' ')
			tick++;
		else
			break;
	}
	
	return tick;

		

}

bool win_or_tie(char** board, int rows, int cols){
	bool tie = true;
	bool win = false;

	// Check for tie
	for(int i = 0; i < cols; i++){
		if(board[0][i] == ' ')
			tie = false;
	}



	if(horizontal_check(board, rows, cols) || vertical_check(board, rows, cols) || diagonal_check(board, rows, cols) || tie){
		cout << "Game over" << endl;
		return true;
	}


}


bool diagonal_check(char** board, int rows, int cols){
	int xCount = 0;
	int lastX = -1;
		

	int oCount = 0;
	int lastO = -1;

	int currentColumn = cols-4;

	for(int i = cols-4; i >= 0; i--){
		for(int j = 0; j > cols-1 - currentColumn; j++){

			if(xCount == 4){
				cout << "X wins!" << endl;
				return true;
			}
			if(oCount == 4){
				cout << "O wins!" << endl;
				return true;
			}


			if(board[j][currentColumn] == 'X' && j == lastX + 1){
				xCount++;
				lastX = j;

				if(j == lastO + 1){
					oCount = 0;

				}
			}
			else if(board[j][currentColumn] == 'O' && j == lastO + 1){
				oCount++;
				lastO = j;

				if(j == lastX + 1){
					xCount = 0;
				}
			
			}

			

			currentColumn++;
		
		}
		currentColumn = 0;
	
	
	}


}

bool vertical_check(char** board, int rows, cols){
	int xCount = 0;
	int lastX = -1;

	int oCount = 0;
	int lastO = -1;

	for(int i = 0; i < cols; i++){
		for(int j = 0; j < rows; j++){
			if(board[j][i] == 'X' && j == lastX + 1){
				xCount++;
				lastX = j;

				if(j == lastO + 1){
					oCount = 0;
				}
			}

			else if(board[j][i] == 'O' && i == lastO + 1){
				oCount++;
				lastO = j;
			
				if(j == lastX +1){
					xCount = 0;
				}
			}
			else if(j == lastO + 1)
				oCount = 0;
			else if(j == lastX + 1)
				xCount = 0;
		}
	}
}




bool horizontal_check(char** board, int rows, int cols){
	// Check for winner and print winner if so

	
	int xCount = 0;
	int lastX = -1;

	
	int oCount = 0;
	int lastO = -1;


	for(int j = 0; i < rows; j++){
		for(int i = 0; i < cols; i++){
			if(board[j][i] == 'X' && i == lastX + 1){
				xCount++;
				lastX = i;

				if(i == lastO + 1){
					oCount = 0;
				}
			}

			else if(board[j][i] == 'O' && i == lastO + 1){
				oCount++;
				lastO = i;
			
				if(i == lastX +1){
					xCount = 0;
				}
			}
			else if(i == lastO + 1)
				oCount = 0;
			else if(i == lastX + 1)
				xCount = 0;
		}
	}
	if(xCount == 4){
		// x wins
		cout << "X wins!" << endl;
		return true;
	}
	if(oCount == 4){
		// o wins
		cout << "O wins!" << endl;
	}
}


void play_multi(char** board, int players, int rows, int cols){
	do
	{
		p_move(board, 'X', rows, cols);

		if(!win_or_tie(board, rows, cols))
			p_move(board, 'O', rows, cols);
		else
			break;
		
	}
	while(!win_or_tie(board, rows, cols));

}

void get_dimensions(int &rows, int &columns){
	cout << "Enter rows: "; cin >> rows; 
	cout << "Enter columns: "; cin >> columns;

}

void delete_2darray(char** array, int len){
	for(int i = 0; i < len; i++){
		delete[] array[i];
	}
	delete[] array;
}


