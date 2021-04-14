## The Nutshell

#### Summary of Work:

The included nutshell is the result of the efforts of students Marshall Aiken and Oliver Dahl. Marshall Aiken worked on implementing Flex and Bison parsing and correctly interpreting input. This included implementation of a system that would allow the feeding of one command string at a time to the parser so as to control input and for ease of controlling the basic shell loop. Within flex, token rules were denoted to process special characters and WORDs. The interpreted stream of tokens was then fed through Bison which then matched the stream to a series of grammar rules. Marshall used these grammar rules to fill out and facilitate the execution of a command table data structure. Marshall also implemented the backbone of the shell execution loop, accounting for input, output, error output, piping, and waiting for background processes.

Oliver Dahl implemented the actual execution of built in functions and non-built-in functions. This included adding functionality to facilitate all environment variable functions, all alias functionality, the cd command, bye, and special handling of the PATH and HOME environment variables. Oliver also implemented the expansion of wild cards, aliases, and environment variables that are present within commands. To facilitate the execution of non built in functions, Oliver employed a searching algorithm that would loop through the current PATH of the shell, executing the function if it is found.

Both students fairly contributed towards the success of the project. Several pair programming sessions were conducted to develop, discuss, and thoroughly test all of the features mentioned above.

We hope you enjoy using the nutshell.

#### Features NOT Implemented:

- Tilde Expansion
- File Name Completion

#### Features Implemented:

- Built-in Commands:
  - setenv variable word, printenv, unsetenv variable, cd, alias name word,  unalias name, alias, bye, infinite loop alias-expansion detection
- Non-built-in Command
- Redirecting I/O with Non-built-in Commands
- Using Pipes with Non-built-in Commands
- Running Non-built-in Commands in Background
- Using both Pipes and I/O Redirection, combined, with Non-built-in Commands
- Environment Variable Expansion
- Alias Expansion
- Wildcard Matching

