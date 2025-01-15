/* Include Files */
#include "src/prototypes.h" // Function Prototypes
#include "src/ui.c"         // UI Functions
#include "src/game.c"       // Game Logic Functions
#include "src/minimax.c"    // Minimax Function Files
#include "src/naive_bayes.c" // Naive Bayes Function Files
#include "src/mode.c" // Mode switch Function Files
/* End of Include Files */


int main()
{
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");
    SetTargetFPS(60);

    // Initialize audio device
    InitAudioDevice();

    // Load textures
    cross = LoadTexture("./resources/cross.png");
    circle = LoadTexture("./resources/circle.png");

    // Load sounds
    click = LoadSound("resources/8bit.wav");
    SetMasterVolume(0.3);

    //NB
    extractData(); // extract dataset for algorithm
    shuffle(); // shuffle dataset
    split(); // split it into training and testing
    learn(); // retrieve win/loss probability from the training dataset
                
    // Main game loop   
    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        // Checks the current game state and calls the appropriate mode function
        switch(numArray.state)
        {
            case 1:
                HomeScreen();
                break;
            case 2:
                TwoPlayerMode();
                break;
            case 3:
            case 4:
            case 5:
                ComputerMode();
                break;
            default:
                break;
        }

        EndDrawing();
    }

    // De-initialize window
    UnloadTexture(cross);
    UnloadTexture(circle);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}