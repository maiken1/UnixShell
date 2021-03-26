%{
#include "nutshparser.tab.h"
#include <string.h>

#ifdef DOESNT_HAVE_STRDUP
#warning DOESNT_HAVE_STRDUP
char *strdup(const char *s);
#endif
%}

%% 


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
%%
