#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <glob.h>

// class SimpleCommand {
//     public:
//         int _numberOfAvailableArguments;

//         int _numberOfArguments;

//         char ** _arguments;

//         SimpleCommand();
//         void InsertArgument( char* arg);
// };

// class Command {
//     public:

//         int _numberOfAvailableSimpleCommands;
//         int _numberOfSimpleCommands;
//         SimpleCommand** _simpleCommands;
//         char* _outFile;
//         char* _inputFile;
//         char* _errFile;
//         int _background;

        

//         Command();
//         void insertSimpleCommand(SimpleCommand * SimpleCommand);
//         void prompt();
//         void print();
//         void execute();
//         void clear();

//         static Command _currentCommand;
//         static SimpleCommand* _currentSimpleCommand;
//};