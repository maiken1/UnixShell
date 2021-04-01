%option noyywrap
%{

#include <string.h>
#include "nutshparser.tab.h"


static YY_BUFFER_STATE my_string_buffer;

int my_scan_string(const char *s) {
    // insist on cleaning up any existing buffer before setting up a new one
    if (my_string_buffer != NULL) return -1;

    // Set up and switch to a buffer for scanning the contents of the
    // specified string.  A copy of the string will be made.
    my_string_buffer = yy_scan_string(s);
    return (my_string_buffer == NULL) ? -1 : 0;
}

void my_cleanup(void) {
    // No effect if my_string_buffer is NULL
    printf("cleaning up buffer\n");
    yy_delete_buffer(my_string_buffer);
    // ... but avoid trying to free the same buffer twice
    my_string_buffer = NULL;
}

%}

%%

\".*\"  {
            yylval.string_val = strdup(yytext);
            return WORD;
        }

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

"| "     {
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
	
