

%token	<string_val> WORD ERROROUTPUT

%token 	INVALID OUTPUTFILE NEWLINE INPUTFILE APPENDFILE GREATGREATAMPERSAND AMPERSAND PIPE ERRORGREATAMPERSAND

%union	{
		char   *string_val;
	}

%{
extern "C" 
int yylex();

#define yylex yylex

#include <stdio.h>

#include <string>

#include <cstring>

#include <regex>
#include <regex.h>
#include <unistd.h>
#include <dirent.h>
#include "command.h"


void expandRegexTokens(char * arg){
	std::string noArgDefault(arg);
	bool argAdded = false;
	if(strchr(arg, '*') == NULL && strchr(arg, '?') == NULL){
		CommandTable::currentCommand->addArg(arg);
		return;
	}
	if(strcmp(CommandTable::currentCommand->args[0], "alias") == 0){
		CommandTable::currentCommand->addArg(arg);
		return;
	}

	//Converting wild cards
	//convert * -> .*
	//? -> .
	//. -> \.
	char * reg = (char*)malloc(2*strlen(arg)+10);
	char * a = arg;
	char * r = reg;
	*r = '^';
	r++; //get the beginning of the line
	while(*a) {
		if (*a == '*'){
			*r='.'; 
			r++;
			*r='*';
			r++;
		}
		else if (*a == '?'){
			*r='.';
			r++;
		}
		else if(*a == '.'){
			*r='\\';
			r++;
			*r='.';
			r++;
		}
		else{
			*r=*a;
			r++;
		}
		a++;
	}
	*r = '$';
	r++;
	*r = '\0';
	//get end of line and add null char
	regex_t regex;
	int retvalue = regcomp(&regex, reg, 0);
	if(retvalue != 0){
		perror("refcomp");
		return;
	}

//	List directory and add as arguments the entries
// that match the regular expression

	DIR * dir = opendir(".");
	if (dir == NULL){
		perror("oopendir");
		return;
	}
	struct dirent * ent;
	while((ent = readdir(dir)) != NULL){
		//check if name is a match
		if(regexec(&regex, ent->d_name, 0, NULL, 0) == 0){
			//add arg if name matches
			CommandTable::currentCommand->addArg(strdup(ent->d_name));
			argAdded = true;
		}
	}
	closedir(dir);
	if(!argAdded){
		noArgDefault.erase(remove(noArgDefault.begin(), noArgDefault.end(), '?'), noArgDefault.end());
		noArgDefault.erase(remove(noArgDefault.begin(), noArgDefault.end(), '*'), noArgDefault.end());
		CommandTable::currentCommand->addArg(strdup(noArgDefault.c_str()));
	}
}

void
yyerror(const char * s) {
  fprintf(stderr, "%s", s);
}

%}


%%

line:	cmd_line
	;

cmd_line: cmd
	| cmd_line cmd 
	;

cmd: parse_cmd
    ;

parse_cmd:	
	pipe_list iomod_list bg_opt NEWLINE {
		CommandTable::currentCommandTable.execute();
	}
	| NEWLINE {CommandTable::currentCommandTable.execute();}
	| error NEWLINE { yyerrok; }
	;

cmd_and_args:
	cmd_word arg_list {
		CommandTable::currentCommandTable.addCmd( CommandTable::currentCommand );
	}
	;

arg_list:	arg_list arg
	| /* empty */
	;

pipe_list:	pipe_list PIPE cmd_and_args
	| cmd_and_args
	;

arg:
	WORD {
			expandRegexTokens($1);
	}
	;

cmd_word:
	WORD {
	    	CommandTable::currentCommand = new Command();
			CommandTable::currentCommand->addArg( $1 );
	}
	;

iomod_opt:
	OUTPUTFILE WORD ERRORGREATAMPERSAND{
		CommandTable::currentCommandTable.outputFile = $2;
		CommandTable::currentCommandTable.errorFile = (char *)malloc((strlen(CommandTable::currentCommandTable.outputFile)+1)*sizeof(char));
		strcpy(CommandTable::currentCommandTable.errorFile, CommandTable::currentCommandTable.outputFile);
	}
	| OUTPUTFILE WORD {
		CommandTable::currentCommandTable.outputFile = $2;
	}
	|INPUTFILE WORD {
		CommandTable::currentCommandTable.inputFile = $2;
	}
	| APPENDFILE WORD{
		CommandTable::currentCommandTable.outputFile = $2;
		CommandTable::currentCommandTable.append = true;
	}
	| ERROROUTPUT WORD{
		CommandTable::currentCommandTable.errorFile = $2;
	}
	| ERROROUTPUT {
		CommandTable::currentCommandTable.errorFile = $1;
	}
	;

iomod_list:	iomod_list iomod_opt
	| /*empty*/
	;

bg_opt:
	AMPERSAND{
		CommandTable::currentCommandTable.background = true;
	}
	|/* empty */
	;
%%



#if 0
main()
{
	yyparse();
}
#endif
