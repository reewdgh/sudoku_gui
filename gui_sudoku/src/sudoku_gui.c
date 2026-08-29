#include "../include/sudoku_gui.h"
#include "../include/sudoku_game.h"

/*Global constant for the grid generation etc*/

/*Function to draw the Sudoku Grid*/

void drawSudokuGrid(HDC hdc, struct PointerStruct *p1)
{
    int val = p1->g1->gridCoordinates.left + 1;

    /*Draw rectangles to give dark border*/
    Rectangle(hdc, val, val, val * 9.65, val * 4 - 9);
    Rectangle(hdc, val, val * 3.80, val * 9.65, val * 4 - 9 + val * 3 - 4);
    Rectangle(hdc, val, val * 6.70, val * 9.65, val * 4 - 9 + val * 5.80);
    RECT rect = p1->g1->gridCoordinates;
    /*Create a custom brush*/
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

    /*Initialize the offfset */
    int value1 = OFFSET_GRID_COORDINATES;
    int value2 = OFFSET_GRID_COORDINATES;
    /*Grid genenration logic starts from here */
    for (int i = 0; i < SUDOKU_DIMENSION; i++)
    {
        for (int j = 1; j <= SUDOKU_DIMENSION; j++)
        {
            /*Draw custom square using rect*/
            FrameRect(hdc, &rect, hBrush);
            /*If it the end of the subgrid make it give a border*/
            if (j % 3 == 0)
            {
                rect.left = rect.left + value1 + 1;
                rect.right = rect.right + value1 + 1;
                continue;
            }
            rect.left = rect.left + value1;
            rect.right = rect.right + value1;
        }

        rect.top = value1 * 2 + 1;
        rect.bottom = value2 + value1 * 2;
        rect.left = value1 + 1;
        rect.right = value1 * 2 + 2;

        value2 += value1;
    }
    /*Create a custom font*/
    HFONT font = CreateFont(24, 0, 0, 15, FW_HEAVY, 0, 0, 0, 0, 0, 0, 0, 0, CELL_FONT);
    /*Select the font*/
    SelectObject(hdc, font);
    /*initialize the offset to variable so we can change it later*/
    const int offset = OFFSET_GRID_COORDINATES;
    for (int i = 0; i < SUDOKU_DIMENSION; i++)
    {
        for (int j = 0; j < SUDOKU_DIMENSION; j++)
        {
            /*If its zero then make text color blue*/
            if (p1->s1->zeroGrid[i][j] == 0)
            {
                SetTextColor(hdc, COLOR_BLUE);
            }
            char buffer[10] = "";
            /*Function convert int to char*/
            sprintf(buffer, "%d", p1->s1->sudokuGrid[i][j]);
            /*Fill the grid*/
            TextOut(hdc, p1->s1->positions[0] + (offset * j), p1->s1->positions[1] + (offset * i), buffer, 1);
            SetTextColor(hdc, COLOR_BLACK);
        }
    }
}

/*Function to ensure that user have entered correct input*/
int validateUserInput(HWND hwnd, HWND textbox, int currentTextboxID, struct Game *s1)
{
    char buffer[100];
    /*Get the entered text and store it to a buffer*/
    GetWindowText(textbox, buffer, sizeof(buffer));
    /*If the text len is 0 dont validate it */
    if (GetWindowTextLength(textbox) == 0)
    {
        return TRUE;
    }
    /*If its not a number or 0 then tell the user*/
    if (!isdigit((unsigned char)buffer[0]) || buffer[0] == '0')
    {
        /*Clear texbox text*/
        SetWindowText(textbox, "");
        MessageBox(textbox, "Input should only be a number , Please enter valid input again", "Error", MB_OK | MB_ICONERROR);
    }
    /*If its a number*/
    else
    {
        int row = 0;
        int col = 0;
        int idCount = 0;
        /*Logic to get the row and col/where the user typed input*/
        /*Used while loop through the get the row and col*/
        while (row != SUDOKU_DIMENSION)
        {
            /*If its a zero then increment idCount */
            if (s1->zeroGrid[row][col] == 0)
            {
                idCount++;
            }
            /*Check if idCount is equal to texbox idea(texbox id start from zero thus we have to increment 1)*/
            if (idCount == currentTextboxID + 1)
            {
                break;
            }
            col++;
            if (col == SUDOKU_DIMENSION)
            {
                row++;
                col = 0;
            }
        }
        /*Convert int to char*/
        int cell = atoi(buffer);
        /*check if its and valid placement,variable to hold the boolean value*/
        int check = checkRows(cell, row, col, s1->sudokuGrid) && checkCols(cell, row, col, s1->sudokuGrid) && checkSubGrid(cell, row, col, s1->sudokuGrid);
        /*If the check is 1 or its dose not match to the solved grid then tell the user*/
        if (check || s1->sudokuGrid[row][col] != cell)
        {
            /*Clear the texbox text*/
            SetWindowText(textbox, "");
            /*Increment the mistakes count*/
            s1->mistakes++;
            /*Redraw the window so mistakes can be updated*/
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
            /*Tell the user it's wrong*/
            MessageBox(textbox, "Invalid placement, Try another number", "Error!", MB_OK | MB_ICONERROR);
            /*if mistakes are 5 then tell the user and end the game*/
            if (s1->mistakes == 5)
            {
                MessageBox(hwnd, "Max limit of Mistakes Reached(5)!", "msg", MB_OK | MB_ICONASTERISK);
                PostQuitMessage(0);
            }
        }
        /*If its correct*/
        else
        {
            /*Destory the texbox , since our text is behind the texbox it will display the number */
            DestroyWindow(textbox);
            /*Increment score*/
            s1->score += 70;
            /*Redraw the window so score can be updated */
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
            /*Decrement the empty cells count*/
            s1->emptyCells--;
            /*If the empty cells are none then its means the user has filled the whole grid */
            if (!s1->emptyCells)
            {
                /*Congratulate the user and exit */
                MessageBox(hwnd, "Congratulations, You solved the Sudoku puzzle!", "Message ", MB_OK | MB_ICONINFORMATION);
                PostQuitMessage(0);
            }
        }
    }
    return 0;
}
/*Function to create the texbox*/
void createSudokuTextbox(HWND hwnd, struct Game *s1)
{
    int id = 0;
    for (int i = 0; i < SUDOKU_DIMENSION; i++)
    {
        for (int j = 0; j < SUDOKU_DIMENSION; j++)
        {
            /*If the grid is zero then only create the texbox on the specified locations*/
            if (s1->zeroGrid[i][j] == 0)
            {
                CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE,
                             s1->positions[0] + 1 + (44 * j), s1->positions[1] + 1.5 + (44 * i),
                             15, 20, hwnd, (HMENU)(INT_PTR)id, 0, 0);
                id++;
            }
        }
    }
    /*assign id to totalEmptyCells*/
    s1->totalEmptyCells = id;
}
/*Function to display user game data*/
int DisplayUserGameData(HDC hdc, struct PointerStruct *p1)
{
    char scoreMsg[20];
    char mistakesMsg[20];
    /*Change the Bk color to our main color so it dosent show white backround behind */
    SetBkColor(hdc, mainBackgroundColorHex);
    /*Convert int score to character*/

    sprintf(scoreMsg, "Score: %d", p1->s1->score);
    /*Display the score*/
    TextOut(hdc, p1->g1->gridCoordinates.left, p1->g1->gridCoordinates.left - 25, scoreMsg, strlen(scoreMsg));
    /*Set color red to say mistakes*/
    SetTextColor(hdc, COLOR_RED);
    /*Convert int mistakes to character*/
    sprintf(mistakesMsg, "Mistakes: %d/5", p1->s1->mistakes);
    /*Display the mistakes*/
    TextOut(hdc, p1->g1->gridCoordinates.right * 4 - 5, p1->g1->gridCoordinates.left - 25, mistakesMsg, strlen(mistakesMsg));

    SetBkColor(hdc, COLOR_WHITE);
    SetTextColor(hdc, COLOR_BLACK);
    return 0;
}

/*Function to create difficulty buttons*/
void createDifficultyButtons(HWND hwnd, HWND *difficultyButtons, struct PointerStruct *p1)
{
    /*Create  font*/
    HFONT font = CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, TITLE_FONT);

    char *level[] = LEVEL_LABELS;
    /*create the buttons */
    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        difficultyButtons[i] = CreateWindow("BUTTON",
                                            level[i], WS_CHILD | WS_VISIBLE, p1->s1->positions[2] - 10,
                                            p1->s1->positions[3] + (i * 80), p1->g1->Rect.right - 100, p1->g1->Rect.right - 250, hwnd, (HMENU)(INT_PTR)(i + 1), NULL, NULL);
        /*Message to set the font*/
        SendMessage(difficultyButtons[i], WM_SETFONT, (WPARAM)font, TRUE);
    }
}
/*Function to handle difficulty buttons*/
void handleDifficultyButtons(int buttonID, HWND *difficultyButtons, struct Game *s1)
{
    /*If button id is in the range of LEVEL_COUNT count then fill with zero according to selected levels*/
    if (buttonID <= LEVEL_COUNT)
    {
        fillWithZero(buttonID * 1, s1);
    }
    /*Destory the buttons*/
    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        DestroyWindow(difficultyButtons[i]);
    }
}