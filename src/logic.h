#ifndef LOGIC_H
#define LOGIC_H

#include <sqlite3.h>

#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

int read_words(char words[MAX_WORDS][MAX_WORD_LENGTH], const char* filename);
double calculateSpeed(int start_time, int end_time, int word_length);
sqlite3* open_database(const char* filename);
void close_database(sqlite3* db);
void update_high_score(sqlite3* db, const char* name, double speed);

#endif // LOGIC_H
