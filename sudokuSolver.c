#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <math.h>

/* #DEFINES =================================================================*/
#define WIDTH 9
#define INVALID 0
#define SOLVABLE 1
#define SOLVED 2

/* #FUNCTIONS INITIALISATION ================================================*/
int **initGrid(FILE *);

int **solve(int **grid);

int validGrid(int **grid);

int **copyGrid(int **grid);

void printGrid(int **, FILE *);

void freeGrid(int **);

int main(int argc, char *agrv[])
{
    /*------------------------------------------*/
    FILE *fpin, *fpout;
    fpin = fopen("sudokuGrid.txt", "r");
    if (fpin == NULL)
        printf("Error opening file.\n");

    int **sudokuGrid = initGrid(fpin);
    /*------------------------------------------*/
    int **solved = solve(sudokuGrid);
    /*------------------------------------------*/
    fpout = fopen("output.txt", "w");
    if (fpout == NULL)
    {
        printf("Error opening output file.\n");
        return 1;
    }
    printGrid(solved, fpout);
    fprintf(fpout, "check result: %d\n", validGrid(sudokuGrid));
    fclose(fpout);
}

/* #EXTRA FUNCTIONS =========================================================*/

/**
 * Initializes a 2D array called `sudokuGrid` with values read from a file.
 *
 * @param fp - A pointer to a file containing the Sudoku grid values.
 * @return A 2D array containing the Sudoku grid values read from the file.
 */
int **initGrid(FILE *fp)
{
    int **sudokuGrid = calloc(WIDTH, sizeof(int *));
    for (int i = 0; i < WIDTH; i++)
        sudokuGrid[i] = calloc(WIDTH, sizeof(int));
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < WIDTH; j++)
            fscanf(fp, "%d", &sudokuGrid[i][j]);
    }
    return sudokuGrid;
}

/**
 * Creates a deep copy of a 2D grid.
 *
 * @param grid The original grid to be copied.
 * @return A deep copy of the original grid.
 */
int **copyGrid(int **grid)
{
    int **copied = malloc(WIDTH * sizeof(int *));
    for (int i = 0; i < WIDTH; i++)
        copied[i] = malloc(WIDTH * sizeof(int));
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < WIDTH; j++)
            copied[i][j] = grid[i][j];
    }
    return copied;
}

/**
 * Solves a Sudoku puzzle using a backtracking algorithm.
 *
 * @param grid A 2D array representing the Sudoku grid.
 * @return A 2D array representing the solved Sudoku grid, or NULL if no
 * solution is found.
 */
int **solve(int **grid)
{
    static int iter = 0;
    printf("iteration no.%d\n", iter++);

    int **nextIter = copyGrid(grid);
    int r = 0, c = 0;

    // Find the next available empty cell in the grid
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (!grid[i][j])
            {
                r = i;
                c = j;
                goto found;
            }
        }
    }
    // If no empty cell is found, the grid is solved
    return nextIter;

found:
    for (int i = 1; i <= WIDTH; i++)
    {
        nextIter[r][c] = i;
        int status = validGrid(nextIter);
        if (status == SOLVED)
            return nextIter;
        else if (status == SOLVABLE)
        {
            int **leaf = solve(nextIter);
            if (leaf)
            {
                freeGrid(nextIter);
                return leaf;
            }
        }
    }
    // If no solution is found, return NULL
    return NULL;
}

/**
 * Checks if a Sudoku grid is valid by ensuring that each row, column, and
 * block contains unique numbers from 1 to 9.
 *
 * @param grid - a 2D array representing the Sudoku grid.
 * @return An integer value indicating the status of the grid
 *         (INVALID, SOLVABLE, or SOLVED).
 */
int validGrid(int **grid)
{
    int status = SOLVED;

    /* Goes through each row/column/block of the grid */
    for (int i = 0; i < WIDTH; i++)
    {
        int checkedRow[WIDTH] = {0};
        int checkedCol[WIDTH] = {0};
        int checkedBlock[WIDTH] = {0};

        /* Check all numbers in row and column */
        for (int j = 0; j < WIDTH; j++)
        {
            if (!grid[i][j])
                status = SOLVABLE;

            /* Check row */
            if (grid[i][j])
            {
                if (!checkedRow[grid[i][j] - 1])
                    checkedRow[grid[i][j] - 1] = 1;
                else
                    return INVALID;
            }

            /* Check col */
            if (grid[j][i])
            {
                if (!checkedCol[grid[j][i] - 1])
                    checkedCol[grid[j][i] - 1] = 1;
                else
                    return INVALID;
            }

            /* Check block */
            if (grid[(int)(floor(i / 3) * 3 + floor(j / 3))]
                    [(i % 3) * 3 + (j % 3)])
            {
                if (!checkedBlock[grid[(int)(floor(i / 3) * 3 + floor(j / 3))]
                                      [(i % 3) * 3 + (j % 3)] -
                                  1])
                    checkedBlock[grid[(int)(floor(i / 3) * 3 + floor(j / 3))]
                                     [(i % 3) * 3 + (j % 3)] -
                                 1] = 1;
                else
                    return INVALID;
            }
        }
    }
    return status;
}

/**
 * Prints the Sudoku grid to the specified file in a formatted manner.
 *
 * @param grid A 2D array representing the Sudoku grid.
 * @param fp A file pointer to the output file.
 */
void printGrid(int **grid, FILE *fp)
{
    fprintf(fp, "|-------+-------+-------|\n");
    for (int i = 0; i < WIDTH; i++)
    {
        fprintf(fp, "| %d %d %d | %d %d %d | %d %d %d |\n",
                grid[i][0], grid[i][1], grid[i][2],
                grid[i][3], grid[i][4], grid[i][5],
                grid[i][6], grid[i][7], grid[i][8]);
        if ((i + 1) % 3 == 0)
            fprintf(fp, "|-------+-------+-------|\n");
    }
}

/**
 * Frees the memory allocated for a 2D grid.
 *
 * @param grid The 2D array representing the Sudoku grid.
 */
void freeGrid(int **grid)
{
    for (int i = 0; i < WIDTH; i++)
        free(grid[i]);
    free(grid);
}
