#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
/* MACROS */
#define MAX 958
#define TRAINLIMIT 766
#define TESTLIMIT 192
#define DEPTH 9
#define ROW 3
#define COL 2
/* END OF MACROS */
void resetBoard();
int checkFreeSpaces();
void playerMove();
void computerMove(int difficulty);
int checkWinner();
void simulateGame(int difficulty);
/* Naive Bayes */
void extractData();
void shuffle();
void split();
void learn();
void predict();
void nextMove();

/* End of Prototypes */

int minimax(int board[3][3], int depth, int isMaximizing, int maxDepth, int isQuick);
int evaluate(int );

int board[3][3]= {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

int AI = 2;// 'O'
int turn = 1;// 'X'


int winner = 0;
int player1Wins = 0;
int player2Wins = 0;
int draws = 0;
double winProbability[2] = {0};                   // Overall win/loss probability list
double featureProbability[DEPTH][ROW][COL] = {0}; // win/loss probability for each square and each configuration (x,o and blank)
char extractedData[MAX][DEPTH];                   // List to hold board information e.g. "b,b,b,b,b,b,b,b,b"
char result[MAX][DEPTH];                          // List to hold board win/loss information
char trainData[TRAINLIMIT][DEPTH], testData[TESTLIMIT][DEPTH];
char trainResult[TRAINLIMIT][DEPTH], testResult[TESTLIMIT][DEPTH];
double moveProbability[2] = {0}; // made for Tic Tac Toe
/* End of Global Variables */
int difficulty;
int main()
{
    int simulations = 1000;

    extractData();
    shuffle();
    split();
    learn();
    printf("Choose difficulty (1 for Medium, 2 for Impossible, 3 for ML): ");
    scanf("%d", &difficulty);
    for (int i = 0; i < simulations; i++)
    {
        simulateGame(difficulty);
    }

    printf("After %d simulations:\n", simulations);
    printf("Player 1 (siumlated player) wins: %d\n", player1Wins);
    printf("Player 2 (Minimax/ML) wins: %d\n", player2Wins);
    printf("Draws: %d\n", draws);

    while ((getchar()) != '\n');
    puts("\nPress any key to exit...");
    getchar();

    return 0;
}

void resetBoard()
{
    if (difficulty == 3) {
        shuffle();
        split();
        learn();
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = 0;
        }
    }
}

void simulateGame(int difficulty)
{
    resetBoard();
    winner = 0;
    int turn = 0; // 0 for Player 1 (random), 1 for Player 2 (Minimax)

    while (winner == 0 && checkFreeSpaces() != 0)
    {
        if (turn == 0)
        {
            playerMove(turn); // Random move for Player 1
            turn = 1;
        }
        else
        {
            computerMove(difficulty); // Minimax move for Player 2
            turn = 0;
        }
        winner = checkWinner();
    }

    // Track the result of each game
    if (winner == turn)
        player1Wins++;
    else if (winner == AI)
        player2Wins++;
    else
        draws++;
}

void playerMove()
{
    int row, col;
    do
    {
        row = rand() % 3;
        col = rand() % 3;
    } while (board[row][col] != 0);
    board[row][col] = 1;
}

void computerMove(int difficulty)
{
    if (difficulty == 1 || difficulty == 2)
    {

        int maxDepth;
        int bestScore = -1000;
        int bestMoveX = -1;
        int bestMoveY = -1;
        if (difficulty == 1)
        {
            maxDepth = 3;
        }
        else
            maxDepth = 9;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = AI;
                    int score = minimax(board, 0, 0, maxDepth, 1);
                    board[i][j] = 0;
                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestMoveX = i;
                        bestMoveY = j;
                    }
                }
            }
        }
        board[bestMoveX][bestMoveY] = AI;
    }
    else if (difficulty == 3)
    { 
    int bestMoveX = -1;
    int bestMoveY = -1;
    double probability[COL] = {0}; // create a new array to hold the prediction for the given tic-tac-toe instance
    predict(); // call prediction, fills up the moveProbability
    for (int i = 0; i < COL; i ++) {
        probability[i] = moveProbability[i]; // assign the probabilities to local array
    }
    for (int i = 0; i < ROW; i ++) { // begin loop to look through board
        for (int k = 0; k < ROW; k ++) {
        if (board[i][k] == 0) { // if its a blank space
            if (bestMoveX == -1 && bestMoveY == -1) {
                // Default to the first empty cell found
                bestMoveX = i;
                bestMoveY = k;
            }
            board[i][k] = 2; // set it as the AI
            predict(); // run the predict again
            int betterMove = (moveProbability[1] > probability[1]) ||
                                 (moveProbability[1] == probability[1] && moveProbability[0] < probability[0]);
            /*
            if new 'o' win higher probability than current 'o' win
            OR
            if new 'o' win same probability than current 'o' win AND 'x' win is lower probability than current 'x' win.
            */                   
            if (betterMove) {
                for (int j = 0; j < COL; j ++) {
                    probability[j] = moveProbability[j]; // assign the probabilities to local array
                }
                bestMoveX = i;
                bestMoveY = k;
            } 
            board[i][k] = 0;
        }
        }
    }
        board[bestMoveX][bestMoveY] = AI;
    }
}

int minimax(int board[3][3], int depth, int isMaximizing, int maxDepth, int isQuick)

{
    if (depth == maxDepth)
    {
        return 0;
    }

    int score = evaluate(checkWinner());

    if (score == 10 || score == -10)
    {
        if (isQuick)
        {
            return score - depth;
        }
        else
        {
            return score;
        }
    }

    if (checkFreeSpaces() == 0)
    {
        return 0;
    }

    if (isMaximizing)
    {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = AI;
                    int score = minimax(board, depth + 1, 0, maxDepth, isQuick);
                    board[i][j] = 0;
                    bestScore = (score > bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = 1000;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = turn;
                    int score = minimax(board, depth + 1, 1, maxDepth, isQuick);
                    board[i][j] = 0;
                    bestScore = (score < bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
}

int evaluate(int )
{
    // Iterate through grid to check for row and column win
    for (int i = 0; i < 3; i++)
    {
        // Check row win
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            if (board[i][0] == 2)
                return 10;
            else if (board[i][0] == 1)
                return -10;
        }

        // Check column win
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            if (board[0][i] == AI)
                return 10;
            else if (board[0][i] == turn)
                return -10;
        }
    }

    // Check diagonal top left to bottom right
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if (board[0][0] == AI)
            return 10;
        else if (board[0][0] == turn)
            return -10;
    }

    // Check diagonal top right to bottom left
    else if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        if (board[0][2] == AI)
            return 10;
        else if (board[0][2] == turn)
            return -10;
    }

    return 0;
}

int checkWinner()
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2])
            return board[i][0];
        if (board[0][i] == board[1][i] && board[0][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[0][0] == board[2][2])
        return board[0][0];
    if (board[0][2] == board[1][1] && board[0][2] == board[2][0])
        return board[0][2];
    return 0;
}

int checkFreeSpaces()
{
    int freeSpaces = 9;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != 0)
            {
                freeSpaces -= 1;
            }
        }
    }
    return freeSpaces;
}
void extractData()
{
    FILE *data;
    data = fopen("resources/tic-tac-toe.data", "rb");
    if (data == NULL)
    { // if file not opened
        printf("Error opening datafile...");
    }

    for (int i = 0; i < MAX; i++)
    {
        fscanf(data, "%c,%c,%c,%c,%c,%c,%c,%c,%c,%s ", &extractedData[i][0], &extractedData[i][1], &extractedData[i][2], &extractedData[i][3], &extractedData[i][4], &extractedData[i][5], &extractedData[i][6], &extractedData[i][7], &extractedData[i][8], result[i]);
    }
    fclose(data);
}
void shuffle()
{
    // makes use of the random function
    srand(time(0));
    char tempData[DEPTH];   // create a temp array to hold the data
    char tempResult[DEPTH]; // create a temp array to hold the data
    for (int i = 0; i < MAX; i++)
    { // loop through the whole dataset
        // start from the first row in the data
        int r = rand() % MAX; // set range to 0 - 958 and take a random number to swap with the row
        for (int k = 0; k < DEPTH; k++)
        {                                              // loop through the 9 squares
            tempData[k] = extractedData[i][k];         // assign current row values to temp
            extractedData[i][k] = extractedData[r][k]; // assign randomized row values to current
            extractedData[r][k] = tempData[k];         // assign temp value to randomized row
        }
        strcpy(tempResult, result[i]); // assign current row values to temp
        strcpy(result[i], result[r]);  // assign randomized row values to current
        strcpy(result[r], tempResult); // assign temp value to randomized row
    }
}
void split()
{
    for (int i = 0; i < MAX; i++)
    {
        if (i < 766)
        {
            strcpy(trainResult[i], result[i]);
            for (int k = 0; k < DEPTH; k++)
            {
                trainData[i][k] = extractedData[i][k];
            }
        }
        else
        {
            strcpy(testResult[i % 766], result[i]);
            for (int k = 0; k < DEPTH; k++)
            {
                testData[i % 766][k] = extractedData[i][k];
            }
        }
    }
}
void learn()
{                        // function for learning from the dataset.
    int toIncrement = 0; // sets an int value for indexing later for 'positive' and 'negative' (0 for positive, 1 for negative)
    for (int i = 0; i < TRAINLIMIT; i++)
    {
        if (strcmp(trainResult[i], "positive") == 0)
        {                        // if the result is 'positive' of the current row
            winProbability[0]++; // add 1 to the postive count to the list
            toIncrement = 0;     // set it so that for all the choices for each square of the current row will be added with a positive label
        }
        if (strcmp(trainResult[i], "negative") == 0)
        {                        // if the result is 'negative' of the current row
            winProbability[1]++; // add 1 to the negative count to the list
            toIncrement = 1;     // set it so that for all the choices for each square of the current row will be added with a negative label
        }
        for (int k = 0; k < DEPTH; k++)
        { // loop through all 9 slots of the current row
            switch (testData[i][k])
            {
            case 'b': // if blank add
                featureProbability[k][0][toIncrement]++;
                break;
            case 'x': // if x add
                featureProbability[k][1][toIncrement]++;
                break;
            case 'o': // if o add
                featureProbability[k][2][toIncrement]++;
                break;
            }
        }
    }
    // calculate conditonal probability
    for (int i = 0; i < DEPTH; i++)
    { // loop through each square
        for (int k = 0; k < ROW; k++)
        { // loop through each state blank, 'x', 'o'
            for (int p = 0; p < COL; p++)
            {                                                     // loop through the 'positive' and 'negative'
                featureProbability[i][k][p] /= winProbability[p]; // take the no of 'positives' of each square and each state, divide it by the overall 'positives' of the dataset... same for 'negative'
            }
        }
    }
    // calculate outcome probability
    for (int i = 0; i < COL; i++)
    {
        winProbability[i] /= TRAINLIMIT; // probability of it being a positive or negative in relation to the dataset.
    }
}
// Naive Bayes (uses the board configuration to get the win/loss probability)
void predict() { // prediction formula
    for (int k = 0; k < COL; k++) {
        moveProbability[k] = winProbability[k]; // sets the values of the array to the label probability... (the base chance of winning or losing based on the dataset given)
    }
    for (int i = 0; i < ROW; i++) { // loop through the instance
        for (int k = 0; k < ROW; k++) {
            switch (board[i][k]) { // grab the current instance
                case 0: // if its blank
                moveProbability[0] *= featureProbability[(i*ROW)+k][0][0]; // multiply the current 'positive' probability with the 'positive' probability of when 'b' is in that position
                moveProbability[1] *= featureProbability[(i*ROW)+k][0][1]; // multiply the current 'negative' probability with the 'negative' probability of when 'b' is in that position
                break;
                case 1:
                moveProbability[0] *= featureProbability[(i*ROW)+k][1][0]; // same as above but if its 'x'
                moveProbability[1] *= featureProbability[(i*ROW)+k][1][1];
                break;
                case 2:
                moveProbability[0] *= featureProbability[(i*ROW)+k][2][0]; // same as above but if its 'o'
                moveProbability[1] *= featureProbability[(i*ROW)+k][2][1];
                break;
            }
        }
    }
    /*
    to be used and compared against other probabilities.
    If its higher, then it will be the most likely to make the game
    end in a loss or draw for the player.
    Therefore, a win or draw for the AI.
    */
}