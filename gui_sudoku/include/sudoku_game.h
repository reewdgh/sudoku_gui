#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "sudoku_gui.h" /*Include sudoku_gui.h so we can use struct Game and <windows.h>*/

int generateValidSudoku(int row, int col, struct Game *s1);
int checkRows(int number, int row, int col, int sudokuGrid[9][9]);
int checkCols(int number, int row, int col, int sudokuGrid[9][9]);
int checkSubGrid(int number, int row, int col, int sudokuGrid[9][9]);
void fillWithZero(int level, struct Game *s1);

#endif