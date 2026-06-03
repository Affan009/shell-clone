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
- The entire history should be persisted in a file whose path is specified by the variable `HISTFILE` (based on the actual env variable). This file is read at the beginning of a shell session, and written/appended to at the end of session.
