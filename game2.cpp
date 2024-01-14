#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

FILE *inp;  // data file reading and writing operators
FILE *outp;

char c[30][21]; // variable for storing screen particles (pixels)
int n[30][21];  // variable for checking
int highscore;
int contr, tuk = 0, score = 0, t = 0, bt = 0, birdx = 0, birdy = 0; // various variables for certain operations
int err; // boolean for error detection

void game();  // various functions
void screen();
void pipes();
void bird();
int gameover();
void checkscore();
void help();
void menu();
void endgame();
void credits();

int main()
{
    srand(time(0));  // seeding random number gen, we will need it later;
    inp = fopen("/Program Files/FlappyBird/options.txt", "r");  // opening file in which highscore is stored
    if (inp != NULL) // if file opens successfully, it reads the highscore
    {
        fscanf(inp, "%d", &highscore);
        fclose(inp);
        err = 0;  // error will be false because the file opened successfully
    }
    else
    {
        highscore = 0; // if the file doesn't exist, highscore will be 0, and err will be true
        err = 1;
    }

    int a = 0;
    char sl; // selection variable
    while (1) // loop for repeating actions after each start
    {
        if (a == 0)
            goto play;
        if (a > 0)               // if you play not the first time, it will ask you if you want to play
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
        menu(); // calling menu function
        scanf(" %c", &sl);
        switch (sl) // menu selections
        {
        case '1':
        {
            game(); // if you choose play, it calls function game
            break;
        }
        case '2': // other selections - other functions
        {
            help();
            goto play;
            break;
        }
        case '3':
        {
            credits();
            goto play;
            break;
        }
        case '4':
        {
            goto quit; // exits game
            break;
        }
        default:
        {
            goto play;
            break;
        }
        }
        a++; // variable for checking how many times you've played
    }
quit:
    printf("I quit."); // stops game, app closes.

    return 0;
}

void game()  // function for playing the game
{
    int x, y;
    char s;
    for (y = 0; y < 21; y++)  // setting screen
    {
        for (x = 0; x < 30; x++)
        {
            if (y < 20)
            {
                c[x][y] = ' ';
                n[x][y] = 0;
            }
            if (y == 20)
            {
                c[x][y] = '-';
                n[x][y] = 2;
            }
        }
    }
    c[10][10] = '*';  // in these coordinates will be our bird, marked *
    screen();      // calls func for showing the screen
    while (1)       // loop starts, actual gameplay begins
    {
        s = '~';  // default control variable
        Sleep(0.2 * 1000);  // this sets how fast everything moves
        t++;              // this is a variable for storing 'time', or how many times a loop passed
        if (kbhit()) // if a key is pressed, certain operations are done for the bird to move up
        {
            s = getch();        // gets what key is pressed
            if (s != '~')
                tuk = 1; // if it is not default, then 'tuk' will be equal to 1, meaning that the bird will fly up
        }
        for (x = 0; x < 30; x++) // just setting ground
        {
            c[x][20] = '-';
            n[x][20] = 2;
        }
        bird();                       // calls bird move function
        checkscore();                 // checks score
        if (gameover() == 1)
            goto gameEnd;   // checks if the bird hits pipes, if yes, the game ends
        pipes();                             // spawns and moves pipes
        if (score > highscore)
            highscore = score;  // i think this is clear
        screen();                            // finally, calls the screen function to show everything.

        if (tuk > 0)
            tuk++;           // if a key is pressed, the bird will fly up 2 times.
        if (tuk == 3)
            tuk = 0;          // after that, the bird falls
    }
gameEnd:   // ends the game
    if (score > highscore)
        highscore = score;
    if (err == 0)              // if hi-score file exists, it writes your new highscore there.
    {
        outp = fopen("/Program Files/FlappyBird/options.txt", "w");
        fprintf(outp, "%d", highscore);
        fclose(outp);
    }
    screen();    // shows the ending screen, and returns to int main
    endgame();
    return;
}

void screen()    // func for showing the screen
{
    int x, y;
    system("cls");    // clears the console
    for (y = 0; y < 21; y++) // shows pixels on their coordinates, and your score
    {
        for (x = 0; x < 30; x++)
        {
            if (x < 29)
                printf("%c", c[x][y]);
            if (x == 29)
                printf("%c\n", c[x][y]);
        }
    }
    printf("\n");
    printf("Your Score: %d", score);
}

void pipes()  // pipe movement and spawn func
{
    int x, y, r;
    if (t == 10)   // if time is 10, or the loop has passed 10 times, it spawns a new pipe
    {
        r = (rand() % 11) + 5;  // generates a random number, which will be the pipe's hole center
        for (y = 0; y < 20; y++)  // only the y coordinate is needed
        {
            c[29][y] = '|';  // sets pipe
            n[29][y] = 2;    // n will be 2, for checking if the bird has hit it,
        }
        c[29][r - 1] = ' ';  // sets hole
        c[29][r] = ' ';
        c[29][r + 1] = ' ';
        n[29][r - 1] = 0;
        n[29][r] = 0;
        n[29][r + 1] = 0;
        t = 0;
        goto mv; // moves pipes
    }
    else
        goto mv;
mv:                 // pipe movement
    for (y = 0; y < 20; y++)  // loops for generating coordinates
    {
        for (x = 0; x < 30; x++)
        {
            if (c[x][y] == '|')  // all the pipes will be moved left by 1
            {
                if (x > 0)
                {
                    c[x - 1][y] = '|';
                    n[x - 1][y] = 2;
                    c[x][y] = ' ';
                    n[x][y] = 0;
                }
                if (x == 0)  // if the screen ends (x=0), the pipe will disappear, to prevent errors
                {
                    c[x][y] = ' ';
                    n[x][y] = 0;
                }
            }
        }
    }
}

void bird()   // bird movement function
{
    int x, y;
    if (tuk > 0) // if a key is pressed, the bird moves up
    {
        bt = 0;
        for (y = 0; y < 20; y++)   // loops for finding bird coordinates
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '*')
                {
                    if (y > 0)
                    {
                        c[x][y - 1] = '*';  // bird moves up by 1
                        c[x][y] = ' ';
                        birdx = x;        // sets bird x coordinate
                        birdy = y - 1;      // sets bird y coord
                        return;         // returns to the game func
                    }
                }
            }
        }
    }
    else   // if no key is pressed, the bird falls
    {
        bt++;
        for (y = 0; y < 20; y++)
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '*')
                {
                    if (y < 20)  // if the bird is not on the ground
                    {
                        if (bt < 3)   // if bird time is lower than 3, it falls 1 pixel
                        {
                            c[x][y + 1] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 1;
                            return;
                        }
                        else if (bt > 2 && bt < 5)  // more time has passed, faster the bird falls (acceleration)
                        {
                            c[x][y + 2] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 2;
                            return;
                        }
                        else if (bt > 4)
                        {
                            c[x][y + 3] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 3;
                            return;
                        }
                    }
                    else
                    {
                        return;  // if the bird is already on the ground, the function returns to check for game over.
                    }
                }
            }
        }
    }
}

void checkscore()  // checks if the bird gained score
{
    int y;
    for (y = 0; y < 20; y++)
    {
        if (c[birdx][y] == '|')  // if the bird x coord is equal to the pipe's coord, you get 1 point
        {
            score++;
            return;
        }
    }
}

int gameover()  // checks if the bird has hit something
{
    int x, y, f = 0;
    if (birdy > 19) // checks if the bird hits the ground
    {
        c[birdx][19] = '*';  // sets bird and ground again, prevents errors
        c[birdx][20] = '-';
        f = 1;           // f=1, means the function will return true
        goto quit;
    }
    else
    {     // checks if the bird has hit pipes, here the 'n' variable is needed (pipe's coordinate's n is equal 2 (more than 0))
        if (n[birdx][birdy] > 0 && (c[birdx][birdy] == '|' || c[birdx][birdy] == '*'))
        {
            c[birdx][birdy] = '|';
            c[birdx - 1][19] = '*';
            f = 1;
            goto quit;
        }
    }
quit:
    if (f == 1)
        return 1;
    else
        return 0;
}

void endgame() // just some screens for certain actions
{
    screen();   // this one pops up if the game ends
    printf("\n\n");
    printf(" ------------------------------------------------------------------------- \n");
    printf("|    *****      *     *       * ******       ****  *       ****** ****    |\n");
    printf("|   *          * *    * *   * * *           *    *  *     * *     *   *   |\n");
    printf("|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |\n");
    printf("|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |\n");
    printf("|    *****  *       * *       * ******       ****      *    ***** *   *   |\n");
    printf(" ------------------------------------------------------------------------- \n");
    printf("\n\n");
    printf("                        Y O U R   S C O R E : %d\n\n", score);
    printf("                        H I G H   S C O R E : %d\n", highscore);
    printf("\n\n");
}

void menu() {
    system("cls");
    printf("\n");
    printf(" --------------------------------------------------------  \n");
    printf("|                                                        | \n");
    printf("|   **** *    **** **** **** *   *    ***  * ***  ***    | \n");
    printf("|   *    *    *  * *  * *  * *   *    *  * * *  * *  *   | \n");
    printf("|   ***  *    **** **** **** *****    ***  * ***  *  *   | \n");
    printf("|   *    *    *  * *    *      *      *  * * *  * *  *   | \n");
    printf("|   *    **** *  * *    *      *      ***  * *  * ***    | \n");
    printf("|                                                        | \n");
    printf(" --------------------------------------------------------  \n");
    printf("\n\n");
    printf("                  High Score:  %d\n\n", highscore);
    printf("\n\n");
    printf("                     M E N U:    \n\n");
    printf("                  1: Start Game  \n\n");
    printf("                  2: Help        \n\n");
    printf("                  3: Credits     \n\n");
    printf("                  4: Exit        \n\n");
}

void credits() {
    char sel;
    system("cls");
    while (1) {
        printf("\n\n");
        printf("               Lead programmer: Shuja \n\n");
        printf("               Designer: Maaz \n\n");
        printf("Go back? [y/n]  ");
        scanf(" %c", &sel);
        if (sel == 'y') return;
        else system("cls");
    }
}

void help() {
    char sel;
    system("cls");
    while (1) {
        printf("\n\n");
        printf("                   Controls: Press any key to fly up.         \n\n");
        printf("             Goal: Fly through the holes between the pipes.   \n");
        printf("             When you pass through the hole, you get 1 point.  \n");
        printf("                    Try to pass as much as you can.           \n");
        printf("            But be careful, don't hit the pipes or the ground!\n\n");
        printf("                          Are you ready? Go!                  \n\n\n\n");
        printf("Go back? [y/n]  ");
        scanf(" %c", &sel);
        if (sel == 'y') return;
        else system("cls");
    }
}
