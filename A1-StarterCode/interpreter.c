#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#include "shellmemory.h"
#include "shell.h"

// int MAX_ARGS_SIZE = 7;

int help();
int quit();
int badcommand();
int toomanytokens();
int notalphanumeric();
int set(char* args[], int args_size);
int print(char* var);
int run(char* script);
int echo(char* var);
int badcommandFileDoesNotExist();
int isAlphanumeric(char* string);

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1){
	// if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand();	
		else if (args_size > 7) return toomanytokens();
		return set(command_args, args_size);

	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size < 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else if (strcmp(command_args[0], "echo")==0) {
		// echo
		if (args_size != 2) return badcommand();
		return echo(command_args[1]);

	} else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

int toomanytokens(){
	printf("%s\n", "Bad command: Too many tokens");
}

int notalphanumeric(){
	printf("%s\n", "Bad argument: Not alphanumeric");
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int isAlphanumeric(char* string){
	int len = strlen(string);
	for (int i = 0; i < len; i++){
		if(!isalnum(string[i])) return 0;
	}
}

int set(char* args[], int args_size) {	
	char* var = args[1];
	char* val = args[2];
	char *space = " ";

	for (int i = 3; i < args_size; i++){
		strcat(val, space); 
		strcat(val, args[i]);
	}

	// printf("set %s to %s\n", var, val);
	mem_set_value(var, val);
	return 0;
}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);
	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}

int echo(char* str){
	if (str[0] == '$') { 
		str++;
		if (!isAlphanumeric(str)) return notalphanumeric();
		print(str);
	} else {
		if (!isAlphanumeric(str)) return notalphanumeric();
		printf("%s\n", str);
	}
	return 0;
}
