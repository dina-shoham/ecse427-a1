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

/* *************************** Include ****************************** */
	#include "interpreter.h"
	#include "shellmemory.h"

/* ************************ Global Variables ************************ */
	const int MAX_USER_INPUT = 1000;

/* ********************* Functions Definitions ********************** */
	int parseInput(char ui[]);

/* ***************************** Main ******************************* */
	int main(int argc, char *argv[]) {
		printf("%s\n", "Shell version 1.1 Created January 2022");
		help();

		char prompt = '$';  				// Shell prompt
		char userInput[MAX_USER_INPUT];		// user's input stored here
		int errorCode = 0;					// zero means no error, default

		//init user input
		for (int i = 0; i < MAX_USER_INPUT; ++i)
			userInput[i] = '\0';
		
		//init shell memory
		mem_init();

		while(!feof(stdin)) 
		{							
			printf("%c ", prompt);
			fgets(userInput, MAX_USER_INPUT - 1, stdin);
			errorCode = parseInput(userInput);
			if (errorCode == -1) 
				exit(99);	
			memset(userInput, 0, sizeof(userInput));
		}
		return 0;
	}

/* *************************** Functions  *************************** */
	// Extract words from the input then call interpreter
	int parseInput(char ui[]){
		int a, b, w;						

		// Skip beginning white spaces
		for(a = 0; ui[a] == ' ' && a < MAX_USER_INPUT; ++a);

		char  tmp[200];
		char *words[100];

		int errorCode;

		// Parse and interpretate words
		w = 0;
		while(ui[a] != '\0' && a < MAX_USER_INPUT) 
		{
			for(b = 0; ui[a] != '\0' && ui[a] != ' ' && ui[a] != ';' && a < MAX_USER_INPUT; ++a, ++b)
				tmp[b] = ui[a];
		 
			tmp[b]   = '\0';
			words[w] = strdup(tmp);
			w++;
			
			// Check if there are more commands
			if (ui[a] == ';')
			{
				errorCode = interpreter(words, w);
				if (errorCode == -1)
					return -1;

				// Remove empty space
				while (ui[a + 1] == ' ')
					a++;
				w = 0;
			}

			// End of command
			if (ui[a] == '\0')
				break;
			a++;
		}
		return interpreter(words, w);
	}
