# Pf_Project
# Flappy Bird in C

This is a recreation of the popular mobile game Flappy Bird in C programming language. The game is a side-scroller where the player controls a bird, attempting to fly between pipes without hitting them or the ground. The game is challenging and requires quick reflexes and good timing.

## Code Overview

The code is organized into several functions, each responsible for a specific aspect of the game. The main function initializes the game, loads the high score, and displays the main menu. The game function handles the actual gameplay, including the bird's movement, pipe generation, and collision detection. The screen function displays the game screen, including the bird, pipes, and score. The pipes function generates and moves the pipes. The bird function controls the bird's movement and animation. The checkscore function checks if the bird has passed through a pipe and increments the score accordingly. The gameover function checks for collisions and ends the game if necessary. The endgame function displays the game over screen and the player's score. The menu function displays the main menu and handles user input. The credits function displays the game credits. The help function displays the game help screen.

## Code Explanation

### Main Function

```c
int main()
{
    // Initialize the game
    srand((unsigned int)time(NULL));

    // Load the high score
    inp = fopen("/Program Files/FlappyBird/options.txt", "r");
    if (inp != NULL)
    {
        fscanf(inp, "%d", &highscore);
        fclose(inp);
        err = 0;
    }
    else
    {
        highscore = 0;
        err = 1;
    }

    // Initialize the game loop
    int a = 0;
    char sl;
    while (1)
    {
        // Display the main menu
        if (a == 0)
            goto play;
        if (a > 0)
        {
            score = 0;
            printf("Do you want to play again? [y/n] ");
            scanf(" %c", &sl);
            if (sl == 'n')
                goto quit;
            else
                goto play;
        }

    play:
        menu();
        scanf(" %c", &sl);
        switch (sl)
        
