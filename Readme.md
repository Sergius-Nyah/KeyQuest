### Typing Speed Game
This is a simple command-line game written in C that tests your typing speed. It reads a list of words from a file, then randomly selects one for you to type. It measures how quickly you type the word and calculates your typing speed in characters per minute.

## How to Play
- Enter the number of players.
- For each player, you will be asked to enter your name and choose a difficulty level (1-easy, 2-medium, 3-hard). The difficulty level determines the number of rounds you will play.
- You will also be asked to set a time limit in seconds.
- For each round, the game will display a word that you need to type as quickly as possible.
- If you type the word correctly, the game will calculate and display your typing speed for that round.
- If you type the word incorrectly, the round is skipped.
- The game ends when you have completed all rounds or when the time limit is exceeded.
- At the end of the game, your average typing speed across all rounds is calculated and displayed.
### High Scores
The game maintains a database of high scores. If your average typing speed is higher than your previous high score, the database is updated with your new score.

### Building and Running 
This project uses the SQLite3 library to manage the high scores database. You will need to have this library installed to build and run the game.

To build the game, use a C compiler such as gcc:
`gcc -o game main.c logic.c -lsqlite3`

To run the game:

Contributing
Contributions are welcome! Please feel free to submit a pull request.

