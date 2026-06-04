#ifndef HISTORY_H
#define HISTORY_H

#define HISTSIZE 1000
#define HISTFILE ".ash_history"

void init_history();
int physical_index(int logical_index);
int get_hist_count();
const char* get_hist_entry(int index);
void free_history();

int prev_hist(int count, int key);
int next_hist(int count, int key);

void add_hist(const char* line);
void read_hist(const char* path);
void write_hist(const char* path);
void append_hist(const char* path);

#endif
