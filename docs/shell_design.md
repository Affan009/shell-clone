# ash: C-Shell Design Document

## Introduction
A UNIX-like shell accepts user commands at the prompt, parses and executes them.

Goals:
1. REPL (Read-Eval-Print Loop) Implementation.
2. Implement commonly used builtin commands.
3. External command execution via PATH lookup.

## REPL Flow

``` mermaid
graph LR
	A[Read] --> B[Tokenize]
	B --> C[Execute]
	C -->|Loop| A
```

### Loop Design

``` C
do {
	print("$")
	
	line = readline()
	args = splitline(line)
	status = execute(args)
	
} while (status)
```

## Parser

Parser handles taking input from the user, and splitting the input line into arguments.

### Functions

| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`read_line()`|None (Void)|`char*` — a string | Reads line entered by the user|
|`split_line(line)`|`char*` — a string line|`char**` — an array of string arguments|Splits the line string with respect to delimiters into a null terminated array of strings. The first argument is the command|


## Executor

Executor handles evaluation and execution of the commands input by the user at the prompt.

### Functions

| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`sh_execute(args)`|`char**` — an array of string arguments|`int` — an integer status|Executes the command in args. It distinguishes between builtin and external programs|
|`sh_launch(args)`|`char**` — an array of string arguments|`int` — an integer status|Executes external programs in the PATH as separate child processes|

## Builtins

Builtins are programs/commands built into the shell. They are executed in the same process as the shell (no forking), as they change the shell's operation itself. Each builtin takes `args` as input and returns an `int` status. The builtins implemented here are as follows:

### Functions

| Function | Description |
|----------|-------------|
|`sh_help(args)`|Prints a list of all the available builtins in the shell|
|`sh_exit(args)`|Terminates the shell loop|
|`sh_echo(args)`|Prints its arguments to the standard output|
|`sh_type(args)`|Prints whether the command passed to it as an argument is a builtin or an external program, along with the path of external programs|
|`sh_pwd(args)`|Prints the absolute path of the current working directory|
|`sh_cd(args)`|Changes the current working directory to the path passed as an argument provided it is valid|

***
