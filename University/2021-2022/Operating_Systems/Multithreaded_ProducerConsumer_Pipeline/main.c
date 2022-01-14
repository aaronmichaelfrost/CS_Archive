/*
*
*	Aaron Frost
*	CS344 - Operating Systems 1
*	Assignment 4 - Multi Threaded Producer Consumer Pipeline
*	11/13/2021
*	frostaa@oregonstate.edu
*
*/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

#define MAX_LINES 49
#define MAX_LINE_LENGTH 1000
#define TERMINATION_STR "STOP\n"
#define OUTPUT_INCREMENT 80

// Shared buffers 
// (Producer -> Consumer)

// Input -> Line Seperator
char buffer1[MAX_LINES][MAX_LINE_LENGTH];

// Line Seperator -> Plus Sign
char buffer2[MAX_LINES * MAX_LINE_LENGTH];

// Plus Sign -> Output
char buffer3[MAX_LINES * MAX_LINE_LENGTH];


// Number of items in buffers
int count_1 = 0;
int count_2 = 0;
int count_3 = 0;

// Index where input thread will place the next line
int producer_idx_1 = 0;
// Index where line seperator thread will parse the next line
int consumer_idx_1 = 0;
// Index where line seperator thread will place the next line
int producer_idx_2 = 0;
// Index where the plus sign thread will parse the next line
int consumer_idx_2 = 0;
// Index where the plus sign thread will place the next line
int producer_idx_3 = 0;
// Index where the output thread will output the next line
int consumer_idx_3 = 0;


// Mutex locks
// These are locked when threads need to add or remove data to buffers.
pthread_mutex_t buffer1_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer2_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer3_lock = PTHREAD_MUTEX_INITIALIZER;

// Condition variables
// These are signalled when the producer thread adds data to it's buffer.
pthread_cond_t buffer1_has_data = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer2_has_data = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer3_has_data = PTHREAD_COND_INITIALIZER;

// Set to 1 when the termination sequence is read by the input thread
int stop_flag = 0;

/// <summary>
/// Reads up to MAX_LINES lines of characters from stdin. 
/// Acts as a producer-only thread.
/// Outputs chars to buffer1 which this function shares with line_seperator_thread.
/// </summary>
void* input_thread(void* args) {

	char temp[1000];
	temp[999] = '\0';

	do {

		for (size_t i = 0; i < MAX_LINES; i++)
		{
			// Wait until there is an input line
			while (!fgets(temp, MAX_LINE_LENGTH, stdin)) {
				
			}

			// Take control of buffer1
			pthread_mutex_lock(&buffer1_lock);

			// Move input line to buffer
			strcpy(buffer1[producer_idx_1], temp);

			// Check for termination sequence
			if (strcmp(temp, TERMINATION_STR) == 0) {
				stop_flag = 1;
			}

			// Increment counter / move forward one element
			count_1++;
			producer_idx_1++;

			// Tell next thread buffer has data
			pthread_cond_signal(&buffer1_has_data);

			// Let go buffer1
			pthread_mutex_unlock(&buffer1_lock);

			if (stop_flag == 1) {
				return NULL;
			}
		}
	} while (!stop_flag);
}

/// <summary>
/// Reads input from buffer1 shared with input_thread.
/// Replaces newline characters with spaces and feeds output into a 1D char buffer shared with plus_sign_thread.
/// </summary>
void* line_seperator_thread(void* args) {

	do {

		// Take control of buffer1
		pthread_mutex_lock(&buffer1_lock);

		while (count_1 == 0) {
			// Wait for more data
			pthread_cond_wait(&buffer1_has_data, &buffer1_lock);
		}

		// Copy data from buffer for parsing
		char line_copy[MAX_LINE_LENGTH];
		strcpy(line_copy, buffer1[consumer_idx_1]);

		// Move forward one element
		consumer_idx_1++;
		count_1--;

		// Let go buffer1
		pthread_mutex_unlock(&buffer1_lock);


		// Take control of buffer2
		pthread_mutex_lock(&buffer2_lock);

		// Add characters from buffer1 to buffer2
		for (size_t i = 0; i < strlen(line_copy); i++)
		{
			count_2++;
			if (line_copy[i] == '\n') {
				line_copy[i] = ' ';
				buffer2[producer_idx_2++] = line_copy[i];
				break;
			}
			buffer2[producer_idx_2++] = line_copy[i];
		}

		buffer2[producer_idx_2] = '\0';

		// Tell next thread buffer has data
		pthread_cond_signal(&buffer2_has_data);

		// Let go buffer2
		pthread_mutex_unlock(&buffer2_lock);
	} while (!stop_flag || count_1 > 0);
}

/// <summary>
/// Reads input from buffer2 shared with line_seperator_thread.
/// Replaces "++" sequences with '^'.
/// Feeds output into to a 1D char buffer shared with output_thread.
/// </summary>
void* plus_sign_thread(void* args) {
	char temp[MAX_LINE_LENGTH];
	int temp_size = 0;
	do {
		// Take control of buffer2
		pthread_mutex_lock(&buffer2_lock);

		while (count_2 == 0) {
			// Wait for more data
			pthread_cond_wait(&buffer2_has_data, &buffer2_lock);
		}

		// Save this for later, this is the number of characters to add into buffer 3
		temp_size = count_2;

		for (size_t i = 0; i < count_2; i++)
		{
			temp[i] = buffer2[consumer_idx_2++];
		}

		count_2 = 0;

		// Let go buffer2
		pthread_mutex_unlock(&buffer2_lock);

		// Replace ++ with ^
		for (size_t i = 0; i < temp_size - 1; i++)
		{
			if (temp[i] == '+' && temp[i + 1] == '+') {
				temp[i] = '^';

				// Shift characters to the left
				for (size_t j = i + 1; j < temp_size - 1; j++)
				{
					temp[j] = temp[j + 1];
				}
				temp_size--;
			}
		}

		temp[temp_size] = '\0';

		// Take control of buffer3
		pthread_mutex_lock(&buffer3_lock);

		// Copy over the new line to buffer 3
		for (size_t i = 0; i < temp_size; i++)
		{
			buffer3[producer_idx_3++] = temp[i];
		}

		buffer3[producer_idx_3] = '\0';

		count_3 += temp_size;

		// Tell next thread buffer has data
		pthread_cond_signal(&buffer3_has_data);

		// Let go buffer3
		pthread_mutex_unlock(&buffer3_lock);
	} while (!stop_flag || count_2 > 0);
}

/// <summary>
/// Consumer thread for plus_sign thread.
/// This is the last thread in the chain, and it outputs the fully processed data to stdout.
/// Shares buffer3 and outputs whenever there are >= OUTPUT_INCREMENT chars in buffer3.
/// </summary>
void* output_thread(void* args) {

	do {

		// Take control of buffer3
		pthread_mutex_lock(&buffer3_lock);

		while (count_3 == 0) {
			// Wait for more data
			pthread_cond_wait(&buffer3_has_data, &buffer3_lock);
		}

		if (count_3 >= OUTPUT_INCREMENT) {
			// Output fully processed characters
			for (size_t i = 0; i < OUTPUT_INCREMENT; i++)
			{
				putc(buffer3[consumer_idx_3++], stdout);
			}
			// Output line seperator 
			putc('\n', stdout);

			count_3 -= OUTPUT_INCREMENT;
		}

		// Let go buffer2
		pthread_mutex_unlock(&buffer3_lock);

	} while (!stop_flag || count_3 >= OUTPUT_INCREMENT);
}

/// <summary>
/// Entry point.
/// Starts up all four threads and waits for them to all complete.
/// </summary>
int main() {

	// Make sure all lines in buffers are null terminated so we can use strcpy
	for (size_t i = 0; i < MAX_LINES; i++)
	{
		buffer1[i][MAX_LINE_LENGTH - 1] = '\0';
	}

	pthread_t input_thread_id;
	pthread_t line_seperator_thread_id;
	pthread_t plus_sign_thread_id;
	pthread_t output_thread_id;

	int result_code = 0;

	// Create four threads
	result_code = pthread_create(&input_thread_id, NULL, input_thread, NULL);
	assert(result_code == 0);

	result_code = pthread_create(&line_seperator_thread_id, NULL, line_seperator_thread, NULL);
	assert(result_code == 0);

	result_code = pthread_create(&plus_sign_thread_id, NULL, plus_sign_thread, NULL);
	assert(result_code == 0);

	result_code = pthread_create(&output_thread_id, NULL, output_thread, NULL);
	assert(result_code == 0);


	// Wait for threads to terminate
	result_code = pthread_join(input_thread_id, NULL);
	assert(result_code == 0);

	result_code = pthread_join(line_seperator_thread_id, NULL);
	assert(result_code == 0);

	result_code = pthread_join(plus_sign_thread_id, NULL);
	assert(result_code == 0);

	result_code = pthread_join(output_thread_id, NULL);
	assert(result_code == 0);

	return 0;
}