# Autocompletion
Shell automatically suggests possible matches and completes partial input commands and filenames.

### Important Note
This implementation involves using the **readline** function and handing it custom completion logic. Future work can involve implementing **readline** from scratch.

### Goals
- Implement command (builtin or PATH executable).
- Implement filename and directory completion for arguments.
- Support multiple and partial completions.
- Support multi-argument completion.

## Common Details

## Design Overview
- Pressing the `<TAB>` key would trigger autocompletion.
- Command completion occurs for the first argument, and filename completion for subsequent completion.
- If there is a single match for completion, it is inserted.
- If there is no match for completion, nothing is inserted.
- If there are multiple matches for completion, the Longest Common Prefix (LCP) is inserted and a sorted list of all matches is shown.

Important Note: The **readline** function would handle the logic for the LCP and the sorted match list.

## Command Completion

### Design Specifics
- Searches builtins first, and then PATH executables.

### Implementation Details
// TO-DO

## Filename Completion

### Implementation Details
// TO_DO
