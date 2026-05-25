#ifndef REDIRECTION_H
#define REDIRECTION_H

#define OVERWRITE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define APPEND_FLAGS (O_WRONLY | O_CREAT | O_APPEND)

int handle_redirection(char** args, int* redirected_fd);

#endif
