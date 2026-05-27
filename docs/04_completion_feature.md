# Autocompletion
Shell automatically suggests possible matches and completes partial input commands and filenames.

### Important Note
- This implementation involves using the **readline** function and handing it custom completion logic. Future work can involve implementing **readline** from scratch.

### Goals
- Implement command (builtin or PATH executable).
- Implement filename and directory completion for arguments.
- Support multiple and partial completions.
- Support multi-argument completion.

## Common Design Details
- Pressing the `<TAB>` key would trigger autocompletion.
- Command completion occurs for the first argument, and filename completion for subsequent completion.
- If there is a single match for completion, it is inserted.
- If there is no match for completion, nothing is inserted.
- If there are multiple matches for completion, the Longest Common Prefix (LCP) is inserted and a sorted list of all matches is shown.

Important Note: The **readline** function would handle the logic for the LCP, match list sorting and displaying - custom logic would only return a single match at a time.

| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`init_completion()`|None (Void)|None (Void)|Initializes the custom completion logic by setting `rl_attempted_completion_function` to it|
|`completion(text, start, end)`|`const char* text` - text read by the readline, `int start` - where the word being completed begins, `int end` - current cursor position (unused)|`char**` - list of match strings|Disables readline's internal completion logic and provide an entry point to the custom|

## Command Completion

### Specifics
- Searches builtins first, and then PATH executables.

### Implementation Details
- Called multiple times within `rl_completion_matches(text, cmd_generator)` when `start == 0`, meaning the first word (command) is being completed, and it returns a single match in each call, which accumulate across multiple calls.
- Uses static variables that persist across calls to avoid redundancy while finding matches.
- `state` variable tracks whether the function is called the first time or later. Helps initialize static variables in the first call when `state == 0`.
- `search_phase` variable determines whether to search for builtins (`0`) or PATH (`1`) for matches.
- Builtins are searched using the `builtins` list already in code. Executables are searched in `PATH` by tokenizing on delimiter `:` using `strtok` and opening that directory, then checking whether an entry of the directory is executable or not before returning that match.
- `path_copy` pointer contains the duplicated PATH pointer (using `strdup`) and is used for tokenization. Direct usage of the PATH pointer (from `getenv`) is not good as `strtok` changes the tokenized string.

#### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`cmd_generator(text, state)` (static)|`char* text` — string of text currently read by readline and `int state` — `0` on the first call, non-zero on subsequent calls|`char* ` — the string of the match|Searches for a match to the given text (command) and returns it. Called multiple times to return multiple matches|

## Filename Completion

### Specifics
- Searches files and directories inside the most enclosing directory in the text.

### Implementation Details
- Called multiple times within `rl_completion_matches(text, file_generator)` when `start != 0`, meaning the arguments are being completed, and returns a single match in each call, which accumulate across multiple calls.
- Uses static variables that persist across calls to avoid redundancy while finding matches.
- `state` variable tracks whether the function is called the first time or later. Helps initialize static variables in the first call when `state == 0`.
- Search occurs inside the most enclosing directory (till last slash `/` in word) for files and other directories. If the word contains no slash, then the current directory (`.`) is searched.
- Directory detection uses `stat` to check `S_ISDIR` on the full path after which a trailing `/` is concatenated to the full path and `rl_completion_append_character` is set to `'\0'` for directories, which is restored to `' '` for regular files.

#### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`file_generator(text, state)` (static)|`char* text` — string of text currently read by readline and `int state` — `0` on the first call, non-zero on subsequent calls|`char* ` — the string of the match|Searches for a match to the given text (file/directory) and returns it. Called multiple times to return multiple matches|
