/* -*- indented-text -*- */

%{
#include "global.h"

extern void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 };
<<<<<<< Updated upstream
extern int yylex(void);
int yylex();
=======
extern int yylex();
int yyparse();

>>>>>>> Stashed changes

%}

%union {
       char *string;
}


<<<<<<< Updated upstream
%start cmd_line
%token <string> EXIT PIPE INPUT_REDIR OUTPUT_REDIR STRING NL BACKGROUND


%%
cmd_line    :
        | EXIT             { }
        | pipeline back_ground
        ;

back_ground : BACKGROUND        {  }
        |                       {  }
        ;

simple      : command redir
        ;

command     : command STRING
                {
                }
        | STRING
                {
                }
        ;

redir       : input_redir output_redir
        ;

output_redir:    OUTPUT_REDIR STRING
                { 
                }
        |        /* empty */
				{
				}
        ;

input_redir:    INPUT_REDIR STRING
                {
                }
        |       /* empty */
                {
				}
        ;

pipeline    : pipeline PIPE simple
                {
                }
        | simple
                {
                }
        ;
=======
%token NOTOKEN GREAT NEWLINE WORD GREATGREAT PIPE AMPERSAND LESS GREATGREATAMPERSAND GREATAMPERSAND BYE SETENV


%%

exit:
        %empty
        | BYE NEWLINE{return BYE;}
        ;

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
>>>>>>> Stashed changes
%%

int yywrap() {
        return 1;
}