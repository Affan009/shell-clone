#include "shell.h"
#include "history.h"

static char* history[HISTSIZE];
static int hist_count = 0;
static int hist_start = 0;

static int hist_index = 0; // Tracks the current logical position in history for navigation

void init_history() {
    rl_bind_keyseq("\\e[A", prev_hist);  // up arrow
    rl_bind_keyseq("\\e[B", next_hist);  // down arrow
    read_hist(HISTFILE);
}

int physical_index(int logical_index) { 
    return (hist_start + logical_index) % HISTSIZE;
}

int get_hist_count() {
    return hist_count;
}

const char* get_hist_entry(int index) {
    if (index < 0 || index >= hist_count) {
        return NULL;
    }
    return history[physical_index(index)];
}

void free_history() {
    for (int i = 0; i < hist_count; i++) {
        free(history[physical_index(i)]);
    }
}

int prev_hist(int count, int key) {
    (void)count, (void)key;  // unused parameters
    if (hist_index > 0) hist_index--;

    if (hist_count == 0) {
        rl_replace_line("", 0);
    } else {
        rl_replace_line(get_hist_entry(hist_index), 0);
    }
    rl_point = rl_end;
    return 0;
}

int next_hist(int count, int key) {
    (void)count, (void)key;  // unused parameters
    if (hist_index < hist_count) hist_index++;

    if (hist_index == hist_count) {
        rl_replace_line("", 0);
    } else {
        rl_replace_line(get_hist_entry(hist_index), 0);
    }
    rl_point = rl_end;
    return 0;
}

void add_hist(const char* line) {
    if (hist_count < HISTSIZE) {
        history[physical_index(hist_count)] = strdup(line);
        hist_count++;
    } else {
        free(history[hist_start]);
        history[hist_start] = strdup(line);
        hist_start = (hist_start + 1) % HISTSIZE;
    }
    hist_index = hist_count;  // Reset index to end after adding
}

void read_hist(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return;

    char line[LINE_BUFF_SIZE];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Remove newline
        add_hist(line);
    }
    fclose(file);
}

void write_hist(const char* path) {
    FILE* file = fopen(path, "w");
    if (!file) return;

    for (int i = 0; i < hist_count; i++) {
        fprintf(file, "%s\n", get_hist_entry(i));
    }
    fclose(file);
}

void append_hist(const char* path) {
    FILE* file = fopen(path, "a");
    if (!file) return;

    for (int i = 0; i < hist_count; i++) {
        fprintf(file, "%s\n", get_hist_entry(i));
    }
    fclose(file);
}
