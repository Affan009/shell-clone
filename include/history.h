#ifndef HISTORY_H
#define HISTORY_H

#define HISTSIZE 1000

void init_history();
int prev_hist(int count, int key);
int next_hist(int count, int key);

void add_hist(const char* line);

int get_hist_count();
const char* get_hist_entry(int index);

#endif
