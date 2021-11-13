///
///     Aaron Frost
///     CS 344 Assignment 2
///     10/12/2021
///     frostaa@oregonstate.edu
/// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h> // For INT_MAX. TODO: use the max for __off_t instead
#include <errno.h>


#define FILE_NAME_PREFIX "movies_"
#define FILE_NAME_EXTENSION "csv"
#define PROCESS_DIR_PREFIX "frostaa.movies."
#define LINE_BUFFER_SIZE 2000


int get_valid_main_option_input();
int get_valid_process_file_option_input();
void give_user_process_file_options();

void process_largest();
void process_smallest();
void process_specific();

void process_file(const char* file_name);

struct list* list_create();
void read_file_populate_list(struct list* list, const char* file_name);
void list_insert(struct list* list, void* val);
void list_free(struct list* list);
void free_movies(struct list* list);
void list_insert(struct list* list, void* val);

const char* get_file_name_extension(const char *file_name);


struct movie
{
	char* title;
	int year;
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

int main(){
    srand(time(NULL));

    int option_input = get_valid_main_option_input();

    switch (option_input)
    {
    case 1:
        give_user_process_file_options();
        fflush(stdin);
        main();
        break;
    case 2:
        return 0;
    }

    return 0;
}

int get_valid_main_option_input(){
    printf("1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2: ");

    int input;

    scanf("%d", &input);
    fflush(stdin);

    if(input < 1 || input > 2){
        printf("\nInvalid input.\n");
        return get_valid_main_option_input();
    }

    return input;
}

void give_user_process_file_options(){
    int option_input = get_valid_process_file_option_input();
    switch (option_input)
    {
    case 1:
        process_largest();
        break;
    case 2:
        process_smallest();
        break;
    case 3:
        process_specific();
        break;
    }
}

int get_valid_process_file_option_input(){
    printf("\nWhat file do you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of the file\n\nEnter a choice from 1 to 3: ");

    int input;

    scanf("%d", &input);
    fflush(stdin);

    if(input < 1 || input > 3){
        printf("\nInvalid input.\n");
        return get_valid_process_file_option_input();
    }

    return input;
}

void process_largest(){
    DIR *current_directory = opendir(".");

    struct dirent *dir;

    __off_t largest_size = 0;
    char* largest_name = NULL;

    struct stat dir_stat;

    if(current_directory){
        // For each file in directory
        while((dir = readdir(current_directory)) != NULL){
            // If filename prefix and extension match
            if(strncmp(FILE_NAME_PREFIX, dir->d_name, strlen(FILE_NAME_PREFIX)) == 0 && strncmp(get_file_name_extension(dir->d_name), FILE_NAME_EXTENSION, strlen(FILE_NAME_EXTENSION)) == 0){
                // store metadata in dir_stat
                stat(dir->d_name, &dir_stat);
                if(dir_stat.st_size > largest_size){
                    largest_size = dir_stat.st_size;
                    largest_name = dir->d_name;
                }
            }
        }
        closedir(current_directory);
        process_file(largest_name);
    }
}

void process_smallest(){
    DIR *current_directory = opendir(".");

    struct dirent *dir;

    __off_t smallest_size = INT_MAX;
    char* smallest_name = NULL;

    struct stat dir_stat;

    if(current_directory){
        // For each file in directory
        while((dir = readdir(current_directory)) != NULL){
            // If filename prefix and extension match
            if(strncmp(FILE_NAME_PREFIX, dir->d_name, strlen(FILE_NAME_PREFIX)) == 0 && strncmp(get_file_name_extension(dir->d_name), FILE_NAME_EXTENSION, strlen(FILE_NAME_EXTENSION)) == 0){
                // store metadata in dir_stat
                stat(dir->d_name, &dir_stat);
                if(dir_stat.st_size < smallest_size){
                    smallest_size = dir_stat.st_size;
                    smallest_name = dir->d_name;
                }
            }
        }
        closedir(current_directory);
        process_file(smallest_name);
    }
}

void process_specific(){
    char input[255];

    printf("Enter the complete file name: ");
    scanf("%s", input);
    fflush(stdin);

    DIR *current_directory = opendir(".");

    struct dirent *dir;

    int file_exists = 0;

    if(current_directory){
        // For each file in directory
        while((dir = readdir(current_directory)) != NULL){
            // If filename prefix and extension match
            if(strncmp(dir->d_name, input, strlen(input)) == 0){
                file_exists = 1;
                break;
            }
        }
        closedir(current_directory);
    }

    if(file_exists == 1){
        process_file(input);
    } else{
        printf("The file %s was not found. Try again\n\n", input);
        give_user_process_file_options();
    }
}

const char* get_file_name_extension(const char *file_name){
    const char* dot = strrchr(file_name, '.');
    if(!dot || dot == file_name){
        return "";
    }
    return dot + 1;
}

void process_file(const char* file_name){
    printf("Now processing the chosen file named %s\n", file_name);

    char dir_name[255];
    sprintf(dir_name, "./%s%d", PROCESS_DIR_PREFIX, rand() % 100000);

    printf("Created directory with name %s\n\n", dir_name);

    mkdir(dir_name, 0750);

	struct list* movie_list = list_create();

	read_file_populate_list(movie_list, file_name);
    

    struct list* unique_years_list = list_create();

    struct link* movie_iterator = movie_list->head;
    struct link* unique_year_iterator = NULL;

    while(movie_iterator){
        unique_year_iterator = unique_years_list->head;

        struct movie* movie = (struct movie*)(movie_iterator->val);

        int is_year_already_added = 0;

        // Find out if the year of this movie has already been recorded
        while(unique_year_iterator){
            if(*((int*)unique_year_iterator->val) == movie->year){
                is_year_already_added = 1;
                break;
            }
            unique_year_iterator = unique_year_iterator->next;
        }

        // If the year has not been added, add it
        if(is_year_already_added == 0){
            list_insert(unique_years_list, &(movie->year));
        }

        movie_iterator = movie_iterator->next;
    }

    unique_year_iterator = unique_years_list->head;

    while(unique_year_iterator){
        int unique_year_value = *((int*)unique_year_iterator->val);

        FILE* file_pointer;

        char year_path[255];

        sprintf(year_path, "%s/%d.txt", dir_name, unique_year_value);

        file_pointer = fopen(year_path, "w");

        if(file_pointer){

            movie_iterator = movie_list->head;
       
            while(movie_iterator){
                struct movie* movie = (struct movie*)(movie_iterator->val);
                if(movie->year == unique_year_value){
                    fputs(movie->title, file_pointer);
                    fputc('\n', file_pointer);
                }
                movie_iterator = movie_iterator->next;
            }
            char change_permissions_parameter[255];
            sprintf(change_permissions_parameter, "%s", year_path);

            int c = chmod(change_permissions_parameter, 0640);

            if (c == -1) {
                printf("Could not set file permissions. errno: %d (%s)\n", errno, strerror(errno));
            }

            fclose(file_pointer);
        }else{
            printf("Failed to create file for year %d, errno: %d\n", unique_year_value, errno);
        }

        unique_year_iterator = unique_year_iterator->next;
    }

    list_free(unique_years_list);

	// Clean up
	free_movies(movie_list);
	list_free(movie_list);
}

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

		free(movie->title);
		free(movie);

		link = link->next;
	}

	return;
}

void read_file_populate_list(struct list* list, const char* file_name) {

    char file_path[510];

    sprintf(file_path, "./%s", file_name);

	FILE* fptr = fopen(file_path, "r");

    
    if (!fptr) {
        printf("Failed to open file %s, errno: %d\n", file_path, errno);
    }


	int num_lines = 0;

	char line_buffer[LINE_BUFFER_SIZE];


	// For each line
	while (fgets(line_buffer, sizeof(line_buffer), fptr))
	{
        fflush(stdout);
		num_lines++;

		if (num_lines == 1)
		{
			// Skip first line; it stores no data we need
			continue;
		}

		char* line_copy = strdup(line_buffer);
		char* mem_alloc_0 = line_copy;

		char* token;

		struct movie* movie = malloc(sizeof(struct movie));


		// Set title
		token = __strtok_r(line_copy, ",", &line_copy);
		movie->title = strdup(token);


		// Set year
		token = __strtok_r(NULL, ",", &line_copy);
		movie->year = atoi(token);


		// Add to linked list
		list_insert(list, movie);


		// Free allocations
		free(mem_alloc_0);
	}

	fclose(fptr);

	return;
}