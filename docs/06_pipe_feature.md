# Pipeline

Shell provides a way to chain commands together using pipelines, where the standard output of one command becomes the standard input of the next.

### Goals
- Implement the pipe operator for connecting commands.
- Implement support for builtin commands.
- Implement support for multi-command pipelines.

## Design
- The pipe `|` operator should redirect the output of the left command to the input of the right command.
    ``` Bash
    // stdout of cmd1 --> stdin of cmd2
    cmd1 | cmd2
    ```
- State changing builtins should not affect the shell process, only reflect changes in a spawned child process.
- Pipeline execution should be concurrent.

Note: Individual command segments in the pipeline can have their own redirection, independent of the pipeline connections.

## Implementation Details
- Pipeline execution works by opening a channel using `pipe()` for each segment, except the last one.
- The execution is then split by `fork()`, spwaning a child process and having the stdin and stdout properly redirected through the pipe fd(s).
- The segment command execution is the same as the main shell loop, except in a child process (even true for builtins).
- In the end, the parent process waits for all child processes to end to prevent zombie processes.

### Constants
| Constant | Value | Description |
|----------|------------|--------------|
|`SEG_BUFF_SIZE`|`64`|Buffer size for the command segments|
|`PIPE`|`"\|"`|Macro for the pipe symbol string|

### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`trim(str)`|`char*` — raw string|`char*` — whitespace trimmed output|Removes leading and trailing whitespaces from an input string|
|`has_pipe(line)`|`char*` — string line|`bool` — a boolean value|Check the line read at the prompt for pipes `"\|"`|
|`split_pipe(line)`|`char*` — string line|`char**` — array of string segments|Splits the line read at the prompt at pipes `"\|"` into a null terminated array of segment strings (fixed, accounts for quoting behavior as well now)|
|`pipe_execute(segments)`|`char**` — null terminated array of segments|`int` — an integer status|Executes the command pipeline with input/output redirection|

***
