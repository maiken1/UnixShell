
#pragma once
// #include <vector>
// std::vector<char *> alias_list;
std::map<char*, char*> aliases;
// Command Data Structure
struct SimpleCommand {
	// Available space for arguments currently preallocated
	int _numberOfAvailableArguments;

	// Number of arguments
	int _numberOfArguments;
	char ** _arguments;
	
	SimpleCommand();
	void insertArgument( char * argument );
};

struct Command {
	int _numberOfAvailableSimpleCommands;
	int _numberOfSimpleCommands;
	SimpleCommand ** _simpleCommands;
	char * _outFile;
	char * _inputFile;
	char * _errFile;
	int _background;

	void prompt();
	void print();
	void execute();
	void clear();
	int CheckNumberOfArguments(char* command, int inputArguments, int min, int max);
	int search(char * file, char * const argv[]);
	
	Command();
	void insertSimpleCommand( SimpleCommand * simpleCommand );

	static Command _currentCommand;
	static SimpleCommand *_currentSimpleCommand;
};

