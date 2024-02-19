#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>

#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

// Function to read words from a file
int read_words(char words[MAX_WORDS][MAX_WORD_LENGTH], const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return 0;
    }

    int i = 0;
    while (fscanf(file, "%s", words[i]) != EOF && i < MAX_WORDS) {
        i++;
    }

    fclose(file);

    return i;
}

// Function to calculate typing speed
double calculateSpeed(int start_time, int end_time, int word_length) {
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // Calculate elapsed time in seconds
    double speed = (word_length / elapsed_time) * 60;  // Calculate speed in characters per minute
    return speed;
}

// Function to open a SQLite database
sqlite3* open_database(const char* filename) {
    sqlite3* db;
    if (sqlite3_open(filename, &db)) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

// Function to close a SQLite database
void close_database(sqlite3* db) {
    sqlite3_close(db);
}

// Function to update high scores in a SQLite database
void update_high_score(sqlite3* db, const char* name, double speed) {
    char sql[200];
    sprintf(sql, "INSERT INTO HighScores (Name, Speed) VALUES ('%s', %.2f);", name, speed);

    char* error_message = 0;
    if (sqlite3_exec(db, sql, NULL, 0, &error_message) != SQLITE_OK) {
        printf("SQL error: %s\n", error_message);
        sqlite3_free(error_message);
    }
}
