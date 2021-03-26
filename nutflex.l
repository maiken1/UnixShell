/* -*- indented-text -*- */
%option noyywrap
%{
#include "nutshparser.tab.h"
#include <string.h>

#ifdef DOESNT_HAVE_STRDUP
#warning DOESNT_HAVE_STRDUP
char *strdup(const char *s);
#endif

#define ECHO
%}
NL              [\n]
PIPE            [|]
OUTPUT_REDIR    [>]
INPUT_REDIR     [<]
BG              [&]
CHAR            [0-9a-zA-Z_/\-\*\.]
SPACE           [\032]
%% 

<<<<<<< Updated upstream
exit            { ECHO; return EXIT; }
{NL}            { ECHO; return 0; }
{BG}            { ECHO; return BACKGROUND; }
{PIPE}          { ECHO; return PIPE;}
{OUTPUT_REDIR}  { ECHO; return OUTPUT_REDIR; }
{INPUT_REDIR}   { ECHO; return INPUT_REDIR; }
{CHAR}+         { ECHO; yylval.string = strdup(yytext); return STRING; }
.               ;
=======
\n      {
            return NEWLINE;
        }

[ \t]   {
            /* Discard spaces and tabs */
        }

">"     {
            return GREAT;
        }

"<"     {
            return LESS;
        }

">>"    {
            return GREATGREAT;
        }

">&"    {
            return GREATAMPERSAND;
        }

">>&"    {
            return GREATGREATAMPERSAND;
        }

"|"     {
            return PIPE;
        }

"&"     {
            return AMPERSAND;
        }

"bye"   {
            return BYE;
        }
"setenv"    {
                return SETENV;
        }

[a-zA-Z][a-zA-Z0-9]*   {
            yylval.string = strdup(yytext);
            return WORD;
        }

.       {
            /* Add Tokens Above Here */
            /*Invalid Character in input */
            return NOTOKEN;
        }
>>>>>>> Stashed changes
%%