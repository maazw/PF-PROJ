#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

FILE *inp;
FILE *outp;

char c[30][21];
int n[30][21];
int highscore;
int contr, tuk = 0, score = 0, t = 0, bt = 0, birdx = 0, birdy = 0;
int err;

void game();
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
    srand((unsigned int)time(NULL));

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

    int a = 0;
    char sl;
    while (1)
    {
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
        {
        case '1':
            game();
            break;
        case '2':
            help();
            goto play;
            break;
        case '3':
            credits();
            goto play;
            break;
        case '4':
            goto quit;
            break;
        default:
            goto play;
            break;
        }
        a++;
    }
quit:
    printf("I quit.");
    return 0;
}

void game()
{
    int x, y;
    char s;
    for (y = 0; y < 21; y++)
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
    c[10][10] = '*';
    screen();
    while (1)
    {
        s = '~';
        Sleep(0.2 * 1000);
        t++;
        if (kbhit())
        {
            s = getch();
            if (s != '~')
                tuk = 1;
        }
        for (x = 0; x < 30; x++)
        {
            c[x][20] = '-';
            n[x][20] = 2;
        }
        bird();
        checkscore();
        if (gameover())
            goto gameEnd;
        pipes();
        if (score > highscore)
            highscore = score;
        screen();
        if (tuk > 0)
            tuk++;
        if (tuk == 3)
            tuk = 0;
    }
gameEnd:
    if (score > highscore)
        highscore = score;
    if (err == 0)
    {
        outp = fopen("/Program Files/FlappyBird/options.txt", "w");
        fprintf(outp, "%d", highscore);
        fclose(outp);
    }
    screen();
    endgame();
}

void screen()
{
    int x, y;
    system("cls");
    for (y = 0; y < 21; y++)
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

void pipes()
{
    int x, y, r;
    if (t == 10)
    {
        r = (rand() % 11) + 5;
        for (y = 0; y < 20; y++)
        {
            c[29][y] = '|';
            n[29][y] = 2;
        }
        c[29][r - 1] = ' ';
        c[29][r] = ' ';
        c[29][r + 1] = ' ';
        n[29][r - 1] = 0;
        n[29][r] = 0;
        n[29][r + 1] = 0;
        t = 0;
        goto mv;
    }
    else
        goto mv;
mv:
    for (y = 0; y < 20; y++)
    {
        for (x = 0; x < 30; x++)
        {
            if (c[x][y] == '|')
            {
                if (x > 0)
                {
                    c[x - 1][y] = '|';
                    n[x - 1][y] = 2;
                    c[x][y] = ' ';
                    n[x][y] = 0;
                }
                if (x == 0)
                {
                    c[x][y] = ' ';
                    n[x][y] = 0;
                }
            }
        }
    }
}

void bird()
{
    int x, y;
    if (tuk > 0)
    {
        bt = 0;
        for (y = 0; y < 20; y++)
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '*')
                {
                    if (y > 0)
                    {
                        c[x][y - 1] = '*';
                        c[x][y] = ' ';
                        birdx = x;
                        birdy = y - 1;
                        return;
                    }
                }
            }
        }
    }
    else
    {
        bt++;
        for (y = 0; y < 20; y++)
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '*')
                {
                    if (y < 20)
                    {
                        if (bt < 3)
                        {
                            c[x][y + 1] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 1;
                            return;
                        }
                        else if (bt > 2 && bt < 5)
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
                        return;
                    }
                }
            }
        }
    }
}

void checkscore()
{
    int y;
    for (y = 0; y < 20; y++)
    {
        if (c[birdx][y] == '|')
        {
            score++;
            return;
        }
    }
}

int gameover()
{
    int x, y, f = 0;
    if (birdy > 19)
    {
        c[birdx][19] = '*';
        c[birdx][20] = '-';
        f = 1;
        goto quit;
    }
    else
    {
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

void endgame()
{
    screen();
    printf("\n\n");
    printf("   _____   ___  ___  ___ _____   _____  _   _ ___________  \n");
    printf("  |  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | |  ___| ___ \\ \n");
    printf("  | |  \\// /_\\ \\| .  . || |__   | | | || | | | |__ | |_/ / \n");
    printf("  | | __ |  _  || |\\/| ||  __|  | | | || | | |  __||    /  \n");
    printf("  | |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ / |___| |\\ \\  \n");
    printf("   \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/\\____/\\_| \\_| \n");
    printf("                                                           \n");
    printf("\n\n");
    printf("                        Y O U R   S C O R E : %d\n\n", score);
    printf("                        H I G H   S C O R E : %d\n", highscore);
    printf("\n\n");
}

void menu()
{
    system("cls");
    printf("\n");
    printf("  ______ _                          ______ _         _ \n");
    printf("  |  ___| |                         | ___ (_)       | |\n");
    printf("  | |_  | | __ _ _ __  _ __  _   _  | |_/ /_ _ __ __| |\n");
    printf("  |  _| | |/ _` | '_ \\| '_ \\| | | | | ___ \\ | '__/ _` |\n");
    printf("  | |   | | (_| | |_) | |_) | |_| | | |_/ / | | | (_| |\n");
    printf("  \\_|   |_|\\__,_| .__/| .__/ \\__, | \\____/|_|_|  \\__,_|\n");
    printf("                | |   | |     __/ |                    \n");
    printf("                |_|   |_|    |___/                     \n");
    printf("                                                       \n");
    printf("\n\n");
    printf("                  High Score:  %d\n\n", highscore);
    printf("\n\n");
    printf("                     M E N U:    \n\n");
    printf("                  1: Start Game  \n\n");
    printf("                  2: Help        \n\n");
    printf("                  3: Credits     \n\n");
    printf("                  4: Exit        \n\n");
}

void credits()
{
    char sel;
    system("cls");
    while (1)
    {
        printf("\n\n");
        printf("               Lead programmer: Shuja \n\n");
        printf("               Designer: Maaz \n\n");
        printf("Go back? [y/n]  ");
        scanf(" %c", &sel);
        if (sel == 'y')
            return;
        else
            system("cls");
    }
}

void help()
{
    char sel;
    system("cls");
    while (1)
    {
        printf("\n\n");
        printf("                   Controls: Press any key to fly up.         \n\n");
        printf("             Goal: Fly through the holes between the pipes.   \n");
        printf("             When you pass through the hole, you get 1 point.  \n");
        printf("                    Try to pass as much as you can.           \n");
        printf("            But be careful, don't hit the pipes or the ground!\n\n");
        printf("                          Are you ready? Go!                  \n\n\n\n");
        printf("Go back? [y/n]  ");
        scanf(" %c", &sel);
        if (sel == 'y')
            return;
        else
            system("cls");
    }
}

