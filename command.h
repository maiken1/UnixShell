#pragma once

#include <stdio.h>

#include <stdlib.h>

#include <map>

#include <string>

// #include <vector>
// std::vector<char *> alias_list;
// CommandTable Data Structure
struct Command {
  // Available space for arguments currently preallocated
  int argSpaceAvailable;

  // Number of arguments
  int numArgs;
  char** args;

  Command();
  void addArg(char * argument);
};

struct CommandTable {
  int numOfCommandsAvailable;
  int numCommands;
  Command** commands;
  char* outputFile;
  char* inputFile;
  char* errorFile;
  bool background = false;
  bool append = false;
  bool bye = false;

  void prompt();
  void execute();
  void clear();
  int CheckNumberOfArguments(char * command, int inputArguments, int min, int max);
  int search(char * file, char *
    const argv[]);

  CommandTable();
  void addCmd(Command* command);

  static CommandTable currentCommandTable;
  static Command* currentCommand;
};