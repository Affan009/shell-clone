# Output Redirection
Redirection changes where the shell reads input from and write output to. This feature is concerned with output redirection only.

## Goals
- Implement output redirection of stdout and stderr to a file.
- Implement appending redirection of stdout and stderr to a file.

## Design
- `>` or `1>` in the command would redirect `stdout` to the file, overwriting its contents. `>>` or `1>>` would be appending instead.
- `2>` in the command would redirect `stderr` to the file, overwriting its contents. `2>>` would be appending instead.
- The file descriptor of the stream would be set to point to the fd of the file, and then reset back at the end of the REPL loop.

## Implementation Details
- Redirection would be handled before command execution.
- A variable `redirected_fd` would be initialized to `-1` pointing to no stream, would be passed to the handler.
- The handler function detects the redirection operator and assigns the corresponding stream to `redirected_fd`, which is then pointed to the fd of the file opened.
- The original stream fd is saved as `saved_fd` and restored after execution.

### Constants
| Constant | Value | Description |
|----------|------------|--------------|
|`OVERWRITE_FLAGS`|`(O_WRONLY \| O_CREAT \| O_TRUNC)`|Open permission flags allowing creating, writing, and truncating a file|
|`APPEND_FLAGS`|`(O_WRONLY \| O_CREAT \| O_APPEND)`|Open permission flags allowing creating, writing, and appending to a file|

Note: Here `|` is the `OR` bitwise operator.

### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`handle_redirection(args, &redirected_fd)`|`char** args` — an array of string arguments and `int* redirected_fd` — the fd of the stream that is to be redirected to a file|`int` — the `saved_fd` of the redirected stream for restoration|Searches for a redirection operator in `args` and redirected corresponding stream to the file. Returns `saved_fd` for restoration|

***
