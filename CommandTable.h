#include "global.h"

int bye(pid_t mainID){
    printf("\n printing from header file\n");
    kill(mainID, SIGINT);
    return 1;
}

int nutshell_setenv(){
    //char* var = yylex().string;
    // char* arg = yylex().string;
    printf("\n setting env \n");
    printf("%s", yytext[0]);
    return 1;
}