#include "prototypes.h"


// Naive Bayes (extract data)
void extractData() {
    FILE *data;
    data = fopen("resources/tic-tac-toe.data", "r");
    if (data == NULL) { // if file not opened
        printf("Error opening datafile...");
    }

    for (int i = 0; i < MAX; i++) {
        fscanf(data, "%c,%c,%c,%c,%c,%c,%c,%c,%c,%s ", &extractedData[i][0]
        , &extractedData[i][1]
        , &extractedData[i][2]
        , &extractedData[i][3]
        , &extractedData[i][4]
        , &extractedData[i][5]
        , &extractedData[i][6]
        , &extractedData[i][7]
        , &extractedData[i][8]
        , result[i]
        );  
        
    }
    fclose(data);
}

// Naive Bayes (shuffle data)
/*
Enables the bot to play differently with every iteration of the game
(Works in conjunction with the split function)
*/
void shuffle() { 
    //makes use of the random function
    srand(time(0));
    char tempData[DEPTH]; // create a temp array to hold the data
    char tempResult[DEPTH]; // create a temp array to hold the data
    for (int i = 0; i < MAX; i++) { // loop through the whole dataset
        // start from the first row in the data
        int r = rand() % MAX; // set range to 0 - 958 and take a random number to swap with the row
        for (int k = 0; k < DEPTH; k++) { // loop through the 9 squares
            tempData[k] = extractedData[i][k]; // assign current row values to temp
            extractedData[i][k] = extractedData[r][k]; // assign randomized row values to current
            extractedData[r][k] = tempData[k]; // assign temp value to randomized row
        }   
        strcpy(tempResult, result[i]); //assign current row values to temp
        strcpy(result[i], result[r]); // assign randomized row values to current
        strcpy(result[r], tempResult); // assign temp value to randomized row
    }
}

// Naive Bayes (split data)
/*
Provides the learn function with a randomized set of data which will affect
the win/loss probability in every iteration.
(Allows the bot to be different every game)
*/
void split() {
    for (int i = 0; i < MAX;i++) {
        if (i < 766) {
            strcpy(trainResult[i], result[i]);
            for (int k = 0; k < DEPTH;k++) {
                trainData[i][k] = extractedData[i][k];
            }
        } else {
            strcpy(testResult[i % 766], result[i]);
            for (int k = 0; k < DEPTH;k++) {
                testData[i % 766][k] = extractedData[i][k];
            }
        }
    }
}

// Naive Bayes (learning from the training dataset)
void learn() { // function for learning from the dataset.
    int toIncrement = 0; // sets an int value for indexing later for 'positive' and 'negative' (0 for positive, 1 for negative)
    for (int i = 0; i < TRAINLIMIT; i++) {
        if (strcmp(trainResult[i], "positive") == 0) { // if the result is 'positive' of the current row
            winProbability[0]++; // add 1 to the postive count to the list
            toIncrement = 0; // set it so that for all the choices for each square of the current row will be added with a positive label
        }
        if (strcmp(trainResult[i], "negative") == 0) { // if the result is 'negative' of the current row
            winProbability[1]++; // add 1 to the negative count to the list
            toIncrement = 1; // set it so that for all the choices for each square of the current row will be added with a negative label
        }
        for (int k = 0; k < DEPTH; k++) { // loop through all 9 slots of the current row
             switch (trainData[i][k]) {
                case 'b': // if blank add
                featureProbability[k][0][toIncrement] ++; 
                break;
                case 'x': // if x add
                featureProbability[k][1][toIncrement] ++;
                break;
                case 'o': // if o add
                featureProbability[k][2][toIncrement] ++;
                break;
             }
        }   
    }
    // calculate conditonal probability
    for (int i = 0; i < DEPTH; i++) { // loop through each square
        for (int k = 0; k < ROW; k ++) { // loop through each state blank, 'x', 'o'
            for (int p = 0; p < COL; p ++) { // loop through the 'positive' and 'negative'
                featureProbability[i][k][p] /= winProbability[p]; // take the no of 'positives' of each square and each state, divide it by the overall 'positives' of the dataset... same for 'negative'
            }
        }
    }
    // calculate outcome probability
    for (int i = 0; i < COL; i++) {
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
            switch (boardPtr[i][k]) { // grab the current instance
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

// Naive Bayes (To get the next move for the AI) made for Tic-Tac-Toe integration
void nextMove() { // get next move
    int bestMoveX = -1;
    int bestMoveY = -1;
    double probability[COL] = {0}; // create a new array to hold the prediction for the given tic-tac-toe instance
    predict(); // call prediction, fills up the moveProbability
    for (int i = 0; i < COL; i ++) {
        probability[i] = moveProbability[i]; // assign the probabilities to local array
    }
    for (int i = 0; i < ROW; i ++) { // begin loop to look through board
        for (int k = 0; k < ROW; k ++) {
        if (boardPtr[i][k] == 0) { // if its a blank space
            if (bestMoveX == -1 && bestMoveY == -1) {
                // Default to the first empty cell found
                bestMoveX = i;
                bestMoveY = k;
            }
            boardPtr[i][k] = 2; // set it as the AI
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
            boardPtr[i][k] = 0;
        }
        }
    }

    if (gameOver == FALSE && numArray.freeSpaces ) {
        boardPtr[bestMoveX][bestMoveY] = numArray.AI;
        --numArray.freeSpaces;
        NextTurn();
    }
    
}