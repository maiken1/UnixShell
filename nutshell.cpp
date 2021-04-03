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

#include <regex>

#include <regex.h>

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
std::map < std::string, int > visited;

int checkForENVS(std::string arg) {
    static
        const std::regex ENV{
          "\\$\\{([^}]+)\\}"
    };
    std::smatch ENVMatch;
    return std::regex_search(arg, ENVMatch, ENV);
}

int checkForAliases(std::string arg) {
    string firstWord = arg.substr(arg.find_first_not_of(" "), arg.find(" "));
    if (visited[firstWord] > 0) {
        perror("Alias Recursion Detected: Halting alias expansion");
        return false;
    }
    return aliases.find(firstWord) != aliases.end();
}

std::string expandEnvVars(std::string& arg) {
    static
        const std::regex ENV{
          "\\$\\{([^}]+)\\}"
    };
    std::smatch ENVMatch;

    while (std::regex_search(arg, ENVMatch, ENV)) {
        arg.replace(ENVMatch.begin()->first, ENVMatch[0].second, getenv(ENVMatch[1].str().c_str()));
    }
    return arg;
}

std::string expandAliases(std::string& arg) {
    
    while (checkForAliases(arg)) {
        string firstWord = arg.substr(arg.find_first_not_of(" "), arg.find(" "));
        arg.replace(arg.find(firstWord), firstWord.length(), aliases[firstWord]);
        visited[firstWord]++;
    }
    return arg;
}

std::string processExpansions(std::string& arg) {
    
    while (checkForENVS(arg) || checkForAliases(arg)) {
        arg = expandEnvVars(arg);
        arg = expandAliases(arg);
    }
    visited.clear();
    return arg;
}

Command::Command() {
  // Creat available space for 5 arguments
  argSpaceAvailable = 5;
  numArgs = 0;
  args = (char ** ) malloc(argSpaceAvailable * sizeof(char * ));
}

void
Command::addArg(char * argument) {
  if (argSpaceAvailable == numArgs + 1) {
    // Double the available space
    argSpaceAvailable *= 2;
    args = (char ** ) realloc(args,
      argSpaceAvailable * sizeof(char * ));
  }

  args[numArgs] = argument;

  // Add NULL argument at the end
  args[numArgs + 1] = NULL;

  numArgs++;
}

CommandTable::CommandTable() {
  // Create available space for one simple command
  numOfCommandsAvailable = 1;
  commands = (Command ** )
  malloc(numCommands * sizeof(Command * ));

  numCommands = 0;
  outputFile = 0;
  inputFile = 0;
  errorFile = 0;
  background = 0;
}

void
CommandTable::addCmd(Command * command) {
  if (numOfCommandsAvailable == numCommands) {
    numOfCommandsAvailable *= 2;
    commands = (Command ** ) realloc(commands,
      numOfCommandsAvailable * sizeof(Command * ));
  }

  commands[numCommands] = command;
  numCommands++;
}

void
CommandTable::clear() {
  for (int i = 0; i < numCommands; i++) {
    for (int j = 0; j < commands[i] -> numArgs; j++) {
      free(commands[i] -> args[j]);
    }

    free(commands[i] -> args);
    free(commands[i]);
  }

  if (outputFile) {
    free(outputFile);
  }

  if (inputFile) {
    free(inputFile);
  }

  if (errorFile) {
    free(errorFile);
  }

  numCommands = 0;
  outputFile = 0;
  inputFile = 0;
  errorFile = 0;
  background = 0;
}

void
CommandTable::print() {
  printf("\n\n");
  printf("              COMMAND TABLE                \n");
  printf("\n");
  printf("  #   Simple Commands\n");
  printf("  --- ----------------------------------------------------------\n");
  printf("Number of Simple Commands: %d\n", numCommands);

  for (int i = 0; i < numCommands; i++) {
    printf("  %-3d ", i);
    printf("Number of arguments: %d\n", commands[i] -> numArgs);
    for (int j = 0; j < commands[i] -> numArgs; j++) {
      printf("\"%s\" \t", commands[i] -> args[j]);
    }
  }

  printf("\n\n");
  printf("  Output       Input        Error        Background\n");
  printf("  ------------ ------------ ------------ ------------\n");
  printf("  %-12s %-12s %-12s %-12s\n", outputFile ? outputFile : "default",
    inputFile ? inputFile : "default", errorFile ? errorFile : "default",
    background ? "YES" : "NO");
  printf("\n\n");

}

void
CommandTable::execute() {
  // Don't do anything if there are no simple commands
  /// <summary>
  /// WHAT DO
  /// </summary>
  int status;
  ////////////////////
  if (numCommands == 0) {
    prompt();
    return;
  } else {
    // Print contents of CommandTable data structure
    print();

    int defaultin = dup(0);
    int defaultout = dup(1);
    int defaulterr = dup(2);

    int fdin;
    if (inputFile) {
      fdin = open(inputFile, O_RDONLY);
      if (fdin < 0) {
          perror("Input File does not exist");
          clear();
          return;
      }
    } else {
      //use default input
      fdin = dup(defaultin);
    }

    pid_t pid;
    int fdout;
    int fderr;

    if (errorFile) {
        if (!append) {
            fderr = open(outputFile, O_CREAT | O_WRONLY, 0777);
        }
        else {
            fderr = open(outputFile, O_WRONLY | O_APPEND, 0777);
        }
    }
    else {
        fderr = dup(defaulterr);
    }
    dup2(fderr, 2);
    close(fderr);
    for (int i = 0; i < numCommands; i++) {
      //redirect input
      dup2(fdin, 0);
      close(fdin);
      //setup output
      if (commands[i] == commands[numCommands - 1]) {
        //last command
        if (outputFile) {
            if (!append) {
                fdout = open(outputFile, O_CREAT | O_WRONLY, 0777);
            }
            else {
                fdout = open(outputFile, O_WRONLY | O_APPEND, 0777);
            }
        } else {
          //default output
          fdout = dup(defaultout);
          // printf("I am the last command\n");
        }
      } else {
        //not last command
        int fdpipe[2];
        pipe(fdpipe);
        fdout = fdpipe[1];
        fdin = fdpipe[0];
      }
      //redirect output
      dup2(fdout, 1);
      //dup2(fderr, 2);
      close(fdout);
      //close(fderr);
      //may have to deal with this for backgrounds
      int status;
      string builtinCheck(commands[i] -> args[0]);

      if (builtinCheck == "bye") {
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 1, 1)) {
            this->bye = true;
        }
      } else if (builtinCheck == "setenv") {
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 3, 3)) {
          printf("Setting environment variable %s to %s\n", commands[i] -> args[1], commands[i] -> args[2]);
          setenv(commands[i] -> args[1], commands[i] -> args[2], 1);
        }
      } else if (builtinCheck == "printenv") {
        char ** s = environ;
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 1, 1)) {
          for (;* s; s++) {
            printf("%s\n", * s);
          }
        }
      } else if (builtinCheck == "unsetenv") {
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 2, 2)) {
          unsetenv(commands[i] -> args[1]);
        }
      } else if (builtinCheck == "cd") {
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 1, 2)) {
            if (commands[i]->numArgs == 1) { chdir(getenv("HOME")); }
            else{ chdir(commands[i]->args[1]); }
          
        }
      } else if (builtinCheck == "alias") {
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 1, 3)) {
          if (commands[i] -> numArgs == 3) {
            aliases.insert(std::make_pair(string(commands[i] -> args[1]), string(commands[i] -> args[2])));
          } else if (commands[i] -> numArgs == 1) {
            for (auto it = aliases.cbegin(); it != aliases.cend(); ++it) {
              string out = it -> first + "=" + it -> second;
              printf("%s\n", out.c_str());
            }
          } else {
            perror("alias: Wrong number of arguments\n");
          }
        }
      } else if (builtinCheck == "unalias") {
        if (CheckNumberOfArguments(commands[i] -> args[0], commands[i] -> numArgs, 2, 2)) {
          aliases.erase(string(commands[i] -> args[1]));
        }
      }
      else {
        //create child process
        // printf("forking\n");
        pid = fork();
        if (pid == 0) {
          //execution code here
          // REMOVE THIS CODE
          //execvp(commands[i]->args[0], commands[i]->args);
          // printf("inside fork\n");
          search(commands[i] -> args[0], commands[i] -> args);
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
    dup2(defaulterr, 2);
    close(defaultin);
    close(defaultout);
    close(defaulterr);

    if (!background) {
      waitpid(pid, & status, WUNTRACED);
    }

  
    // Clear to prepare for next command
    clear();
    // Print new prompt

  }

}

int CommandTable::CheckNumberOfArguments(char * command, int inputArguments, int min, int max) {
  if (inputArguments < min) {
    printf("%s: Too few arguments\n", command);
    return 0;
  } else if (inputArguments > max) {
    printf("%s: Too many arguments\n", command);
    return 0;
  }
  return 1;
}

int CommandTable::search(char * cmd, char *
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
CommandTable::prompt() {
  char* wd = get_current_dir_name();
  printf("nutshell:%s$  ", wd);
  free(wd);
  fflush(stdout);
}

CommandTable CommandTable::currentCommandTable;
Command* CommandTable::currentCommand;

int GetCommand();

int main() {
  
  do {

    GetCommand();
  } while (CommandTable::currentCommandTable.bye == false);
  printf("Thank you for using the nutshell.\n");

  return 0;
}

int GetCommand() {
  CommandTable::currentCommandTable.prompt();
  string command;
  //char eatnewline;
  std::cin.clear();
  if (getline(std::cin, command)) {
    if (command.substr(command.find_first_not_of(" "), command.find(" ")) != "unalias") {
        processExpansions(command);
    }
    else {
        expandEnvVars(command);
    }
    command = command + "\n";
    if (my_scan_string(command.c_str()) != 0) {
      printf("error in internal buffer\n");
      my_cleanup();
    }
    else {
        yyparse();
        my_cleanup();
    }
  }
  std::cin.clear();

  return 1;
}