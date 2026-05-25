# Quoting
Quoting controls how characters and words are interpreted by the shell.

Examples of quoting are:
``` Bash
$ echo 'hello world'
hello world

$ echo "hello world"
hello world

$ echo 'hello    world'
hello    world

$ echo hello\ world
hello world

$ echo "hello \"world\""
hello "world"

$ echo "\n"
\n
```

## Goals
- Implement quoting with single and double quotes, disabling all meaning from certain characters inside.
- Implement escape behaviour inside and outside quotes.

## Design
- Characters inside quotes are treated literally. Quoted strings are concatenated.
- Escaping is done through the backslash character `\` to remove special meaning from the character after it.
- Single quotes have no escaping behavior and only certain characters are escapable inside double quotes.
- Quoted executables run normally.

## Implementation Details
- The `split_line()` function is modified to implement the quoting behavior by introducing a Read-Write Pointer approach.
- The line from `read_line()` is read by a `read_ptr` and modified in place by a `write_ptr`.
- A boolean flag `in_arg` is used to track whether the character currently being visited is part of the current argument.
- A `ParserState` variable `state` tracks the current parser state.

### Constants
| Constant | Value | Description |
|----------|------------|--------------|
|`ESCAPE_CHARS`|`"\"\\"`|Special characters that can be escaped inside double quotes|

### Data Structures
- An `enum` to represent the parsing state, aliased as `ParserState`:
    ``` C
    typedef enum {
        STATE_PARSER_NORMAL,
        STATE_IN_SINGLE_QUOTE,
        STATE_IN_DOUBLE_QUOTE
    } ParserState;
    ```

### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`split_line(line)` (modified)|`char*` — a string line|`char**` — an array of string arguments|Splits the line string with respect to delimiters and accounting for quoting behavior into a null terminated array of strings|

***
