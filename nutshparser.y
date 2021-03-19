/* -*- indented-text -*- */

%{
#include "global.h"

extern void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 };
extern int yylex(void);
int yylex();

%}

%union {
       char *string;
}


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
%%
