/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements The Game of Fifteen (generalized to d x d).
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
#include <time.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define BLANK 0

// directional constants
#define	UP 		1
#define	RIGHT 	2
#define	DOWN 	3
#define	LEFT 	4
#define	ILLEGAL	0

#define NUM_DIRECTIONS 4

// input sentinel for end of game
#define	SENTINEL -1

// error value for specified tile not found on board
#define	TILE_NOT_FOUND -1

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

// returns a direction constant
int	get_direction(int tile_id);

// moves the given tile
void make_move(int tile_id, int direction);

// returns the column containing a tile
int	get_col(int tile_id);

// returns the row containing a tile
int	get_row(int tile_id);

// get the tile at position row, col
int get_tile(int row, int col);

// set row, col to tile_id
void set_tile(int row, int col, int tile_id);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: %s d\n", argv[0]);
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %d x %d and %d x %d, inclusive.\n",
               DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

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
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}


/**
 * Clears screen using ANSI escape sequences.
 */
void clear()
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}


/**
 * Greets player.
 */
void greet()
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles (numbered 1 through d*d - 1),
 * i.e., fills 2D array with values but does not actually print them).  
 */
void init()
{
    int i, j;
    int counter = d * d;

    // initialize the board for the normal cases
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            set_tile(i, j, --counter);
        }
    }

    set_tile(d - 1, d - 1, BLANK);

    // swap the 1 and 2 tiles if the dimension is even
    if (d % 2 == 0)
    {
        set_tile(d - 1, d - 3, 1);
        set_tile(d - 1, d - 2, 2);
    }
}


/**
 * Prints the board in its current state.
 */
void draw()
{
    int i, j;
    printf("\n");
    for (i = 0; i < d; i++)
    {   
        printf(" ");
        for (j = 0; j < d; j++)
        {	
            if (get_tile(i, j) == BLANK)
            {
                printf("%3c", '_');
			}
            else
            {
                printf("%3d", get_tile(i, j));
			}
		}
        printf("\n\n");
	}
}

/** 
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile_id)
{
    int direction;

    direction = get_direction(tile_id);

    if (direction != ILLEGAL)
    {
        make_move (tile_id, direction);
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
bool won()
{
    int i, j;
    int counter = 1;
    int num_tiles = d * d;
    
    // the lower right hand corner must be the blank
    if (get_tile(d - 1, d - 1) != BLANK)
    {
        return false;
    }

    // loop through all spaces on the board assuring they are correct
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
        
            // If the counter is equal to the number of tiles,
            // then all tiles have been checked except for the
            // blank, which was checked earlier --> victory.
            // Otherwise, a tile that's not equal to
            // the counter is reach, and that tile must
            // be out of place --> no victory
            if (counter == num_tiles)
            {
                return true;
            }
            else if (get_tile(i, j) != counter)
            {
                return false;
            }
            counter++;
        }
    }

    // execution will never reach here
    return false;
}

/**
 * Determines the direction to move the indicated tile. Returns UP,
 * DOWN, LEFT, or RIGHT to indicate the direction or ILLEGAL if no
 * legal move exists for that tile.
 */
int get_direction(int tile_id)
{
    // coordinates of the piece to be moved
    int p_row, p_col;
    
    // coordinates of the blank
    int b_row, b_col;

    // make sure the tile is in bounds
    if ((tile_id <= 0) || (tile_id >= d * d))
    {
        return ILLEGAL;
    }

    // locate the tile and the blank
    p_row = get_row(tile_id);
    p_col = get_col(tile_id);
    b_row = get_row(BLANK);
    b_col = get_col(BLANK);

    // make sure that the tile was found on the board
    if ((p_row == TILE_NOT_FOUND) || (p_col == TILE_NOT_FOUND) || 
        (b_row == TILE_NOT_FOUND) || (b_col == TILE_NOT_FOUND))
    {
        return ILLEGAL;
	}

    // determine the move direction
    if ((p_row == b_row + 1) && (p_col == b_col))
    {
        return UP;
	}
    else if ((p_row == b_row - 1) && (p_col == b_col))
    {
        return DOWN;
	}
    else if ((p_row == b_row) && (p_col == b_col - 1))
    {
        return RIGHT;
	}
    else if ((p_row == b_row) && (p_col == b_col + 1))
    {
        return LEFT;
	}
    else
    {
        return ILLEGAL;
	}
}

/**
 * Implements a move in the given direction.  The  indicated tile is
 * moved in the given direction one space and the blank is moved to
 * where the tile was originally. This function does no error
 * checking; it assumes that the validity of the move has already been
 * determined by get_direction.
 */
void make_move(int tile_id, int direction)
{
    // coordinates of the tile to be moved
    int row, col; 

    // find the tile
    row = get_row(tile_id);
    col = get_col(tile_id);

    // set the blank to (row, col)
    set_tile(row, col, BLANK);

    // move the tile onto the blank
    switch (direction)
    {
        case LEFT:
            set_tile(row, col - 1, tile_id);
            break;
        case RIGHT:
            set_tile(row, col + 1, tile_id);
            break;
        case UP:
            set_tile(row - 1, col, tile_id);
            break;
        case DOWN:
            set_tile(row + 1, col, tile_id);
            break;
        default:
            break;
	}
}

/**
 * Returns the column index of the given tile_id. 
 */
int get_col(int tile_id)
{
    int i, j;
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (get_tile(i, j) == tile_id)
            {
                return j;
			}
		}
	}

    // tile wasn't found, return error value
    return TILE_NOT_FOUND;
}

/**
 * Returns the row index of the given tile_id. 
 */
int get_row(int tile_id)
{
    int i, j;
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (get_tile(i, j) == tile_id)
            {
                return i;
			}
		}
	}

    // tile wasn't found, return error value
    return TILE_NOT_FOUND;
}

/**
 * Returns the tile at position (row, col), but performs no error
 * checking.
 */
int get_tile(int row, int col)
{
    return board[row][col];
}

/**
 * Set row, col of the board to tile_id
 */
void set_tile(int row, int col, int tile_id)
{
    board[row][col] = tile_id;
}
