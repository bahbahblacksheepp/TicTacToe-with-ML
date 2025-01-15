/* Define a #include guard, is a way to avoid the problem of double inclusion */
#ifndef PROTOTYPES_H
#define PROTOTYPES_H

/* Include Libraries/Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h" // Raylib library header
/* End of Include Libraries */

/* Macros */
#define TRUE 1
#define FALSE 0

// ML Macros
#define MAX 958
#define TRAINLIMIT 766
#define TESTLIMIT 192
#define DEPTH 9
#define ROW 3
#define COL 2
/* End of Macros */


/* Prototypes */
// Gameplay Functions
void playerMove();
void computerMove(int);
int checkWinner(char (*boardPtr)[3]);
void displayWinner(int, int);
void resetBoard(char (*boardPtr)[3]);
void NextTurn();

// Game Functions
void HomeScreen();
void TwoPlayerMode();
void ComputerMode();
void showBoard(Texture2D, Texture2D);
void gameOverHandler();
void gridClickHandler(Rectangle, int, int, Sound);

// Minimax Functions
int minimax(char (*boardPtr)[3], int, int, int, int);
int evaluate(int);

// UI Functions
void title(char *, int, int, int);
void drawTextureMoves(int, int, int, Texture2D, Texture2D);
void button(char *, Rectangle, int);
void showHomeButton();

// Naive Bayes Functions
void extractData();
void shuffle();
void split();
void learn();
void predict();
void nextMove();
/* End of Function Prototypes */


/* Global Variables */
// Constants
const short SCREEN_WIDTH = 800;
const short SCREEN_HEIGHT = 450;
const char FONT_SIZE = 20;
const char TEXT_FONT_SIZE = 18;
const char MULTIPLAYER_MODE = 1;
const char COMPUTER_MODE = 2;

// UI Variables
Texture2D cross, circle;
Sound click;

// Game Variables
char board[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};
char (*boardPtr)[3] = board; //Pointer for board
static char gameOver = FALSE;
struct Numbers {
    unsigned short freeSpaces : 4;
    unsigned short state : 3;       // Screen state: 1 = Home Screen, 2 = 2 Player Mode, 3 = Computer Mode (Medium), 4 = Computer Mode (Impossible), 5 = Computer Mode (ML)
    unsigned short AI : 2;          // 'O'
    unsigned short turn : 2;        // 'X' start first
    unsigned short winner : 2;
};
struct Numbers numArray = {9, 1, 2, 1, 0};

// Points Structure
struct Points {
    unsigned int wins;
    unsigned int losses;
    unsigned int draws;
};
static struct Points player1, player2;
struct Points *player1Ptr = &player1; // Pointer structure for player 1
struct Points *player2Ptr = &player2; // Pointer structure for player 2 (multiplayer)
int pointFlag = FALSE;
static int buttonDelay = FALSE;

// Naive Bayes Variables
double winProbability[2] = {0}; // Overall win/loss probability list
double featureProbability[DEPTH][ROW][COL] = {0}; // win/loss probability for each square and each configuration (x,o and blank)
char extractedData[MAX][DEPTH]; // List to hold board information e.g. "b,b,b,b,b,b,b,b,b"
char result[MAX][DEPTH]; // List to hold board win/loss information
char trainData[TRAINLIMIT][DEPTH], testData[TESTLIMIT][DEPTH];
char trainResult[TRAINLIMIT][DEPTH], testResult[TESTLIMIT][DEPTH];
double moveProbability[2] = {0}; // made for Tic Tac Toe
/* End of Global Variables */
#endif
/* End of #include guard */



