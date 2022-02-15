/* ************************** Project Info ************************** *
 * Authors:       Dina Shoham / Sepehr Moalemi                        *
 * Student IDs:   260823582   / 260791983                             *
 * Project:       Comp 310 A1                                         *
 * Purpose:       Building an OS Shell                                *
 * Collaboration: "No Collaborators"                                  *
 * ****************************************************************** */

/* *************************** Libraries **************************** */
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h> 

	#include <dirent.h>				// for my_ls

/* *************************** Includes ***************************** */
	#include "shellmemory.h"
	#include "shell.h"

/* ************************ Global Variables ************************ */
	const int MAX_ARGS_SIZE = 7;

	const int MAX_HELP_ARGS  = 1;			// command
	const int MAX_QUIT_ARGS  = 1;			// command
	const int MAX_SET_ARGS   = 7;           // command + var + (1-5)val
	const int MAX_PRINT_ARGS = 2;			// command + var
	const int MAX_RUN_ARGS   = 2;			// command + file 
	const int MAX_ECHO_ARGS  = 2;			// command + var/string 
	const int MAX_MY_LS_ARGS = 1;			// command

	const int MAX_TOKEN_LENGTH = 100;	
	const int MAX_CHAR_READ    = 999;

	const int MAX_NUM_FILES = 100;

/* ********************* Functions Definitions ********************** */
	int help();
	int quit();
	int set(char* command_args[], int args_size);
	int print(char* var);
	int run(char* script);
	int echo(char* var);
	int my_ls();

	int stringComp(const void *str1, const void *str2);
	int badcommand(char* errorType);

/* ************************* Interpreter **************************** */
	int interpreter(char* command_args[], int args_size){
		// Check for end of file (ASCII < 33 are not symbols)
		if (command_args == NULL || *command_args[0] < 33)
			return badcommand("EndOfFile");

		if (args_size < 1)
			return badcommand("NoCommand");

		if (args_size > MAX_ARGS_SIZE)
			return badcommand("argumentError");

		// Remove space/empty line
		for (int i = 0; i < args_size; ++i)
			command_args[i][strcspn(command_args[i], "\r\n")] = 0;

		// Extract inputs
		char* commandName = command_args[0];

		if (isEqualto(commandName, "help") && args_size == MAX_HELP_ARGS)
		    return help();

		if (isEqualto(commandName, "quit") && args_size == MAX_QUIT_ARGS) 
			return quit();

		if (isEqualto(commandName, "set") && args_size <= MAX_SET_ARGS)
			return set(command_args, args_size);

		if (isEqualto(commandName, "print") && args_size == MAX_PRINT_ARGS) 
			return print(command_args[1]);

		if (isEqualto(commandName, "run") && args_size == MAX_RUN_ARGS) 
			return run(command_args[1]);

		if (isEqualto(commandName, "echo") && args_size == MAX_ECHO_ARGS) 
			return echo(command_args[1]);		

		if (isEqualto(commandName, "my_ls") && args_size == MAX_MY_LS_ARGS) 
			return my_ls();

		// Defualt:
		return badcommand("Undefined");
	}

/* ************************* Functions  **************************** */
	int help(){
		char help_string[] = 
			"-------------------------------------------------------------\n"
		    "COMMAND		    		DESCRIPTION\n"
			"help				Displays all the commands\n"
			"quit				Exits / terminates the shell with “Bye!”\n"
			"set VAR STRING			Assigns a value to shell memory\n" 
			"print VAR			Displays the STRING assigned to VAR\n"
			"run SCRIPT.TXT			Executes the file SCRIPT.TXT\n"
			"-------------------------------------------------------------\n";
		printf("%s\n", help_string);
		return 0;
	}

	int quit(){
		printf("%s\n", "Bye!");
		exit(0);
	}

	int set(char* command_args[], int args_size){
		char *link = " ";
		char  buffer[MAX_TOKEN_LENGTH * args_size + args_size];

		// Add the first element
		strcpy(buffer, command_args[2]);

		// Add the rest if available
		for (int i = 3; i < args_size; ++i)
		{
			strcat(buffer, link);
			strcat(buffer, command_args[i]);
		}

		mem_set_value(command_args[1], buffer);
		return 0;
	}

	int print(char* var){
		printf("%s\n", mem_get_value(var)); 
		return 0;
	}

	int run(char* script){
		int errCode = 0;
		char line[MAX_CHAR_READ + 1];
		FILE *p = fopen(script,"rt");  

		if(p == NULL)
			return badcommand("NoFile");

		fgets(line, MAX_CHAR_READ, p);
		while(!feof(p) && strlen(line) != 0)
		{
			errCode = parseInput(line);	// which calls interpreter()
			if (errCode == -2)
				break;
			memset(line, 0, sizeof(line));
			fgets(line,MAX_CHAR_READ,p);
		}
	    fclose(p);
		return errCode;
	}

	int echo(char* var){
		// Check if the argument is a variable ($VarNAME)
		// Just print the argument if its a string
		if (strcspn(var, "$") == 0)
			print(++var); 				// Drop "$" from pointer
		else
			printf("%s\n", var);
		return 0;
	}

	int my_ls(){
		// Storage
		char *files[MAX_NUM_FILES];
		int numFiles = 0;

		// No input for current dir
	    DIR *d;
	    struct dirent *dir;
	    d = opendir(".");
	    if (d != NULL)
	    {
	        while ((dir = readdir(d)) != NULL)
	        	if(dir->d_type == DT_REG || (dir->d_type == DT_DIR))
					if(!isEqualto(dir->d_name, ".") && !isEqualto(dir->d_name, ".."))
					{
			            files[numFiles] = dir->d_name;
			            numFiles++;
					}
	        closedir(d);
	    }

	    // Sort Alphabetically
	    qsort(files, numFiles, sizeof *files, stringComp);
	    for (int i = 0; i < numFiles; ++i)
	    	printf("%s\n",files[i]);
	    return 1;
	}

/* ********************** Helper Functions  *********************** */
	int stringComp(const void *str1, const void *str2){
	    return strcmp(*((char**) str1), *((char**) str2));
	}

/* ********************** Error Handeling  ************************ */
	int badcommand(char* errorType){
		if (isEqualto(errorType, "Undefined"))
		{
			printf("%s\n", "Unknown Command");
			return 1;
		}

		if (isEqualto(errorType, "argumentError"))
		{
			printf("%s\n", "Bad command: Too many tokens");
			return 3;
		}

		if (isEqualto(errorType, "NoFile"))
		{
			printf("%s\n", "Bad command: File not found");
			return 3;
		}
		if (isEqualto(errorType, "EndOfFile"))
			return -2; //terminate program

		printf("%s\n", errorType);
		return 1;
	}
