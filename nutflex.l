
%{

#include <string.h>
#include "y.tab.h"

%}

%%

\n 	{
		return NEWLINE;
	}

[ \t] 	{
		/* Discard Spaces and Tabs */
     	}	

">" 	{
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

[^ \t\n][^ \t\n]*   {
            yylval.string_val = strdup(yytext);
            return WORD;
        }

.	{
		/* Invalid character in input */
		return NOTOKEN;
	}

%%
	
