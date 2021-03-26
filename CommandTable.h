#include "global.h"
#include "nutshparser.tab.h"
int bye(pid_t mainID){
    printf("\n printing from header file\n");
    kill(mainID, SIGINT);
    return 1;
}

int nutshell_setenv(){
    //char* var = yylex().string;
    // char* arg = yylex().string;
    printf("\n setting env \n");
    
    return 1;
}