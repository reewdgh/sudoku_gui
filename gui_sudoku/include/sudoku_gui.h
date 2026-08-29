#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "../src/config.h"

#ifndef SUDOKU_GUI_H
#define SUDOKU_GUI_H

/*Struct for user game data*/
struct Game
{
    int totalEmptyCells;
    int emptyCells;
    int mistakes;
    int score;
    int sudokuGrid[9][9];
    int zeroGrid[9][9];
    int isStart;
    int positions[4];
};

struct Gui
{
    RECT gridCoordinates;
    RECT Rect;
};
/*Struct pointer to store to store both Gui and Game Struct*/
struct PointerStruct
{
    struct Game *s1;
    struct Gui *g1;
};

void drawSudokuGrid(HDC hdc, struct PointerStruct *p1);
int DisplayUserGameData(HDC hdc, struct PointerStruct *p1);
int validateUserInput(HWND hwnd, HWND textbox, int currentTextboxID, struct Game *s1);
void handleDifficultyButtons(int buttonID, HWND *difficultyButtons, struct Game *s1);
void createDifficultyButtons(HWND hwnd, HWND *difficultyButtons, struct PointerStruct *p1);
void createSudokuTextbox(HWND hwnd, struct Game *s1);

#endif

