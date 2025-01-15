#include "prototypes.h"

// Minimax algorithm
int minimax(char (*boardPtr)[3], int depth, int isMaximizing, int maxDepth, int isQuick)
{
    // Check if maxDepth reached
    if (depth == maxDepth)
    {
        return 0;
    }

    // Evaluate winner and assign score based on who win
    int score = evaluate(checkWinner(boardPtr));

    // AI win
    if (score == 10)
    {
        if (isQuick)
        {
            return score - depth; // Minus depth to reward for quicker AI wins
        }
        return score;
    }

    // Player win
    if (score == -10)
    {
        if (isQuick)
        {
            return score + depth; // Add depth to delay player wins
        }
        return score;
    }

    // Check if there is free space.
    if (numArray.freeSpaces == 0) // If no free space
    {
        return 0; // Draw
    }

    if (isMaximizing) // maximizing
    {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (boardPtr[i][j] == 0)
                {
                    boardPtr[i][j] = numArray.AI;
                    --numArray.freeSpaces;
                    int score = minimax(boardPtr, depth + 1, 0, maxDepth, isQuick);

                    boardPtr[i][j] = 0;
                    ++numArray.freeSpaces;

                    // Get the maximum score for computer
                    bestScore = (score > bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
    else // minimizing
    {
        int bestScore = 1000;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (boardPtr[i][j] == 0)
                {
                    boardPtr[i][j] = 1;
                    --numArray.freeSpaces;
                    int score = minimax(boardPtr, depth + 1, 1, maxDepth, isQuick);

                    boardPtr[i][j] = 0;
                    ++numArray.freeSpaces;

                    // Get the minimum score for player
                    bestScore = (score < bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
}

// Evaluate the score of the board, this will check if AI or player win.
int evaluate(int player)
{
    switch (player)
    {
    case 1: // Player win
        return -10;
    case 2: // AI win
        return 10;
    default: // No win
        return 0;
    }
}