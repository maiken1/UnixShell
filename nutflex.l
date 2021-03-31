<<<<<<< Updated upstream
=======

%option noyywrap
>>>>>>> Stashed changes
%{
#include "nutshparser.tab.h"
#include <string.h>
<<<<<<< Updated upstream
=======
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
    printf("cleaning up buffer");
    yy_delete_buffer(my_string_buffer);
    // ... but avoid trying to free the same buffer twice
    my_string_buffer = NULL;
}
>>>>>>> Stashed changes

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
