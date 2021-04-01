#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/wait.h>

#include <string.h>

#include <signal.h>

#include <typeinfo>

#include <string>

#include <fcntl.h>

#include <map>

#include <iostream>

#include "command.h"

#include "nutshparser.tab.h"

using namespace std;

int yyparse(void);
extern char ** environ;
extern "C"
int my_scan_string(const char * s);
extern "C"
void my_cleanup(void);

std::map < std::string, std::string > aliases;

SimpleCommand::SimpleCommand() {
  // Creat available space for 5 arguments
  _numberOfAvailableArguments = 5;
  _numberOfArguments = 0;
  _arguments = (char ** ) malloc(_numberOfAvailableArguments * sizeof(char * ));
}

void
SimpleCommand::insertArgument(char * argument) {
  if (_numberOfAvailableArguments == _numberOfArguments + 1) {
    // Double the available space
    _numberOfAvailableArguments *= 2;
    _arguments = (char ** ) realloc(_arguments,
      _numberOfAvailableArguments * sizeof(char * ));
  }

  _arguments[_numberOfArguments] = argument;

  // Add NULL argument at the end
  _arguments[_numberOfArguments + 1] = NULL;

  _numberOfArguments++;
}

Command::Command() {
  // Create available space for one simple command
  _numberOfAvailableSimpleCommands = 1;
  _simpleCommands = (SimpleCommand ** )
  malloc(_numberOfSimpleCommands * sizeof(SimpleCommand * ));

  _numberOfSimpleCommands = 0;
  _outFile = 0;
  _inputFile = 0;
  _errFile = 0;
  _background = 0;
}

void
Command::insertSimpleCommand(SimpleCommand * simpleCommand) {
  if (_numberOfAvailableSimpleCommands == _numberOfSimpleCommands) {
    _numberOfAvailableSimpleCommands *= 2;
    _simpleCommands = (SimpleCommand ** ) realloc(_simpleCommands,
      _numberOfAvailableSimpleCommands * sizeof(SimpleCommand * ));
  }

  _simpleCommands[_numberOfSimpleCommands] = simpleCommand;
  _numberOfSimpleCommands++;
}

void
Command::clear() {
  for (int i = 0; i < _numberOfSimpleCommands; i++) {
    for (int j = 0; j < _simpleCommands[i] -> _numberOfArguments; j++) {
      free(_simpleCommands[i] -> _arguments[j]);
    }

    free(_simpleCommands[i] -> _arguments);
    free(_simpleCommands[i]);
  }

  if (_outFile) {
    free(_outFile);
  }

  if (_inputFile) {
    free(_inputFile);
  }

  if (_errFile) {
    free(_errFile);
  }

  _numberOfSimpleCommands = 0;
  _outFile = 0;
  _inputFile = 0;
  _errFile = 0;
  _background = 0;
}

void
Command::print() {
  printf("\n\n");
  printf("              COMMAND TABLE                \n");
  printf("\n");
  printf("  #   Simple Commands\n");
  printf("  --- ----------------------------------------------------------\n");
  printf("Number of Simple Commands: %d\n", _numberOfSimpleCommands);

  for (int i = 0; i < _numberOfSimpleCommands; i++) {
    printf("  %-3d ", i);
    printf("Number of arguments: %d\n", _simpleCommands[i] -> _numberOfArguments);
    for (int j = 0; j < _simpleCommands[i] -> _numberOfArguments; j++) {
      printf("\"%s\" \t", _simpleCommands[i] -> _arguments[j]);
    }
  }

  printf("\n\n");
  printf("  Output       Input        Error        Background\n");
  printf("  ------------ ------------ ------------ ------------\n");
  printf("  %-12s %-12s %-12s %-12s\n", _outFile ? _outFile : "default",
    _inputFile ? _inputFile : "default", _errFile ? _errFile : "default",
    _background ? "YES" : "NO");
  printf("\n\n");

}

void
Command::execute() {
  // Don't do anything if there are no simple commands
  /// <summary>
  /// WHAT DO
  /// </summary>
  int status;
  ////////////////////
  if (_numberOfSimpleCommands == 0) {
    prompt();
    return;
  } else {
    // Print contents of Command data structure
    print();

    int defaultin = dup(0);
    int defaultout = dup(1);

    int fdin;
    if (_inputFile) {
      fdin = open(_inputFile, O_RDONLY);
    } else {
      //use default input
      fdin = dup(defaultin);
    }

    pid_t pid;
    int fdout;
    for (int i = 0; i < _numberOfSimpleCommands; i++) {
      //redirect input
      dup2(fdin, 0);
      close(fdin);
      //setup output
      if (_simpleCommands[i] == _simpleCommands[_numberOfSimpleCommands - 1]) {
        //last command
        if (_outFile) {
          fdout = open(_outFile, O_CREAT | O_WRONLY, 0777);
        } else {
          //default output
          fdout = dup(defaultout);
          // printf("I am the last command\n");
        }
      } else {
        //not last simple command
        printf("creating pipes\n");
        int fdpipe[2];
        pipe(fdpipe);
        fdout = fdpipe[1];
        fdin = fdpipe[0];
      }
      //redirect output
      dup2(fdout, 1);
      close(fdout);
      //may have to deal with this for backgrounds
      int status;
      string builtinCheck(_simpleCommands[i] -> _arguments[0]);

      if (builtinCheck == "bye") {
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 1, 1)) {
          printf("I want to exit\n");
        }
      } else if (builtinCheck == "setenv") {
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 3, 3)) {
          printf("Setting environment variable %s to %s\n", _simpleCommands[i] -> _arguments[1], _simpleCommands[i] -> _arguments[2]);
          setenv(_simpleCommands[i] -> _arguments[1], _simpleCommands[i] -> _arguments[2], 1);
        }
      } else if (builtinCheck == "printenv") {
        char ** s = environ;
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 1, 1)) {
          for (;* s; s++) {
            printf("%s\n", * s);
          }
        }
      } else if (builtinCheck == "unsetenv") {
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 2, 2)) {
          unsetenv(_simpleCommands[i] -> _arguments[1]);
        }
      } else if (builtinCheck == "cd") {
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 2, 2)) {
          chdir(_simpleCommands[i] -> _arguments[1]);
        }
      } else if (builtinCheck == "alias") {
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 1, 3)) {
          if (_simpleCommands[i] -> _numberOfArguments == 3) {
            aliases.insert(std::make_pair(string(_simpleCommands[i] -> _arguments[1]), string(_simpleCommands[i] -> _arguments[2])));
          } else if (_simpleCommands[i] -> _numberOfArguments == 1) {
            for (auto it = aliases.cbegin(); it != aliases.cend(); ++it) {
              string out = it -> first + "=" + it -> second;
              printf("%s\n", out.c_str());
            }
          } else {
            printf("alias: Wrong number of arguments\n");
          }
        }
      } else if (builtinCheck == "unalias") {
        if (CheckNumberOfArguments(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _numberOfArguments, 2, 2)) {
          aliases.erase(string(_simpleCommands[i] -> _arguments[1]));
        }
      }
      // else if (builtinCheck == "aliases") {
      // 	if (CheckNumberOfArguments(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_numberOfArguments, 2, 2)) {
      // 		aliases.erase(_simpleCommands[i]->_arguments[1]);
      // 	}
      // }
      else {
        //create child process
        // printf("forking\n");
        pid = fork();
        if (pid == 0) {
          //execution code here
          // REMOVE THIS CODE
          //execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
          // printf("inside fork\n");
          search(_simpleCommands[i] -> _arguments[0], _simpleCommands[i] -> _arguments);
          perror("search failed\n");
          // perror("execvp encountered an error");
          _exit(1);
          //search path for executable, error if not found

        }
      }
    }

    //restore in/out states to default
    dup2(defaultin, 0);
    dup2(defaultout, 1);
    close(defaultin);
    close(defaultout);
    // printf("defaults restored\n");

    if (!_background) {
      waitpid(pid, & status, WUNTRACED);
    }

    /*if (_numberOfSimpleCommands == 1 && _simpleCommands[0]->_numberOfArguments == 1) {
    	printf("I want to exit the shell.\n");
    	string someString(_simpleCommands[0]->_arguments[0]);
    	printf("%s",someString.c_str());
    }*/

    // Add execution here
    // For every simple command fork a new process
    // Setup i/o redirection
    // and call exec

    // Clear to prepare for next command
    // printf("clearing command table\n");
    clear();
    // Print new prompt

  }

}

int Command::CheckNumberOfArguments(char * command, int inputArguments, int min, int max) {
  if (inputArguments < min) {
    printf("%s: Too few arguments\n", command);
    return 0;
  } else if (inputArguments > max) {
    printf("%s: Too many arguments\n", command);
    return 0;
  }
  return 1;
}

int Command::search(char * cmd, char *
  const argv[]) {
  char buf[260];
  // return an error is there is no cmd
  if ( * cmd == '\0') {
    return -1;
  }
  // skip the search and execute if the cmd has a path
  else if (strchr(cmd, '/') != NULL) {
    execv(cmd, argv);
  }
  // search for the command and execute it
  else {
    size_t len, pathlen;
    char * name, * p;
    char * path = getenv("PATH");

    // if no path is specified check in bin
    if (path == NULL)
      path = (char * )(":/bin:/usr/bin");

    // make a copy of the command name and add a slash to the end
    name = (char * ) memcpy(buf + strlen(path) + 1, cmd, strlen(cmd) + 1);
    *--name = '/';

    p = path;
    do {
      char * startp;
      path = p;

      // check for any colons in the path
      p = strchr(path, ':');
      if (!p)
        p = strchr(path, '\0');
      if (p == path)
        startp = name + 1;
      else
        startp = (char * ) memcpy(name - (p - path), path, p - path);

      // eccute when path is figured out
      execv(startp, argv);

    } while ( * p++ != '\0');
  }

  return 1;
}

// Shell implementation

void
Command::prompt() {
  printf("myshell>");
  fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int GetCommand();

int main() {
  Command::_currentCommand.prompt();
  while (1) {

    GetCommand();
  }

  return 0;
}

int GetCommand() {
  string command;
  //char eatnewline;
  std::cin.clear();
  if (getline(std::cin, command)) {
    command = command + "\n";
    cout << command << endl;

    printf("\n%s\n", command.c_str());
    if (my_scan_string(command.c_str()) != 0) {
      printf("error in internal buffer\n");
      exit(1);
    }
    yyparse();
    my_cleanup();
    Command::_currentCommand.prompt();
  }
  std::cin.clear();

  return 1;
}