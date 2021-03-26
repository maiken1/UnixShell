%{
#include "global.h"

extern void yyerror (char *s) {
   fprintf (stderr, "%s\n", s);
 };
extern int yylex();
int yyparse();
int yywrap() {
        return 1;
}

%}

%union {
       char *string;
}


%token NOTOKEN GREAT NEWLINE WORD GREATGREAT PIPE AMPERSAND LESS GREATGREATAMPERSAND GREATAMPERSAND


%%

input:
        %empty
        | input object { printf("input found"); } ;

object:
        WORD 
         ; 

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
