#include "shell.h"
#include "history.h"

static char* history[HISTSIZE];
static int hist_count = 0;
static int hist_index = 0;

void init_history() {
    rl_bind_keyseq("\\e[A", prev_hist);  // up arrow
    rl_bind_keyseq("\\e[B", next_hist);  // down arrow
}

int prev_hist(int count, int key) {
    (void)count, (void)key;  // unused parameters
    if (hist_index > 0) hist_index--;

    rl_replace_line(history[hist_index], 0);
    rl_point = rl_end;
    return 0;
}

int next_hist(int count, int key) {
    (void)count, (void)key;  // unused parameters
    if (hist_index < hist_count) hist_index++;

    if (hist_index >= hist_count) {
        rl_replace_line("", 0);
    } else {
        rl_replace_line(history[hist_index], 0);
    }
    rl_point = rl_end;
    return 0;
}

void add_hist(const char* line) {
    if (hist_count < HISTSIZE) {
        history[hist_count++] = strdup(line);
    } else {
        free(history[0]);
        memmove(history, history + 1, (HISTSIZE - 1) * sizeof(char*));
        history[HISTSIZE - 1] = strdup(line);
    }
    hist_index = hist_count;  // Reset index to end after adding
}

int get_hist_count() {
    return hist_count;
}

const char* get_hist_entry(int index) {
    if (index < 0 || index >= hist_count) {
        return NULL;
    }
    return history[index];
}
