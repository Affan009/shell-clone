# Background Jobs

Shell runs a terminal command in the foreground. A background job is a command that is run without blocking the shell.

### Goals
- Implement background job execution with unblocking behavior.
- Implement the jobs builtin and the job table.
- Implement handler for SIGCHLD signal to reap finished background jobs.

## Design
- A command suffixed with `&` should run be executed in the background, without the shell waiting for it in the foreground.
- The jobs builtin should show the jobs currently being tracked in the background.
- These jobs should be reaped after they finish execution, when they fire the SIGCHLD signal.
