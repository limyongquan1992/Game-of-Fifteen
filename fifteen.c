/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(int d);
void draw(int d);
bool move(int tile, int d);
bool won(int d);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init(d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(d);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile, d))
        {
            printf("\nIllegal move.\n");
            usleep(500);
        }

        // sleep thread for animation's sake
        usleep(500);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int d)
{
    int a = d*d-1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = a;
            a--;
        }
    }
    
    if (d%2 == 0)
    {
        int temp = board[d-1][d-2];
        board[d-1][d-2] = board[d-1][d-3];
        board[d-1][d-3] = temp;
    }
    return;
}

/**
 * Prints the board in its current state.
 */
void draw(int d)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (j==0)
            {
                if (board[i][j] == 0)
                {
                    printf("__");
                }
                else
                {
                    printf("%2i", board[i][j]);
                }
            }
            else if (j == d-1)
            {
                if (board[i][j] == 0)
                {
                    printf(" __\n");
                }
                else
                {
                    printf(" %2i\n", board[i][j]);
                }
            }
            else
            {
                if (board[i][j] == 0)
                {
                    printf(" __");
                }
                else
                {
                    printf(" %2i", board[i][j]);
                }
            }
        }
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile, int d)
{
    int a;
    int b;
    int i = 0;
    int j = 0;
    while (true)
    {
        if (board[i][j] == tile)
        {
            a = i;
            b = j;
            break;
        }
        
        if (i == d-1 && j == d-1)
        {
            return false;
        }
        
        if (j == d-1)
        {
            i++;
            j = 0;
        }
        else
        {
            j++;
        }
    }
    
    int c;
    int e;
    int k = 0;
    int l = 0;
    while (true)
    {
        if (board[k][l] == 0)
        {
            c = k;
            e = l;
            break;
        }
        
        if (l == d-1)
        {
            k++;
            l = 0;
        }
        else
        {
            l++;
        }
    }
    
    
    if (c == a && e == b - 1)
    {
        int temp = board[a][b];
        board[a][b] = board[c][e];
        board[c][e] = temp;
        return true;
    }
    else if (c == a && e == b + 1)
    {
        int temp = board[a][b];
        board[a][b] = board[c][e];
        board[c][e] = temp;
        return true;
    }
    else if (c == a - 1 && e == b)
    {
        int temp = board[a][b];
        board[a][b] = board[c][e];
        board[c][e] = temp;
        return true;
    }
    else if (c == a + 1 && e == b)
    {
        int temp = board[a][b];
        board[a][b] = board[c][e];
        board[c][e] = temp;
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int d)
{
    int a = 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (i == d-1 && j == d-1)
            {
                if (board[i][j] != 0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            
            if (board[i][j] != a)
            {
                return false;
            }
            else 
            {
                a++;
            }
        }
    }
    return false;
}
