/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

#include "dank.h"
#include "y.tab.h"

void shell_init(){
	printf("%s ", "INITIALIZING SHELL...");
	command.atptr = &argtab;
	//char *path = getenv("PATH");
	//printf("\nPATH = %s", path);
}

void print_prompt(){
	printf("\n%s", "dankShell: ");
}

void init_scanner_and_parser(){

}

void understand_errors(){

}

int get_command(){
	init_scanner_and_parser();
	if( yyparse() ){

		understand_errors();
		return 1;
	}
	else{
		printf("\n%s ", "getting here!");
		return 1;
	}
	printf("\n%s ", "getting here blah!");
	return 1;
}

void execute_builtin(){
	switch( builtin ){
		case SETENV:
		printf("\n%s ", "getting to execute builtin");
		printf("\nCommand is: %s\n", command.comname);
		printf("\narg 1 is: %s\n", command.atptr->args[0]);
		//printf("\narg 2 is: %s\n", command.atptr->args[1]);
		exit( EXIT_SUCCESS );
		break;
		case PRINTENV:
		break;
		case UNSETENV:
		break;
		case CD:
		break;
		/*case ALIAS:
		break;
		case UNALIAS:
		break;*/
		case EXIT:
		break;
	}
}

void execute_command(){

}

void process_command(){
	if ( builtin ){
		printf("\n%s ", "getting to process builtin command");
		execute_builtin();
	}
	else{
		printf("\n%s ", "getting to process command");
		execute_command();
	}
}

void recover_from_errors(){

}

int main() {
	shell_init();
	while(1){
		print_prompt();
		int CMD = get_command();
		switch( CMD ){
			case 1:
				printf("\n%s ", "OK!");
				process_command();
				break;
			case 2:
				printf("\n%s ", "Error!");
				recover_from_errors();
				break;
			case 3:
				printf("\n%s\n", "Goodbye!");
				exit( EXIT_SUCCESS );
				break;
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
