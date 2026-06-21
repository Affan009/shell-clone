# ash: a C-shell
ash is a UNIX-like shell clone in C that reads, parses, and executes commands similar to popular shells like Bash or Zsh.

## Features
- Builtin commands
- External program execution via PATH lookup
- Quoting (Single and Double Quotes)
- Output Redirection (Overwrite and Appending Behavior)
- Tab autocompletion for commands (builtins and PATH executables) and filenames
- Command history and persistence to files
- Pipelines for chaining multiple commands
- Signal Handling

## Requirements
- Linux or macOS
- GCC/Clang or any C compiler
- The readline GNU library

## Quick Start
```bash
git clone https://github.com/Affan009/shell-clone.git
cd shell-clone
make
./ash
```

## Future Improvements
- Jobs Control

## References
- [Write a Shell in C - Stephan Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [Write a Shell in C - danishpraka.sh](https://danishpraka.sh/posts/write-a-shell/)
- [Writing a UNIX Shell - Indradhanush Gupta](https://igupta.in/blog/writing-a-unix-shell-part-1/)
- [CodeCrafters](https://app.codecrafters.io/catalog)
