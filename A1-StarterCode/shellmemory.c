/* ************************** Project Info ************************** *
 * Authors:       Dina Shoham / Sepehr Moalemi                        *
 * Student IDs:   260823582   / 260791983                             *
 * Project:       Comp 310 A1                                         *
 * Purpose:       Building an OS Shell                                *
 * Collaboration: "No Collaborators"                                  *
 * ****************************************************************** */

/* *************************** Libraries **************************** */
	#include<stdlib.h>
	#include<string.h>
	#include<stdio.h>

/* **************************** Macros ****************************** */
	#define SHELLSIZE 1000
	#define FOR_EACH for(int i = 0; i < SHELLSIZE; ++i)

/* ************************ Struct/typedef ************************** */
	typedef enum { false, true } bool;
	struct memory_struct{
		char *var;
		char *value;
	};

/* ************************ Global Variables ************************ */
	struct memory_struct shellmemory[SHELLSIZE];

/* *********************** Helper Functions ************************* */
	int match(char *model, char *var){
		int len = strlen(var);
		int matchCount = 0;
		for(int i = 0; i < len; ++i)
			if (*(model + i) == *(var + i)) 
				matchCount++;
		if (matchCount == len)
			return 1;
		return 0;
	}

	char *extract(char *model){
		char token = '=';    
		char value[SHELLSIZE];    
		int i,j, len=strlen(model);

		// Loop till we reach an "="
		for(i = 0; i < len && *(model + i)!= token; ++i); 
		for(i = i + 1, j = 0; i < len ; ++i, ++j) 
			value[j] = *(model + i);

		value[j] = '\0';
		return strdup(value);
	}

/* ********************** My Helper Functions *********************** */
	int isEqualto(char *str1, char *str2){
		if (strcmp(str1, str2) == 0)
			return 1;
		return 0;
	}	

/* ************************* Constructor **************************** */
	void mem_init(){
		FOR_EACH
		{		
			shellmemory[i].var   = "none";
			shellmemory[i].value = "none";
		}
	}

/* ************************* Set Methods **************************** */
	void mem_set_value(char *var_in, char *value_in){
		FOR_EACH
			if (isEqualto(shellmemory[i].var, var_in))
			{
				shellmemory[i].value = strdup(value_in);
				return;
			} 

		//Value does not exist, need to find a free spot
		FOR_EACH
			if (isEqualto(shellmemory[i].var, "none"))
			{
				shellmemory[i].var   = strdup(var_in);
				shellmemory[i].value = strdup(value_in);
				return;
			} 
		return;
	}

	char *mem_get_value(char *var_in){
		FOR_EACH
			if ( shellmemory[i].var != NULL && isEqualto(shellmemory[i].var, var_in))
				return strdup(shellmemory[i].value);
		return ""; // Modified to adjust for echo(char* var)
	}
