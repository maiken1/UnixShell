
/*
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * 
 *
 */

%token	<string_val> WORD

%token 	NOTOKEN GREAT NEWLINE LESS GREATGREAT GREATGREATAMPERSAND AMPERSAND PIPE GREATAMPERSAND

%union	{
		char   *string_val;
	}

%{
extern "C" int yylex();
#define yylex yylex
#include <stdio.h>
#include "command.h"

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

%}


%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	pipe_list iomodifier_opt background_optional NEWLINE {
		printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
		Command::_currentCommand.prompt();
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word arg_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
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

	       Command::_currentSimpleCommand->insertArgument( $1 );\
	}
	;

command_word:
	WORD {
               printf("   Yacc: insert command \"%s\"\n", $1);
	       
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;

iomodifier_opt:
	GREAT WORD {
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
	}
	|LESS WORD {
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inputFile = $2;
	}
	| GREATAMPERSAND WORD{
		printf("   Yacc: insert output \"%s\"\n", $2);
		printf(" Yacc: setting background True\n");
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._background = 1;
	}
	| /* empty */ 
	;

background_optional:
	AMPERSAND{
		printf(" Yacc: setting background True\n");
		Command::_currentCommand._background = 1;
	}
	|/* empty */
%%

#if 0
main()
{
	yyparse();
}
#endif
