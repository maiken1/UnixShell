
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

#include "command.h"
using namespace std;

SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **) malloc( _numberOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numberOfAvailableArguments == _numberOfArguments  + 1 ) {
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numberOfAvailableArguments * sizeof( char * ) );
	}
	
	_arguments[ _numberOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numberOfArguments + 1] = NULL;
	
	_numberOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numberOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numberOfAvailableSimpleCommands == _numberOfSimpleCommands ) {
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
			 _numberOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}
	
	_simpleCommands[ _numberOfSimpleCommands ] = simpleCommand;
	_numberOfSimpleCommands++;
}

void
Command:: clear()
{
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numberOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}
		
		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inputFile ) {
		free( _inputFile );
	}

	if ( _errFile ) {
		free( _errFile );
	}

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
}

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");
	printf("Number of Simple Commands: %d\n", _numberOfSimpleCommands);
	
	
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		printf("Number of arguments: %d\n", _simpleCommands[i]->_numberOfArguments);
		for ( int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
		_inputFile?_inputFile:"default", _errFile?_errFile:"default",
		_background?"YES":"NO");
	printf( "\n\n" );
	
}

void
Command::execute()
{
	// Don't do anything if there are no simple commands
	if ( _numberOfSimpleCommands == 0 ) {
		prompt();
		return;
	}

	else {
		// Print contents of Command data structure
		print();

		string someString(_simpleCommands[0]->_arguments[0]);

		int tmpin = dup(0);
		int tmpout = dup(1);

		int fdin;
		if (_inputFile) {
			fdin = open(_inputFile, O_RDONLY);
		}
		else {
			//use default input
			fdin = dup(tmpin);
		}

		pid_t pid;
		int fdout;
		for (int i = 0; i < _numberOfSimpleCommands; i++) {
			//redirect input
			dup2(fdin, 0);
			close(fdin);
			//setup output
			if (i == _numberOfSimpleCommands - 1) {
				//last command
				if (_outFile) {
					fdout = open(_outFile, O_CREAT | O_WRONLY, 0777);
				}
				else {
					//default output
					fdout = dup(tmpout);
				}
			}

			else {
				//not last simple command
				int fdpipe[2];
				pipe(fdpipe);
				fdout = fdpipe[1];
				fdin = fdpipe[0];
			}
			//redirect output
			dup2(fdout, 1);
			close(fdout);

			//create child process
			int status;
			pid = fork();
			if (pid == 0) {
				//execution code here
				// REMOVE THIS CODE
				execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
				perror("execvp encountered an error");
				_exit(1);
				//search path for executable, error if not found

			}
			
			//restore in/out states to default
			dup2(tmpin, 0);
			dup2(tmpout, 1);
			close(tmpin);
			close(tmpout);

			if (!_background) {
				waitpid(pid, &status, WUNTRACED);
			}
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
		clear();
		// Print new prompt
		
	}
	
}

// Shell implementation

void
Command::prompt()
{
	printf("myshell>");
	fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

int main()
{
	Command::_currentCommand.prompt();
	yyparse();
	return 0;
}

