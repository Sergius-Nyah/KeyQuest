#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_logic.h"
#include "sqlite3.h"

#define MAX_WORD_LENGTH 20
#define MAX_WORDS 1000
#define MAX_ROUNDS 5

// Function to open database
sqlite3* open_database(const char* filename) {
    sqlite3* db;
    int rc = sqlite3_open(filename, &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    char* sql = "CREATE TABLE IF NOT EXISTS HighScores(Name TEXT PRIMARY KEY, Score REAL);";
    char* err_msg = 0;

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
        printf("Failed to create table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }

    return db;
}

// Function to update the high score
void update_high_score(sqlite3* db, const char* name, double score) {
    char sql[200];
    sprintf(sql, "INSERT OR REPLACE INTO HighScores(Name, Score) VALUES('%s', %f);", name, score);

    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
        printf("Failed to update high score: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Function to close the database
void close_database(sqlite3* db) {
    sqlite3_close(db);
}

// Function to read words from a file
int read_words(char words[MAX_WORDS][MAX_WORD_LENGTH], const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        printf("Failed to open file: %s\n", file_name);
        return 0;
    }

    int i = 0;
    while (i < MAX_WORDS && fscanf(file, "%s", words[i]) == 1) {
        i++;
    }

    fclose(file);
    return i;
}

// Main function
int main() {
    srand(time(0));  // Seed the random number generator

    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int num_words = read_words(words, "words.txt");
    if (num_words == 0) {
        return 1;
    }

    double total_speed = 0.0;
    for (int round = 0; round < MAX_ROUNDS; round++) {
        int random_index = rand() % num_words;  // Generate a random index
        char* random_word = words[random_index];  // Select a random word

        printf("Round %d: Your word is: %s\n", round + 1, random_word);
        printf("Type this word as fast as you can and press enter.\n");

        char input[MAX_WORD_LENGTH];
        int start_time = clock();  // Record the start time
        scanf("%s", input);  // Get the user's input
        int end_time = clock();  // Record the end time

        if (strcmp(input, random_word) != 0) {
            printf("Incorrect input. Skipping this round.\n");
            continue;
        }

        double speed = calculateSpeed(start_time, end_time, strlen(random_word));
        printf("Your typing speed was %.2f characters per minute.\n", speed);
        total_speed += speed;
    }

    double average_speed = total_speed / MAX_ROUNDS;
    printf("Your average typing speed was %.2f characters per minute.\n", average_speed);

    // Save the high score to a file
    FILE* file = fopen("high_score.txt", "r");
    double high_score;
    if (!file || fscanf(file, "%lf", &high_score) != 1 || average_speed > high_score) {
        file = fopen("high_score.txt", "w");
        if (file) {
            fprintf(file, "%.2f", average_speed);
            fclose(file);
            printf("New high score!\n");
        }
    }

    return 0;
}