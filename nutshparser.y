
/*
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * 
 *
 */

%token	<string_val> WORD

%token 	NOTOKEN GREAT NEWLINE LESS GREATGREAT GREATGREATAMPERSAND AMPERSAND PIPE GREATAMPERSAND

%union	{
		char   *string_val;
	}

%{
extern "C" int yylex();
#define yylex yylex
#include <stdio.h>
#include <string>
#include <cstring>
#include <regex>
#include "command.h"

extern std::map<std::string, std::string> aliases;

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}


std::string expandEnvVars(std::string arg) {
	static const std::regex ENV{"\\$\\{([^}]+)\\}"};
    std::smatch ENVMatch;
	int ENVMatched = std::regex_search(arg, ENVMatch, ENV);
	auto aliasMatched = aliases.find(arg);
    while (ENVMatched || (aliasMatched != aliases.end())) {
		if (ENVMatched){
			arg.replace(ENVMatch.begin()->first, ENVMatch[0].second, getenv(ENVMatch[1].str().c_str()));
		} 
		ENVMatched = std::regex_search(arg, ENVMatch, ENV);
		if (aliasMatched != aliases.end()){
			arg = aliases[arg];
		}
		aliasMatched = aliases.find(arg);
	}
    return arg;
}

%}


%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	pipe_list iomodifier_opt background_optional NEWLINE {
		printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	| NEWLINE {Command::_currentCommand.execute();}
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word arg_list {
		Command::_currentCommand.insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;

arg_list:
	arg_list argument
	| /* empty */
	;

pipe_list:
		pipe_list PIPE command_and_args
		| command_and_args
		;

argument:
	WORD {
               printf("   Yacc: insert argument \"%s\"\n", $1);
			std::string s = expandEnvVars(std::string($1));
			char *p = (char *)malloc(sizeof(char) * (s.size() + 1));
			strcpy(p, s.c_str());
	       Command::_currentSimpleCommand->insertArgument( p );\
	}
	;

command_word:
	WORD {
               printf("   Yacc: insert command \"%s\"\n", $1);
	       
	       Command::_currentSimpleCommand = new SimpleCommand();
		   std::string s = expandEnvVars(std::string($1));
			char *p = (char *)malloc(sizeof(char) * (s.size() + 1));
			strcpy(p, s.c_str());
		   Command::_currentSimpleCommand->insertArgument( p );
	}
	;

iomodifier_opt:
	GREAT WORD {
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
	}
	|LESS WORD {
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inputFile = $2;
	}
	| GREATAMPERSAND WORD{
		printf("   Yacc: insert output \"%s\"\n", $2);
		printf(" Yacc: setting background True\n");
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._background = 1;
	}
	| /* empty */ 
	;

background_optional:
	AMPERSAND{
		printf(" Yacc: setting background True\n");
		Command::_currentCommand._background = 1;
	}
	|/* empty */
%%



#if 0
main()
{
	yyparse();
}
#endif
