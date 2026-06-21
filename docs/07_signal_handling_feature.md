# Signal Handling

Shell handles signals and interrupts and prevents them from affecting the shell session.

### Goals
- Detect and handle the terminal interrupt signal (`SIGINT`).
- Detect and handle the terminal suspend signal (`SIGTSTP`).

## Design
- `Ctrl + C` triggers the `SIGINT` signal and should be handled by a custom handler. The signal should kill the child process executing and not affect the shell process.
- `Ctrl + Z` triggers the `SIGTSTP` signal and should be ignored by the shell process, while suspending the child process executing.
- The handler for `SIGINT` should write a newline to the terminal (`STDOUT`) and resume the prompting in the REPL.

## Implementation Details
- The sigaction struct is configured before the REPL.
- The sigaction struct is set with the `SA_RESTART` flag that resumes interrupted system calls.
- The sigaction struct is set with a custom handler for `SIGINT` and `SIG_IGN` (Ignore signal) for `SIGTSTP`.
- The custom handler prints a newline using `write()` for async-safety. It jumps to a signal set point managed by `env` variable when `jump_active` variable is set.
- The `jump_active` variable is set before the `readline` call and disabled during execution.
- The handlers are reset inside the child process to `SIG_DFL` that react to the signal in the intended default manner.

### Important Note
- Handling of `SIGTSTP` is incomplete, and would be expanded upon when jobs are implemented.

### Variables
| Variable | Type | Description |
|----------|------------|--------------|
|`env`|`sigjmp_buf`|Jump buffer storing the prompt restore point|
|`jump_active`|`volatile sig_atomic_t`|Flag to signal whether to jump to the restore point|

### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`init_handlers()`|None (Void)|None (Void)|Sets the flags and handlers for the signals using sigaction|
|`sigint_handler(sig)`|`int` — an integer parameter required to match function signature for sigaction|None (Void)|Prints newline character to stdout and jumps to the signal set point in the REPL loop when `jump_active` is set|
|`reset_handlers()`|None (Void)|None (Void)|Resets the flags and handlers for the signals using sigaction|

***
