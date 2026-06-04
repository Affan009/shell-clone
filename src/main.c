#include "shell.h"

int main(int argc, char *argv[]) {
    (void)argc, (void)argv; // Unused parameters
    sh_loop();
    return EXIT_SUCCESS;
}
