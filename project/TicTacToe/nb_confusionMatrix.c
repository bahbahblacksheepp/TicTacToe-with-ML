/* start of include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* end of include */

/* MACROS */
#define MAX 958
#define TRAINLIMIT 766
#define TESTLIMIT 192
#define DEPTH 9
#define ROW 3
#define COL 2
/* END OF MACROS */

/* FUNCTION PROTOTYPES */
void extractData(char extractedData[MAX][DEPTH], char result[MAX][DEPTH]);
void shuffle(char extractedData[MAX][DEPTH], char result[MAX][DEPTH]);
void learn(char trainData[TRAINLIMIT][DEPTH], char trainResult[TRAINLIMIT][DEPTH], double winProbability[COL], double featureProbability[DEPTH][ROW][COL]);
void split(char extractedData[MAX][DEPTH], char result[MAX][DEPTH]
, char trainData[TRAINLIMIT][DEPTH], char testData[TESTLIMIT][DEPTH], char trainResult[TRAINLIMIT][DEPTH], char testResult[TESTLIMIT][DEPTH]);
const char* predict(double winProbability[COL], double featureProbability[DEPTH][ROW][COL], char instance[DEPTH]);
int nextMove(double winProbability[COL], double featureProbability[DEPTH][ROW][COL], char instance[DEPTH]);
void testErrorProbability(int errorMatrix[4], char testData[TESTLIMIT][DEPTH], char testResult[TESTLIMIT][DEPTH], double winProbability[2], double featureProbability[DEPTH][ROW][COL]);
void trainErrorProbability(int errorMatrix[4], char testData[TRAINLIMIT][DEPTH], char testResult[TRAINLIMIT][DEPTH], double winProbability[2], double featureProbability[DEPTH][ROW][COL]);

int main(){
    srand(time(0)); // ensures that the random shuffle is really random on every run
    double winProbability[COL] = {0};
    /*
    will be used to hold win and loss percentage of the learn dataset.
    winProbability[0] = win percentage
    winProbability[1] = loss percentage
    */
    double featureProbability[DEPTH][ROW][COL] = {0};
    /*
    Nested array to hold 9 values each with 3 values and each of these values with 2...
    9 values for the 9 boxes in a tic-tac-toe board.
    each box will have 3 values 'b', 'x', 'o'.
    each value in each square will have its own win/loss percentage
    */
    char extractedData[MAX][DEPTH]; // extracted data that holds the tic-tac-toe table information
    char result[MAX][DEPTH]; // extracted data that holds the corresponding 'positive' or 'negative'
    char trainData[TRAINLIMIT][DEPTH], testData[TESTLIMIT][DEPTH];
    char trainResult[TRAINLIMIT][DEPTH], testResult[TESTLIMIT][DEPTH];
    int errorMatrix[4];
    /*
    errorMatrix[0]: True Positive
    errorMatrix[1]: True Negative
    errorMatrix[2]: False Positive
    errorMatrix[3]: False Negative
    */
    extractData(extractedData, result); // runs the extraction function
    shuffle(extractedData, result); // runs the shuffle function which is to be done before partitioning the dataset into 80% and 20%
    split(extractedData, result, trainData, testData, trainResult, testResult);
    learn(trainData, trainResult, winProbability, featureProbability); // runs the learning function to get the probabilities 
    testErrorProbability(errorMatrix, testData, testResult, winProbability, featureProbability);
    trainErrorProbability(errorMatrix, trainData, trainResult, winProbability, featureProbability);

    int ch = 0;
    do {
        puts("\nPress any key to exit...");
    }
    while((ch = getchar()) == EOF);

    return 0;
}

/*
function to find error probability
*/
void testErrorProbability(int errorMatrix[4], char testData[192][DEPTH], char testResult[192][DEPTH], double winProbability[COL], double featureProbability[DEPTH][ROW][COL]) {
    char result[DEPTH];    
    for (int i = 0; i < TESTLIMIT; i++) {
        strcpy(result, predict(winProbability, featureProbability, testData[i]));
        if (strcmp(testResult[i], result) == 0) { // if same
            if (strcmp(testResult[i], "positive") == 0) { // if it was the same and is a positive
                errorMatrix[0] ++;
            } else {errorMatrix[1] ++;}
        } else {
            if (strcmp(testResult[i], "positive") == 0) { // if it is not the same and is a positive
                errorMatrix[2] ++;
            } else {errorMatrix[3] ++;}
        }
    }
    printf("---------- 'test data' error probability ----------\n");
    printf("true positive: %d \n", errorMatrix[0]);
    printf("true negative: %d \n", errorMatrix[1]);
    printf("false positive: %d \n", errorMatrix[2]);
    printf("false negative: %d \n", errorMatrix[3]);
    printf("total errors: %d \n", errorMatrix[2] + errorMatrix[3]);
    double probabilityError = ((errorMatrix[2] + errorMatrix[3])/192.0);
    printf("probability of error: %f\n", probabilityError);
}

void trainErrorProbability(int errorMatrix[4], char testData[TRAINLIMIT][DEPTH], char testResult[TRAINLIMIT][DEPTH], double winProbability[2], double featureProbability[DEPTH][ROW][COL]) {
    char result[DEPTH];    
    for (int i = 0; i < TRAINLIMIT; i++) {
        strcpy(result, predict(winProbability, featureProbability, testData[i]));
        if (strcmp(testResult[i], result) == 0) { // if same
            if (strcmp(testResult[i], "positive") == 0) { // if it was the same and is a positive
                errorMatrix[0] ++;
            } else {errorMatrix[1] ++;}
        } else {
            if (strcmp(testResult[i], "positive") == 0) { // if it is not the same and is a positive
                errorMatrix[2] ++;
            } else {errorMatrix[3] ++;}
        }
    }
    printf("---------- 'train data' error probability ----------\n");
    printf("true positive: %d \n", errorMatrix[0]);
    printf("true negative: %d \n", errorMatrix[1]);
    printf("false positive: %d \n", errorMatrix[2]);
    printf("false negative: %d \n", errorMatrix[3]);
    printf("total errors: %d \n", errorMatrix[2] + errorMatrix[3]);
    double probabilityError = ((errorMatrix[2] + errorMatrix[3])/766.0);
    printf("probability of error: %f\n", probabilityError);
}

/* function to split the dataset into 80% learning and 20% testing */
void split(char extractedData[MAX][DEPTH], char result[MAX][DEPTH]
, char trainData[TRAINLIMIT][DEPTH], char testData[TESTLIMIT][DEPTH], char trainResult[TRAINLIMIT][DEPTH], char testResult[TESTLIMIT][DEPTH]) {
    for (int i = 0; i < MAX; i++) {
        if (i < TRAINLIMIT) {
            strcpy(trainResult[i], result[i]);
            for (int k = 0; k < DEPTH;k++) {
                trainData[i][k] = extractedData[i][k];
            }
        } if (i >= TRAINLIMIT ) {
            strcpy(testResult[i % TRAINLIMIT], result[i]);
            for (int k = 0; k < DEPTH;k++) {
                testData[i % TRAINLIMIT][k] = extractedData[i][k];
            }
        }
    }
}


/* predict function will return the predicted result of the board whether it will end in a win or loss for 'x' */
const char* predict(double winProbability[COL], double featureProbability[DEPTH][ROW][COL], char instance[DEPTH]) { // prediction formula
    double probability[COL] = {0}; // create a new array to hold the prediction for the given tic-tac-toe instance
    for (int k = 0; k < COL; k++) {
        probability[k] = winProbability[k]; // sets the values of the array to the label probability... (the base chance of winning or losing based on the dataset given)
    }
    for (int i = 0; i < DEPTH; i++) { // loop through the instance
        switch (instance[i]) { // grab the current instance
            case 'b': // if its blank
            probability[0] *= featureProbability[i][0][0]; // multiply the current 'positive' probability with the 'positive' probability of when 'b' is in that position
            probability[1] *= featureProbability[i][0][1]; // multiply the current 'negative' probability with the 'negative' probability of when 'b' is in that position
            break;
            case 'x':
            probability[0] *= featureProbability[i][1][0]; // same as above but if its 'x'
            probability[1] *= featureProbability[i][1][1];
            break;
            case 'o':
            probability[0] *= featureProbability[i][2][0]; // same as above but if its 'o'
            probability[1] *= featureProbability[i][2][1];
            break;
        }
    }
    printf("chance of 'x' win: %f, chance of 'o' win: %f\n", probability[0], probability[1]);
    if (probability[0] > probability[1]) { // if the value of 'positive' is higher than 'negative' then the current instance will most likely result in a win. 
        return "positive";
    } else return "negative"; // else, it wil result in a loss.

}

void learn(char trainData[TRAINLIMIT][DEPTH], char trainResult[TRAINLIMIT][DEPTH], double winProbability[COL], double featureProbability[DEPTH][ROW][COL]) { // function for learning from the dataset.
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
                /*
                e.g. if the given row is "o,x,x,x,o,o,o,x,x, negative" for example,
                add 1 to 'negative' in the winProbability array.
                then for each square and its state add 1 to 'negative' in the featureProbability.
                so in the first square o will have 1 to 'negative'... and so on...
                
                */
             }
        }   
    }
    printf("No of positives: %f, No of negatives: %f\n", winProbability[0], winProbability[1]);
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
    printf("%% of positives: %f, %% of negatives: %f\n", winProbability[0], winProbability[1]);
}

/* shuffles the dataset to ensure a different set of training and testing data (avoiding bias) */
void shuffle(char extractedData[MAX][DEPTH], char result[MAX][DEPTH]) { // function to shuffle the dataset around
    //makes use of the random function
    char tempData[DEPTH]; // create a temp array to hold the data
    char tempResult[DEPTH]; // create a temp array to hold the data
    int r;
    for (int i = 0; i < MAX; i++) { // loop through the whole dataset
        // start from the first row in the data
        r = rand() % MAX; // set range to 0 - 958 and take a random number to swap with the row
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

/* extract data! */
void extractData(char extractedData[MAX][DEPTH], char result[MAX][DEPTH]) {
    FILE *data;
    data = fopen("resources/tic-tac-toe.data", "rb");
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