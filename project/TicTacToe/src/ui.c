#include "prototypes.h"

// Draw title
void title(char *title, int fontSize, int screen_width, int y)
{
    int titleWidth = MeasureText(title, fontSize);
    int titleX = (screen_width - titleWidth) / 2;
    DrawText(title, titleX, y, fontSize, GREEN);
}

// Draw button
void button(char *text, Rectangle rectangle, int fontSize)
{
    int textWidth = MeasureText(text, fontSize);
    int textX = rectangle.x + (rectangle.width - textWidth) / 2;
    int textY = rectangle.y + (rectangle.height - fontSize) / 2;
    DrawRectangleLinesEx(rectangle, 1, GREEN);
    DrawText(text, textX, textY, fontSize, GREEN);
}

// Draw textures of moves made by player. This happens everytime showboard() is called
void drawTextureMoves(int player, int x, int y, Texture2D cross, Texture2D circle)
{
    switch (player)
    {
        case 1:
            DrawTexture(cross, x, y, WHITE);
            break;
        case 2:
            DrawTexture(circle, x, y, WHITE);
            break;
    }
}

// Constantly checks for click events on grid every frame
void gridClickHandler(Rectangle grid, int row, int col, Sound click)
{
    if (CheckCollisionPointRec(GetMousePosition(), grid) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && boardPtr[row][col] == 0)
    {
        boardPtr[row][col] = numArray.turn;
        --numArray.freeSpaces;
        PlaySound(click);
        NextTurn();
    }
}

// Show home button
void showHomeButton()
{
    Rectangle backButton = (Rectangle){630, 400, 140, 40};
    button("Back to Home", backButton, TEXT_FONT_SIZE);
    
    if (buttonDelay > 30)
    {
        if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            numArray.state = 1;
            resetBoard(boardPtr);
            player1Ptr->wins = 0;
            player1Ptr->losses = 0;
            player1Ptr->draws = 0;
            player2Ptr->wins = 0;
            player2Ptr->losses = 0;
            player2Ptr->draws = 0;
            buttonDelay = 0;
        }
    }
    buttonDelay++;
}

// Handle game over state
void gameOverHandler()
{
    // Add a "Play Again" button
    Rectangle playAgainButton = (Rectangle){(SCREEN_WIDTH - 140) / 2, 380, 140, 40};
    button("Play Again", playAgainButton, TEXT_FONT_SIZE);
    if (CheckCollisionPointRec(GetMousePosition(), playAgainButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        resetBoard(boardPtr);
    }
}