#include "prototypes.h"

// Player move
void playerMove()
{
    Rectangle grid[9]; // Array to hold 9 rectangles

    int startX = 310;
    int startY = 100;
    int size = 60;

    // Initialize grid rectangles
    for (int i = 0; i < 9; ++i)
    {
        int row = i / 3;
        int col = i % 3;
        grid[i] = (Rectangle){startX + col * (size), startY + row * (size), size, size};
    }

    if (gameOver == FALSE)
    {
        for (int i = 0; i < 9; ++i)
        {
            int row = i / 3;
            int col = i % 3;
            gridClickHandler(grid[i], row, col, click);
        }
    }
}

// Computer move
void computerMove(int AILevel)
{
    int bestScore = -1000;
    int bestMoveRow = 0;
    int bestMoveCol = 0;
    int maxDepth = AILevel;
    int isQuick = AILevel == 9 ? 1 : 0;

    // Generate random starting index
    int startRow = rand() % 3;
    int startCol = rand() % 3;

    // Check left to right, top to bottom and play move when it is empty
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // Start at random row and column for minimax
            int row = (startRow + i) % 3;
            int col = (startCol + j) % 3;

            if (boardPtr[row][col] == 0)
            {
                // Use freeSpaces and board to evaluate minimax score
                boardPtr[row][col] = numArray.AI;
                --numArray.freeSpaces;
                int score = minimax(boardPtr, 0, 0, maxDepth, isQuick);

                // Undo freeSpaces and board back to original
                boardPtr[row][col] = 0;
                ++numArray.freeSpaces;
                if (score > bestScore)
                {
                    bestScore = score;
                    bestMoveRow = row;
                    bestMoveCol = col;
                }
            }
        }
    }

    if (gameOver == FALSE && numArray.freeSpaces)
    {
        boardPtr[bestMoveRow][bestMoveCol] = numArray.AI;
        --numArray.freeSpaces;
        NextTurn();
    }
}

// Function to switch turns in two-player mode
void NextTurn()
{
    ++numArray.turn;
    if (numArray.turn > 2)
        numArray.turn = 1;
}

// Check if someone has won the game.
// Return 0 if no winner, 1 if player 1 win, 2 if player 2 or AI win
int checkWinner(char (*boardPtr)[3])
{
    // Iterate through grid to check for row and column win
    for (int i = 0; i < 3; i++)
    {
        // Check row win
        if (boardPtr[i][0] != 0 && boardPtr[i][0] == boardPtr[i][1] && boardPtr[i][1] == boardPtr[i][2])
        {
            return boardPtr[i][0];
        }

        // Check column win
        if (boardPtr[0][i] != 0 && boardPtr[0][i] == boardPtr[1][i] && boardPtr[1][i] == boardPtr[2][i])
        {
            return boardPtr[0][i];
        }
    }

    // Check diagonal top left to bottom right
    if (boardPtr[0][0] != 0 && boardPtr[0][0] == boardPtr[1][1] && boardPtr[1][1] == boardPtr[2][2])
    {
        return boardPtr[0][0];
    }

    // Check diagonal top right to bottom left
    else if (boardPtr[0][2] != 0 && boardPtr[0][2] == boardPtr[1][1] && boardPtr[1][1] == boardPtr[2][0])
    {
        return boardPtr[0][2];
    }

    return 0;
}

// Display winner
void displayWinner(int winner, int mode)
{
    // Update points only when game state changes
    if (gameOver && pointFlag != gameOver)
    {
        if (mode == MULTIPLAYER_MODE)
        {
            if (winner == 1)
            {
                player1Ptr->wins++;
                player2Ptr->losses++;
            }
            else if (winner == 2)
            {
                player2Ptr->wins++;
                player1Ptr->losses++;
            }
            else if (winner == 0 && numArray.freeSpaces == 0)
            {
                player1Ptr->draws++;
                player2Ptr->draws++;
            }
        }
        else
        {
            if (winner == 1)
            {
                player1Ptr->wins++;
            }
            else if (winner == 2)
            {
                player1Ptr->losses++;
            }
            else if (winner == 0 && numArray.freeSpaces == 0)
            {
                player1Ptr->draws++;
            }
        }
        pointFlag = gameOver;
    }

    if (gameOver || numArray.freeSpaces == 0)
    {
        // Display winner text
        if (winner == 1)
        {
            title("Player 1 win", FONT_SIZE, SCREEN_WIDTH, 350);
        }
        else if (winner == 2)
        {
            title(mode == COMPUTER_MODE ? "Computer win" : "Player 2 win", FONT_SIZE, SCREEN_WIDTH, 350);
        }
        else if (winner == 0 && numArray.freeSpaces == 0)
        {
            title("It's a tie", FONT_SIZE, SCREEN_WIDTH, 350);
        }
    }
}

// Display 3x3 board
void showBoard(Texture2D cross, Texture2D circle)
{
    // Draw Grid
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            DrawRectangleLines(310 + (j * 60), 100 + (i * 60), 60, 60, GREEN);
            drawTextureMoves(boardPtr[i][j], 325 + j * 60, 115 + i * 60, cross, circle);
        }
    }
}

// Reset 3x3 grid
void resetBoard(char (*boardPtr)[3])
{
    if (numArray.state == 5)
    {
        shuffle(); // shuffle dataset
        split();   // split it into training and testing
        learn();   // retrieve win/loss probability from the training dataset
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            *(*(boardPtr + i) + j) = 0; // Pointer arithmetric, equivalent to board[i][j]
        }
    }
    numArray.freeSpaces = 9;
    numArray.winner = 0;
    gameOver = FALSE;
    numArray.turn = 1;
    pointFlag = FALSE;
}