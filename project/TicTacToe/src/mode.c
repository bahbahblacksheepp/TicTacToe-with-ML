#include "prototypes.h"

// Update and draw game frame for home screen
void HomeScreen(void)
{
    Rectangle twoPlayerButton, aiButtonEasy, aiButtonImpossible, mlButton;

    // Draw home screen
    ClearBackground(BLACK);

    title("Tic Tac Toe", FONT_SIZE, SCREEN_WIDTH, 20);

    // Draw buttons and centralize it
    twoPlayerButton = (Rectangle){335, 130, 140, 40};
    button("2 Player", twoPlayerButton, TEXT_FONT_SIZE);
    aiButtonEasy = (Rectangle){335, 200, 140, 40};
    button("AI Medium", aiButtonEasy, TEXT_FONT_SIZE);
    aiButtonImpossible = (Rectangle){335, 270, 140, 40};
    button("AI Impossible", aiButtonImpossible, TEXT_FONT_SIZE);
    mlButton = (Rectangle){335, 340, 140, 40};
    button("ML (Imperfect)", mlButton, TEXT_FONT_SIZE);

    // Handles button clicks on the home screen, transitioning to different game modes based on the button pressed
    if (buttonDelay > 30)
    {
        
        if (CheckCollisionPointRec(GetMousePosition(), twoPlayerButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            buttonDelay = 0;
            numArray.state = 2;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), aiButtonEasy) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            buttonDelay = 0;
            numArray.state = 3;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), aiButtonImpossible) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            buttonDelay = 0;
            numArray.state = 4;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), mlButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            buttonDelay = 0;
            numArray.state = 5;          
        }        
    }
    buttonDelay++;
}


// Update and draw game frame for two player mode
void TwoPlayerMode()
{
    ClearBackground(BLACK);

    // Draw title and centralize it
    title("PVP", FONT_SIZE, SCREEN_WIDTH, 20);
    DrawText("Sound Effect by floraphonic from Pixabay", 5, 430, 14, GREEN);

    showBoard(cross, circle);
    showHomeButton();

    // Display scores
    DrawText("Player 1", 30, 110, 16, GREEN);
    DrawText(TextFormat("Wins: %u", player1Ptr->wins), 30, 150, 14, GREEN);
    DrawText(TextFormat("Losses: %u", player1Ptr->losses), 30, 170, 14, RED);
    DrawText(TextFormat("Draws: %u", player1Ptr->draws), 30, 190, 14, YELLOW);
    DrawText("Player 2", 650, 110, 16, GREEN);
    DrawText(TextFormat("Wins: %u", player2Ptr->wins), 650, 150, 14, GREEN);
    DrawText(TextFormat("Losses: %u", player2Ptr->losses), 650, 170, 14, RED);
    DrawText(TextFormat("Draws: %u", player2Ptr->draws), 650, 190, 14, YELLOW);

    if (numArray.turn == 1)
    {
        title("Player 1 turn", TEXT_FONT_SIZE, SCREEN_WIDTH, 53);
    }
    else if (numArray.turn == 2)
    {
        title("Player 2 turn", TEXT_FONT_SIZE, SCREEN_WIDTH, 53);
    }

    if (!gameOver)
    {
        playerMove();
        numArray.winner = checkWinner(boardPtr);

        if(numArray.winner == 0 && numArray.freeSpaces == 0)
        {
            gameOver = TRUE;
        }
        else if (numArray.winner == 0)
        {
            gameOver = FALSE;
        }
        else
        {
            gameOver = TRUE;
        }
    }
    displayWinner(numArray.winner, MULTIPLAYER_MODE);

    if (gameOver || numArray.freeSpaces == 0)
    {
        gameOverHandler();
    }
}


// Update and draw game frame for computer mode
void ComputerMode()
{
    ClearBackground(BLACK);

    title("Player vs Computer", FONT_SIZE, SCREEN_WIDTH, 20);
    DrawText("Sound Effect by floraphonic from Pixabay", 5, 430, 14, GREEN);

    showBoard(cross, circle);
    showHomeButton();

    // Display scores
    DrawText(TextFormat("Wins: %u", player1Ptr->wins), 650, 150, 14, GREEN);
    DrawText(TextFormat("Losses: %u", player1Ptr->losses), 650, 170, 14, RED);
    DrawText(TextFormat("Draws: %u", player1Ptr->draws), 650, 190, 14, YELLOW);

    if (!gameOver)
    {
        if (numArray.turn == 1)
        {
            playerMove();
        }
        else
        {
            switch (numArray.state)
            {
            case 3:
                computerMove(3);
                break;
            case 4:
                computerMove(9);
                break;
            case 5:
                nextMove(); // get next move using ML
                break;
            default:
                break;
            }
        }
        numArray.winner = checkWinner(boardPtr);

        if(numArray.winner == 0 && numArray.freeSpaces == 0)
        {
            gameOver = TRUE;
        }
        else if (numArray.winner == 0)
        {
            gameOver = FALSE;
        }
        else
        {
            gameOver = TRUE;
        }
    }

    displayWinner(numArray.winner, COMPUTER_MODE);

    if (gameOver || numArray.freeSpaces == 0)
    {
        gameOverHandler();
    }
}