#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logic.h"
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

/// Main function
int main() {
    srand(time(0));  // Seed the random number generator

    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int num_words = read_words(words, "words.txt");
    if (num_words == 0) {
        return 1;
    }

    int num_players;
    printf("Enter the number of players: ");
    scanf("%d", &num_players);

    sqlite3* db = open_database("high_scores.db");
    if (!db) {
        return 1; // Err 
    }
   
    for (int player = 0; player < num_players; player++) {
        printf("Player %d's turn\n", player + 1);

        char name[100];
        printf("Enter your name: ");
        scanf("%s", name);

        int difficulty;
        printf("Enter your dificulty level (1-easy, 2- Medium, 3-Hard):\n");
        scanf("%d", &difficulty);

        int rounds = MAX_ROUNDS;
        if(difficulty == 1){
            rounds = 2;
        }
        else if (difficulty == 2){
            rounds = 4;
        }
        else if (difficulty == 3){
            rounds = 6;
        }

        int time_limit;
        printf("Enter time limit in seconds: ");
        scanf("%d", &time_limit); 

        double total_speed = 0.0;
        int game_start_time = clock();
        for (int round = 0; round < rounds; round++) {
            int current_time = clock();
            if((current_time - game_start_time) / CLOCKS_PER_SEC > time_limit){
                printf("Time limit exceeded. Game over!!. \n");
                break;
            }
            
            int random_index = rand() % num_words;  // Generate a random index
            char* random_word = words[random_index];  // Select a random word

            printf("Round %d: Your word is: %s\n", round + 1, random_word);
            printf("Type this word as fast as you can and press enter.\n");

            char input[MAX_WORD_LENGTH];
            int round_start_time = clock();  // Record the start time
            scanf("%s", input);  // Get the user's input
            int round_end_time = clock();  // Record the end time

            if (strcmp(input, random_word) != 0) {
                printf("Incorrect input. Skipping this round.\n");
                continue;
            }

            double speed = calculateSpeed(round_start_time, round_end_time, strlen(random_word));
            printf("Your typing speed was %.2f characters per minute.\n", speed);
            total_speed += speed;
        }

        double average_speed = total_speed / rounds;
        printf("Your average typing speed was %.2f characters per minute.\n", average_speed);

        update_high_score(db, name, average_speed);
    }
    close_database(db);

    return 0;
}
