#include <stdio.h> // for fopen and fclose
#include <stdlib.h> // for atoi and itoa
#include <string.h> // for strdup

#define LINE_BUFFER_SIZE 1024 * 1024

#define LANGUAGE_STRING_MAX_LENGTH 20

#define MIN_YEAR 1900
#define MAX_YEAR 2021


/// <summary>
/// Removes a character from a string
/// </summary>
/// <param name="str">The output string</param>
/// <param name="c">The character to remove</param>
void remove_char(char* str, char c) {
	if (str == NULL) {
		return;
	}

	char* str_destination = str;

	while (*str) 
	{
		if (*str != c) {
			*str_destination++ = *str;
		}
		str++;
	}

	*str_destination = '\0';

	return;
}

struct movie
{
	char* title;
	int year;

	int num_languages;
	double rating;
	char** languages;
};

struct link 
{
	void* val;
	struct link* next;
};

struct list 
{
	struct link* head;
};

struct list* list_create() 
{
	struct list* l = malloc(sizeof(struct list));


	l->head = NULL;

	return l;
}

void list_insert(struct list* list, void* val) {

	struct link* temp = malloc(sizeof(struct link));

	temp->val = val;

	temp->next = list->head;

	list->head = temp;

	return;
}

void list_free(struct list* list) 
{
	struct link* temp;

	while (list->head) 
	{
		temp = list->head;

		list->head = list->head->next;

		free(temp);
	}

	free(list);

	return;
}

void free_movies(struct list* list) {

	struct link* link = list->head;


	while (link)
	{
		struct movie* movie = (struct movie*)link->val;

		for (int i = movie->num_languages - 1; i > -1; i--)
		{
			free(movie->languages[i]);
		}

		free(movie->languages);
		free(movie->title);
		free(movie);

		link = link->next;
	}

	return;
}

void print_movies(struct list* list) {

	printf("PRINTING MOVIES ------------\n");
	struct link* link = list->head;

	while (link) {
		struct movie movie = *((struct movie*)link->val);
		printf("Title: %s\n", movie.title);
		printf("Num languages: %d\n", movie.num_languages);
		for (size_t i = 0; i < movie.num_languages; i++)
		{
			printf("%s\n", movie.languages[i]);
		}
		printf("Rating: %.1f\n", movie.rating);
		link = link->next;
		printf("- - - - - - - - - - - - \n");
	}
}

void read_file_populate_list(struct list* list, char* file_name, int* num_movies) {
	FILE* fptr;
	fptr = fopen(file_name, "r");

	int num_lines = 0;


	char line_buffer[LINE_BUFFER_SIZE];

	// For each line
	while (fgets(line_buffer, sizeof(line_buffer), fptr))
	{
		num_lines++;

		if (num_lines == 1)
		{
			// Skip first line; it stores no data we need
			continue;
		}

		char* line_copy = strdup(line_buffer);
		char* mem_alloc_0 = line_copy;

		char* token;
		char* token_dup;

		struct movie* movie = malloc(sizeof(struct movie));


		// Set title
		token = __strtok_r(line_copy, ",", &line_copy);
		movie->title = strdup(token);


		// Set year
		token = __strtok_r(NULL, ",", &line_copy);
		movie->year = atoi(token);


		char* line_copy2 = strdup(line_copy);
		char* mem_alloc_1 = line_copy2;


		// Get language count 
		token_dup = strdup(token);
		char* mem_alloc_3 = token_dup;

		token = __strtok_r(NULL, ",", &line_copy);

		remove_char(token, '[');

		int num_commas = 0;
		for (int i = 0; i < strlen(token); i++)
		{
			if (token[i] == ';')
			{
				num_commas++;
			}
		}

		int num_languages = num_commas + 1;
		movie->num_languages = num_languages;


		// Set languages
		movie->languages = malloc(num_languages * sizeof(char*));

		int i = 0;

		for (; i < num_languages; i++)
		{
			char ch[LANGUAGE_STRING_MAX_LENGTH];
			movie->languages[i] = ch;

			char* delimiter;

			if (i == num_languages - 1) {

				delimiter = "]";
			}
			else
			{
				delimiter = ";";
			}

			token_dup = __strtok_r(NULL, delimiter, &line_copy2);

			if (i == 0) {
				remove_char(token_dup, '[');
			}

			movie->languages[i] = strdup(token_dup);
		}

		// Set movie rating
		token = __strtok_r(NULL, "\n", &line_copy);

		char* temp;
		movie->rating = strtod(token, &temp);


		// Add to linked list
		list_insert(list, movie);


		// Free allocations
		free(mem_alloc_0);
		free(mem_alloc_1);
		free(mem_alloc_3);
	}


	//print_movies(list);
	*num_movies = num_lines - 1;

	fclose(fptr);

	return;
}

int get_option_input(int low_inclusive, int high_inclusive) {
	printf("1. Show movies released in the specified year\n2. Show highest rated movie for each year\n3. Show the title and year of release of all movies in a specific language\n4. Exit from the program\n\n");

	printf("Enter a choice from 1 to 4: ");


	int input, status, temp;

	status = scanf("%d", &input);
	while (status != 1) {
		while ((temp = getchar()) != EOF && temp != '\n');

		printf("You entered an incorrect choice. Try again.\n\n");

		return get_option_input(low_inclusive, high_inclusive);
	}

	if (input < low_inclusive || input > high_inclusive) {
		printf("You entered an incorrect choice. Try again.\n\n");

		return get_option_input(low_inclusive, high_inclusive);
	}

	return input;
}

int get_year_input(int low_inclusive, int high_inclusive) {
	
	printf("Enter the year for which you want to see movies: ");
	int input, status, temp;

	status = scanf("%d", &input);
	while (status != 1) {
		while ((temp = getchar()) != EOF && temp != '\n');

		printf("You entered an incorrect choice. Try again.\n\n");

		return get_year_input(low_inclusive, high_inclusive);
	}

	if (input < low_inclusive || input > high_inclusive) {
		printf("You entered an incorrect choice. Try again.\n\n");

		return get_year_input(low_inclusive, high_inclusive);
	}

	return input;
}

void show_by_year(struct list* list) {
	int year_input = get_year_input(MIN_YEAR, MAX_YEAR);

	struct link* link = list->head;

	int num_matches = 0;

	while (link) {
		struct movie* movie = (struct movie*)link->val;

		if (movie->year == year_input) {
			num_matches++;
			printf("%s\n", movie->title);
		}
		link = link->next;
	}

	if (num_matches == 0) {
		printf("No data about movies released in the year %d\n", year_input);
	}

	printf("\n");
}

void show_highest_rating_by_year(struct list* movie_list) {
	
	struct list* years_list = list_create();

	struct link* search_link = years_list->head;

	struct link* link = movie_list->head;


	// First record all of the years
	while (link) 
	{
		struct movie* movie = (struct movie*)link->val;

		int is_year_added = 0;

		search_link = years_list->head;

		while (search_link) 
		{
			if (*((int*)search_link->val) == movie->year) 
			{
				is_year_added = 1;
				break;
			}
			search_link = search_link->next;
		}

		if (is_year_added == 0) 
		{
			list_insert(years_list, &(movie->year));
		}

		link = link->next;
	}


	link = years_list->head;

	// For each year
	while (link) {

		int year = *((int*)link->val);

		double current_highest_rating = 0;

		struct movie* highest_rating_movie;

		search_link = movie_list->head;

		// For each movie in that year
		while (search_link)
		{
			struct movie* movie = (struct movie*)search_link->val;

			if (movie->year == year) 
			{
				// Compare rating 
				if (movie->rating > current_highest_rating) {
					highest_rating_movie = movie;
					current_highest_rating = movie->rating;
				}
			}
			search_link = search_link->next;
		}

		printf("%d %.1f %s\n", year, highest_rating_movie->rating, highest_rating_movie->title);

		link = link->next;
	}

	list_free(years_list);

	printf("\n");

	return;
}



void show_by_select_language(struct list* list) 
{
	char input[LANGUAGE_STRING_MAX_LENGTH];

	printf("Enter the language for which you want to see movies: ");
	scanf("%s", input);

	struct link* link = list->head;

	int num_language_matches = 0;

	while (link) {
		struct movie* movie = (struct movie*)link->val;
		for (int i = 0; i < movie->num_languages; i++)
		{
			if (!strcmp(movie->languages[i], input))
			{
				num_language_matches++;
				printf("%d %s\n", movie->year, movie->title);
			}
		}
		link = link->next;
	}

	if (num_language_matches == 0) {
		printf("No data about movies released in %s\n", input);
	}

	printf("\n");

	return;
}

void do_main_loop(struct list* list) {
	int input = get_option_input(1, 4);

	switch (input)
	{
	case 1:
		show_by_year(list);
		break;
	case 2:
		show_highest_rating_by_year(list);
		break;
	case 3:
		show_by_select_language(list);
		break;
	case 4:
		return;
		break;
	default:
		break;
	}

	do_main_loop(list);
}

int main(char* argc, char** argv) 
{
	// Create a linked-list of movies
	struct list* list;
	list = list_create();

	int num_movies;

	char* file_name = argv[1];

	read_file_populate_list(list, file_name, &num_movies);

	//print_movies(list);

	printf("Processed file %s and parsed data for %d movies\n\n", file_name, num_movies);

	do_main_loop(list);


	// Clean up
	free_movies(list);
	list_free(list);
	

	return 0;
}