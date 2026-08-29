#include "../include/sudoku_game.h"

/*Sudoku grid Generation Backtracking logic that uses recursion*/
int generateValidSudoku(int row, int col, struct Game *s1)
{
    /*If rows is equal to max rows then exit*/
    if (row == SUDOKU_DIMENSION)
    {
        return TRUE;
    }
    /*If column is equal to max column then increment row and set column to 0*/
    if (col == SUDOKU_DIMENSION)
    {
        return generateValidSudoku(row + 1, 0, s1);
    }
    /*Loop to initialize 1-9 value for shuffle*/
    int numbers[SUDOKU_DIMENSION] = {};
    for (int i = 0; i < SUDOKU_DIMENSION; i++)
    {
        numbers[i] = i + 1;
    }
    /*Shuffle Array using Fisher Yates algorithm*/
    for (int i = SUDOKU_DIMENSION - 1; i > 0; i--)
    {
        int random = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[random];
        numbers[random] = temp;
    }
    /*now check if our array is valid*/
    for (int i = 0; i < SUDOKU_DIMENSION; i++)
    {
        int num = numbers[i];
        /*Checks if valid on row/column/subgrid*/
        if (checkRows(num, row, col, s1->sudokuGrid) &&
            checkCols(num, row, col, s1->sudokuGrid) &&
            checkSubGrid(num, row, col, s1->sudokuGrid))
        {
            /*if its then add it to both of our grid*/
            s1->zeroGrid[row][col] = num;
            s1->sudokuGrid[row][col] = num;
            /*if its done then exit */
            if (generateValidSudoku(row, col + 1, s1))
            {
                return 1;
            }
            /*Else add a zero */
            s1->sudokuGrid[row][col] = 0;
        }
    }

    return 0;
}
/*Function to check if the number is valid in column*/
int checkRows(int number, int row, int col, int sudokuGrid[9][9])
{
    /*loop through the columns*/
    for (int i = 0; i < col; i++)
    {
        /*if the given number is present is that column then return false*/
        if (sudokuGrid[row][i] == number)
        {
            return FALSE;
        }
    }
    /*If not then return true*/
    return TRUE;
}

int checkCols(int number, int row, int col, int sudokuGrid[9][9])
{
    /*loop through the row*/
    for (int i = 0; i < row; i++)
    {
        /*if the given number is present is that row then return false*/
        if (sudokuGrid[i][col] == number)
        {
            return FALSE;
        }
    }
    /*If not then return true*/
    return TRUE;
}

int checkSubGrid(int number, int row, int col, int sudokuGrid[9][9])
{
    /*formula to track subgrid*/
    int first = (row / 3) * 3;
    int second = (col / 3) * 3;

    /*loop through the subgrid*/
    for (int row = first; row < first + 3; row++)
    {
        for (int col = second; col < second + 3; col++)
        {
            /*if the given number is present is the subgrid then return false*/
            if (sudokuGrid[row][col] == number)
            {
                return FALSE;
            }
        }
    }
    /*If not then return true*/

    return TRUE;
}
/*Function to Fill zero according to selected level*/
void fillWithZero(int level, struct Game *s1)
{
    for (int i = 0; i < SUDOKU_DIMENSION; i++)
    {
        for (int j = 0; j < level; j++)
        {
            /*Fill the zero on random index */
            s1->zeroGrid[i][rand() % SUDOKU_DIMENSION] = 0;
        }
    }
}