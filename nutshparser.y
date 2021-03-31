/* -*- indented-text -*- */

%{
#include "global.h"

extern void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 };

extern int yylex();
int yyparse();

<<<<<<< Updated upstream
%}
=======
%{
extern "C" int yylex();

#define yylex yylex
#include <stdio.h>
#include <string>
#include <cstring>
#include <regex.h>
#include <regex>
#include <dirent.h>
#include "command.h"

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}
>>>>>>> Stashed changes

%union {
       char *string;
}

<<<<<<< Updated upstream
%token <string> WORD BYE SETENV
%token NOTOKEN GREAT NEWLINE GREATGREAT PIPE AMPERSAND LESS GREATGREATAMPERSAND GREATAMPERSAND
=======
void expandWildCards(char * arg){
	if(strchr(arg, '.') == NULL && strchr(arg, '.')){
		Command::_currentSimpleCommand->insertArgument(arg);
		return;
	}

	//Converting wild cards
	//convert * -> .*
	//? -> .
	//. -> \.
	char * reg = (char*)malloc(2*strlen(arg)+10);
	char * a = arg;
	char * r = reg;
	*r = '^';
	r++; //get the start of the command
	while(*a) {
		if (*a == '*'){
			*r='.'; 
			r++;
			*r='*';
			r++;
		}
		else if (*a == '?'){
			*r='.';
			r++;
		}
		else if(*a == '.'){
			*r='\\';
			r++;
			*r='.';
			r++;
		}
		else{
			*r=*a;
			r++;
		}
		a++;
	}
	*r = '$';
	r++;
	*r = '\0';
	//get end of line and add null char
	regex_t regex;
	int retvalue = regcomp(&regex, reg, 0);
	if(retvalue != 0){
		perror("refcomp");
		return;
	}

	DIR * dir = opendir(".");
	if (dir == NULL){
		perror("oopendir");
		return;
	}
	struct dirent * ent;
	while((ent = readdir(dir)) != NULL){
		if(regexec(&regex, ent->d_name, 0, NULL, 0) == 0){
			Command::_currentSimpleCommand->insertArgument(strdup(ent->d_name));
		}
	}
	closedir(dir);
}

%}
>>>>>>> Stashed changes


%%

exit:
        %empty
        | BYE NEWLINE{return BYE;}
        ;

<<<<<<< Updated upstream
setenv:
        %empty
        | SETENV WORD WORD NEWLINE{
                printf('%s', $2);
                printf('%s', $3);
                return SETENV; }

// objects:
//         %empty
//         | objects object { }
//         ;

// object:
//         WORD {printf("input found");}
//          ; 

// goal:	
// 	commands
// 	;

// commands: 
// 	command
// 	| commands command 
// 	;

// command: simple_command
//         ;

// simple_command:	
// 	command_and_args iomodifier_opt NEWLINE {
// 		printf("   Yacc: Execute command\n");
// 		Command::_currentCommand.execute();
// 	}
// 	| NEWLINE 
// 	| error NEWLINE { yyerrok; }
// 	;

// command_and_args:
// 	command_word arg_list {
// 		Command::_currentCommand.
// 			insertSimpleCommand( Command::_currentSimpleCommand );
// 	}
// 	;

// arg_list:
// 	arg_list argument
// 	| /* empty */
// 	;

// argument:
// 	WORD {
//                printf("   Yacc: insert argument \"%s\"\n", $1);

// 	       Command::_currentSimpleCommand->insertArgument( $1 );\
// 	}
// 	;

// command_word:
// 	WORD {
//                printf("   Yacc: insert command \"%s\"\n", $1);
=======
simple_command:	
	pipe_list iomodifier_opt background_optional NEWLINE {
		printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	| NEWLINE {
		Command::_currentCommand.execute();
		return 0;
		}
	| error NEWLINE { 
		printf("There was a yyerror\n");
		yyerrok; }
	;

command_and_args:
	command_word arg_list {
		Command::_currentCommand.insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;

arg_list:
	arg_list argument
	| /* empty */
	;

pipe_list:
		pipe_list PIPE command_and_args
		| command_and_args
		;

argument:
	WORD {
               printf("   Yacc: insert argument \"%s\"\n", $1);
			std::string s = expandEnvVars(std::string($1));
			char *p = (char *)malloc(sizeof(char) * (s.size() + 1));
			strcpy(p, s.c_str());
	       Command::_currentSimpleCommand->insertArgument( p );\
	}
	;

command_word:
	WORD {
               printf("   Yacc: insert command \"%s\"\n", $1);
>>>>>>> Stashed changes
	       
// 	       Command::_currentSimpleCommand = new SimpleCommand();
// 	       Command::_currentSimpleCommand->insertArgument( $1 );
// 	}
// 	;

// iomodifier_opt:
// 	GREAT WORD {
// 		printf("   Yacc: insert output \"%s\"\n", $2);
// 		Command::_currentCommand._outFile = $2;
// 	}
// 	| /* empty */ 
// 	;
%%

int yywrap() {
        return 1;
}