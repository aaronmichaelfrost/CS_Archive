/*
*	Aaron Frost
*	Small Shell - CS344 Program 3
*	10/28/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 2048
#define MAX_ARGUMENTS 512

static int foreground_only;


// Integer filled linked-list for storing background pids
struct link
{
	int val;
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

void list_insert(struct list* list, int val) {
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

void list_remove(struct list* list, int val) {
	struct link* temp = list->head;
	struct link* prev;
	while (temp) {
		if (val == temp->val) { // If elements are the same
			if (prev)
				prev->next = temp->next; // If item to remove is not the head, simply fill in the gap by "skipping over" the element to remove
			else
				list->head = temp->next; // Item to remove is the head. Move the head forward
			free(temp); // Delete the element to remove
			break;
		}
		// Move pointers forward
		prev = temp;
		temp = temp->next;
	}
	return;
}

// Object for abstracting user commands
struct command_line {
	int arguments_count;	// The number of arguments for the actual command itsself. Note: does not include < > or &
	char** arguments;		// The arguments for the command to execute. Including the command itsself, excluding redirection and other unrelated characters
	char* input_file;		// Input file for redirection specified after '<' character is read
	char* output_file;		// Output file for redirection specified after '>' character is read
	int is_background;		// Was there an '&' character at the end of the command?
};

/// <summary>
/// Prints the exit status or termination signal of a process to stdout. 
/// </summary>
/// <param name="status">This variable should be set before calling this function by passing it into waitpid()</param>
void show_status(int status) {
	if (WIFEXITED(status)) {
		// Print the exit status
		printf("exit value %d\n", WEXITSTATUS(status));
		fflush(stdout);
	}
	else if (WIFSIGNALED(status)) {
		// Print the termination signal
		printf("terminated by signal %d\n", WTERMSIG(status));
		fflush(stdout);
	}
}

/// <summary>
/// Seperates an input string using a set of delimiters
/// </summary>
/// <param name="input">The string to split</param>
/// <param name="delimiters">A string which is a concatenation of the seperation characters to split with</param>
/// <param name="output">The array of split strings</param>
/// <param name="num_arguments">The number of strings in the output</param>
void split_string(char* input, const char* delimiters, char** output, int* num_arguments) {
	char* token = __strtok_r(input, delimiters, &input);
	int counter = 0;
	while (token) {
		output[counter++] = token;
		token = __strtok_r(NULL, delimiters, &input);
	}
	*num_arguments = counter;
}

/// <summary>
/// Expands all instances of "$$" to the current process's pid. 
/// WARNING: Expansion will not extend the buffer size limit
/// </summary>
/// <param name="str">The string with variables to expand</param>
void expand_variables(char* str) {
	char format_string[BUFFER_SIZE];
	strcpy(format_string, str);

	int iterations = strlen(str) - 1;
	for (int i = 0; i < iterations; i++) {

		// "%1$d is the formatting that expands to the first integer variable for sprintf
		if (format_string[i] == '$' && format_string[i + 1] == '$') {

			format_string[i] = '%';
			format_string[i + 1] = '1';

			// Shift to right by two elements
			for (size_t j = iterations - 1; j > i + 1; j--)
			{
				format_string[j + 2] = format_string[j];
			}
			format_string[iterations + 2] = '\0';

			format_string[i + 2] = '$';
			format_string[i + 3] = 'd';

			iterations += 2;

			if (iterations > BUFFER_SIZE) {
				printf("smallsh: ERROR: cannot perform variable expansion which extends input string past 2048 characters\n");
				fflush(stdout);
				return;
			}
		}
	}

	snprintf(str, BUFFER_SIZE, format_string, getpid());
}

/// <summary>
/// Executes the user's command. Only call this if there is at least one argument in argv
/// </summary>
/// <param name="argc">The number of words in the users entire input, aka length of argv</param>
/// <param name="argv">The split words from the user's input</param>
/// <param name="list">Storage for background pid's</param>
void execute_user_command(int argc, char** argv, struct list* list, int* status) {

	// Create a command_line object by parsing the user input
	struct command_line line;

	char* cmd_args[513];
	line.arguments = cmd_args;
	line.input_file = NULL;
	line.output_file = NULL;
	line.is_background = 0;

	// Initialize count to one.
	line.arguments_count = 1;

	line.arguments[0] = argv[0];

	for (int i = 1; i < argc; i++)
	{
		if (i < argc - 1) {
			// Parse for input redirection
			if (strcmp(argv[i], "<") == 0) {
				line.input_file = argv[i + 1];
				i++;
				continue;
			}

			// Parse for output redirection
			else if (strcmp(argv[i], ">") == 0) {
				line.output_file = argv[i + 1];
				i++;
				continue;
			}
		}

		// Last argument denotes if the process should run in the backgroud or foreground
		else if (strcmp(argv[i], "&") == 0) {
			if (!foreground_only) {
				line.is_background = 1;
			}
			break;
		}

		// Populate arguments to pass with execvp
		line.arguments[line.arguments_count++] = argv[i];
	}

	// execvp expects last argument to be null
	line.arguments[line.arguments_count] = NULL;

	// Fork a new process because calling execvp will kill this current process
	pid_t spawn_pid = fork();

	switch (spawn_pid) {
		case -1:
			perror("Hull Breach!\n");
			exit(EXIT_FAILURE);
			break;

		case 0:
			//// CHILD BRANCH
			
			if (line.is_background) {
				// Ignore CTRL C signal (SIGINT) 
				signal(SIGINT, SIG_IGN);
				// Ignore CTRL Z signal (SIGTSTP)
				signal(SIGTSTP, SIG_IGN);
			}
			else {
				// Terminate with CTRL C signal (SIGINT)
				signal(SIGINT, SIG_DFL);
				// Ignore CTRL Z signal (SIGTSTP)
				signal(SIGTSTP, SIG_IGN);
			}

			// IO redirection

			if (line.input_file) {

				// Try to redirect stdin (file descriptor 0)
				int input_file_descriptor = open(line.input_file, O_RDONLY);
				if (input_file_descriptor == -1 
					|| dup2(input_file_descriptor, 0) == -1) {
					printf("smallsh: cannot open %s for input\n", line.input_file);
					fflush(stdout);
					exit(EXIT_FAILURE);
				}
			}
			else if(line.is_background) {

				// When no input file specified for background process, redirect to /dev/null
				int input_file_descriptor = open("/dev/null", O_RDONLY);
				if (input_file_descriptor == -1
					|| dup2(input_file_descriptor, 0) == -1) {
					printf("smallsh: cannot open /dev/null for input\n");
					fflush(stdout);
					exit(EXIT_FAILURE);
				}
			}

			if (line.output_file) {

				// Try to redirect stdout (file descriptor 1)
				int output_file_descriptor = open(line.output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (output_file_descriptor == -1
					|| dup2(output_file_descriptor, 1) == -1) {
					printf("smallsh: cannot open %s for output\n", line.output_file);
					fflush(stdout);
					exit(EXIT_FAILURE);
				}
			}
			else if (line.is_background) {

				// When no output file specified for background process, redirect to /dev/null
				int output_file_descriptor = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (output_file_descriptor == -1
					|| dup2(output_file_descriptor, 0) == -1) {
					printf("smallsh: cannot open /dev/null for output\n");
					fflush(stdout);
					exit(EXIT_FAILURE);
				}
			}

			// Create the new process and discontinue this code execution
			execvp(line.arguments[0], line.arguments);
			
			printf("%s: no such file or directory\n", line.arguments[0]);
			fflush(stdout);
			exit(EXIT_FAILURE);

			break;

		default:
			//// PARENT BRANCH

			

			if (line.is_background) {
				printf("background pid is %d\n", spawn_pid);

				// Track the background process
				list_insert(list, spawn_pid);
			}
			else {
				// Child is a foreground process so execution must halt until it finishes
				int child_process_id = waitpid(spawn_pid, status, 0);
				while (child_process_id > 0) {
					child_process_id = waitpid(spawn_pid, status, 0);
				}
			}
			break;
	}
}

/// <summary>
/// Handler function / callback for CTRL Z / SIGTSTP. Toggles foreground only mode
/// </summary>
void toggle_foreground_only(int signum) {
	// Toggle foreground-only mode
	// Here we use write instead of printf because this sort of callback is safer with functions that are interruptible

	if (foreground_only) {
		char* message = "Exiting foreground-only mode\n";
		write(1, message, 30);
		foreground_only = 0;
	}
	else {
		char* message = "Entering foreground-only mode (& is now ignored)\n";
		write(1, message, 50);
		foreground_only = 1;
	}
	
	fflush(stdout);
}

void parent_handle_sigchld() {
	int exit_status;
	
	int child_process_id = waitpid(-1, &exit_status, WNOHANG);
	if (child_process_id > 0) {
		printf("child pid %d ", child_process_id);
		show_status(exit_status);
	}
}

int main() {

	struct list* list = list_create();

	// Parent process (the shell) must ignore CTRL C / SIGINT
	signal(SIGINT, SIG_IGN);

	signal(SIGCHLD, parent_handle_sigchld);

	// Custom callback for CTRL Z
	signal(SIGTSTP, toggle_foreground_only);

	int exit_status = 0;
	int pid = getpid();
	int child_process_id = 0;
	int child_process_status = 0;
	foreground_only = 0;

	// Main shell loop
	while (1) {
		// Cleanup completed background processes
		struct link* link = list->head;
		while (link) {
			int child_process_id = waitpid(link->val, &exit_status, WNOHANG);
			if (child_process_id > 0) {
				printf("background process %d is done ", child_process_id);
				fflush(stdout);
				show_status(exit_status);
				list_remove(list, child_process_id);
			}
			link = link->next;
		}

		// Display prompt
		printf(": ");
		fflush(stdout);

		// User input
		size_t bufsize = BUFFER_SIZE;
		char buffer[bufsize];
		char* b = buffer;
		getline(&b, &bufsize, stdin);

		// Expand all instanced of "$$" to the current process pid
		expand_variables(b);

		// Split string and store result in args
		int arg_count = 0;
		char* args[MAX_ARGUMENTS];
		split_string(b, " \n", args, &arg_count);

		// Ignore comments / empty line
		if (arg_count == 0 || args[0][0] == '#') {
			continue;
		}

		// Built-in commands
		if (strcmp(args[0], "cd") == 0) {

			//Try to change directory
			if (arg_count > 1) {
				if (chdir(args[1]) == -1) {
					printf("Directory null.\n");
					fflush(stdout);
				}
			}
			else {
				// Use HOME environment variable as path if none specified
				chdir(getenv("HOME"));
				continue;
			}
		}
		else if (strcmp(args[0], "status") == 0) {
			show_status(exit_status);
		}
		else if (strcmp(args[0], "exit") == 0) {
			break;
		}
		// User commands
		else {

			execute_user_command(arg_count, args, list, &exit_status);
		}
	}

	// Exit shell

	// Kill all zombie processes
	struct link* link = list->head;
	while (link) {
		kill(link->val, SIGKILL);
		link = link->next;
	}

	free(list);

	return 0;
}