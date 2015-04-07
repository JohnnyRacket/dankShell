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
	alias.used = 0;
}

void print_prompt(){
	printf(KGRN "\ndankShell:");
	printf(KBLK " ");
}

void understand_errors(){
	printf("There were errors");
}

int get_command(){
	if( yyparse() ){
		understand_errors();
		return 1;
	}
	else{
		if( builtin == EXIT ){
			return 2;
		}
		return 0;
	}
}

void list_aliases(){
	if( alias.used == 0 ){
		printf("No aliases to display.");
	}
	else{
		for(int i = 0; i < alias.used; ++i ){
			printf("\n%s %s", alias.alname[i], alias.alstr[i]);
		}
	}
}

void set_alias(){
	if (alias.used >= MAXALIAS){
		printf("\nToo many aliases already exist.");
	}
	else{
		int i = 0;
		while( i < alias.used && strcmp(command.atptr->args[0], alias.alname[i]) ){
			++i;
		}
		alias.alname[i] = command.atptr->args[0];
		alias.alstr[i] = command.atptr->args[1];
		if( i == alias.used ){
			++alias.used;
		}
	}
}

void remove_alias(){
	int i = 0;
	while( i < alias.used && strcmp(command.atptr->args[0], alias.alname[i]) ){
		++i;
	}
	if( i >= alias.used ){
		printf("Alias not found");
	}
	else{
		while( i < alias.used ){
			alias.alname[i] = alias.alname[i+1];
			alias.alstr[i] = alias.alstr[i+1];
			++i;
		}
		alias.alname[alias.used] = "";
		alias.alstr[alias.used--] = "";
	}
}

void execute_builtin(){
	switch( builtin ){
		case SETENV:
			setenv(command.atptr->args[0], command.atptr->args[1], 1);
			break;
		case PRINTENV:
			for(char **current = environ; *current; current++){
				puts(*current);
			}
			break;
		case UNSETENV:
			unsetenv(command.atptr->args[0]);
			break;
		case CD:
			break;
		case ALIAS:
			if(command.nargs == 0){
				list_aliases();
			}
			else{
				set_alias();
			}
			break;
		case UNALIAS:
			remove_alias();
			break;
		case EXIT:
			break;
	}
}

void execute_command(){
    int process = fork ();
    if (process > 0){
			wait ((int *) 0);
    }else if (process == 0){
   		command.atptr->args[0]= command.comname;
    	execvp( command.comname,command.atptr->args );
    	fprintf (stderr, "Can't execute \n");
    	exit (1);
    }else if(process == -1){
    	fprintf (stderr, "Can't fork!\n");
    	exit (2);
    }
}

void clear_args(){
	int i;
	for (i = 0; i < command.nargs+1; ++i)
	{
		command.atptr->args[i] = NULL;
	}
}

void process_command(){
	if ( builtin ){
		execute_builtin();
	}
	else{
		//this if statement is for testing pipes
		printf("calling : %s\n",command.comname );
		if(strcmp(command.comname,"pipe") == 0){
			printf("calling piped and sniped: %s\n",command.comname );
			piped_and_sniped();}
		else{
		execute_command();
		clear_args();
		}
	}
}
void piped_and_sniped(){
	  int des_p[2];
        if(pipe(des_p) == -1) {
          perror("Pipe failed");
          exit(1);
        }

        if(fork() == 0)        //first fork
        {
            close(1);          //closing stdout
            dup(des_p[1]);     //replacing stdout with pipe write 
            close(des_p[0]);   //closing pipe read
            close(des_p[1]);

            const char* prog1[] = { "ls", "-l", 0};
            execvp(prog1[0], prog1);
            perror("execvp of ls failed");
            exit(1);
        }

        if(fork() == 0)        //creating 2nd child
        {
            close(0);          //closing stdin
            dup(des_p[0]);     //replacing stdin with pipe read
            close(des_p[1]);   //closing pipe write
            close(des_p[0]);

            const char* prog2[] = { "wc", "-l", 0};
            execvp(prog2[0], prog2);
            perror("execvp of wc failed");
            exit(1);
        }

        close(des_p[0]);
        close(des_p[1]);
        wait(0);
        wait(0);

	
}

void recover_from_errors(){

}

int main( int argc, char* argv[] ) {
	shell_init();

	while(1){

		print_prompt();
		int CMD = get_command();
		switch( CMD ){
			case 0:
				process_command();
				break;
			case 1:
				recover_from_errors();
				break;
			case 2:
				printf("\n%s\n", "Goodbye!");
				exit( EXIT_SUCCESS );
				break;
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
