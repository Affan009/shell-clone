# History and Persistence
The commands executed in the shell are stored in an in-memory history and persisted in a file to share said history across shell sessions.

### Important Note
- This implementation involves using the GNU Readline library for arrow key navigation for the history.

### Goals
- Implement the history builtin using readline's in-memory history storage.
- Implement persistence of command history in a separate file.

## Design
- A data structure, in this case readline's in-memory history list, should be used to store the command input by the user.
- The `history` builtin should use the history list to print the command history. It may additionally take an argument `n` to show the last `n` commands.
- The `history` builtin should accept `-r`, `-w`, and `-a` flags alongside a file name to read, write and append respectively, the command history to the provided file path.
- The entire history should be persisted in a file whose path is specified by the variable `HISTFILE` (a macro based on the actual env variable, not the actual env variable). This file is read at the beginning of a shell session, and written (overwritten) to at the end of session.

## Implementation Details
- A circular buffer `history` is used to store the actual command history in-memory.
- `hist_count` keeps track of the history size, `hist_start` is where the history logically starts, and `hist_index` is where the cursor logically is in the history.
- All these variables are static and exposed to other files via APIs.

### Constants
| Constant | Value | Description |
|----------|------------|--------------|
|`HISTSIZE`|`1000`|Maximum size of the in-memory history|
|`HISTFILE`|`.ash_history`|File where the history of a shell session is persisted (overwritten each time)|

### Data Structures
- A circular buffer using logical indexing:
    ``` C
    static char* history[HISTSIZE];

    int physical_index(int logical_index) { 
        return (hist_start + logical_index) % HISTSIZE;
    }
    ```

### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`init_history()`|None (Void)|None (Void)|Sets up the arrow navigation and loads the `HISTFILE` history|
|`physical_index(logical_index)`|`int` — logical Index|`int` — physical Index |Converts the logical index of an entry to its physical index in the buffer|
|`get_hist_count()`|None (Void)|`int` — `hist_count`|Returns the count of the total history|
|`get_hist_entry(index)`|`int` — index (logical)|`const char*` — history entry|Returns the entry in the history buffer corresponding to the logical index provided|
|`free_history()`|None (Void)|None (Void)|Frees the circular history buffer (at the physical indexes of the logical indexes present)|
|`prev_hist(count, key)`|`(int, int)` — unused parameters needed for correct function signature|`int` — status code|Moves the logical index to the previous entry (if possible) and replaces the line with its entry|
|`next_hist(count, key)`|`(int, int)` — unused parameters needed for correct function signature|`int` — status code|Moves the logical index to the next entry and replaces the line with its entry (empty line when `hist_index == hist_count`)|
|`add_hist(line)`|`const char*` — the line input by the user at the prompt|None (Void)|Adds the line to the in-memory history on the correct physical indexes accounting for wraparound behavior. Sets `hist_index` to the current `hist_count` at the end|
|`read_hist(path)`|`const char*` — path to the file being read in|None (Void)|Reads command history from the given file (if it exists, else returns) path and loads it into the in-memory history|
|`write_hist(path)`|`const char*` — path to the file being written to|None (Void)|Writes the in-memory command history to the given file. If the file exists, it overwrites it, else it creates a new one|
|`append_hist(path)`|`const char*` — path to the file being appended to|None (Void)|Appends the in-memory command history to the end of the given file. If the file doesn't exist, it creates it|
|`sh_history(args)`|`char**` — arguments to the builtin|`int` — status code|Prints the in-memory history to the terminal. A numeric argument (`n`) limits the number of entries being shown, while the `-r`, `-w`, and `-a` flags persist history to files|

