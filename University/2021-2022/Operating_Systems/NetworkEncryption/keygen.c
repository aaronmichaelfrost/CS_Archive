///
///		keygen.c
///		Random key generator. Not cryptographically safe.
///		Ouputs random characters from A-Z and spaces (followed by a newline character) to stdout
///		User must supply the keylength as a command line argument
/// 
///		Aaron Frost - 11/26/2021
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {

	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments %d expected %d\n", argc, 2);
		exit(1);
	}

	int keylength = atoi(argv[1]);

	time_t t;
	srand((unsigned) time(&t));

	for (size_t i = 0; i < keylength; i++)
	{
		int random = rand() % 27 + 65;

		if (random == 65 + 26) {
			// Include space characters
			random = 32;
		}

		char random_char = random;
		// write a random character between 65 and 90 inclusive to stdout
		printf("%c", random_char);
	}

	// write newline character to stdout
	printf("\n");
}