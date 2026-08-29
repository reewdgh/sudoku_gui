/*----------------------------------------------------------
Sudoku GUI in c lang with win32
HWND : handle to a window/ HWND identifies the actual instance of some Window Class
WPARAM  : use it to pass things like handles and integers
LPARAM  : use it  pass pointers.
Msg :  message information from a thread's message queue
RECT : RECT struct that store the left,right,bottom and top value to pass it to rectangle drawing function
PAINTSTRUCT : PAINTSTRUCT structure contains information for window
*/

#include "../include/sudoku_gui.h"
#include "../include/sudoku_game.h"
/*
Function:LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
PURPOSE: Processes messages for the main window
Messages

*   FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
*
*   WM_COMMAND  - process the window menu
*   WM_PAINT    - post when window requests to Paint the main window
*   WM_CLOSE    - post when  window requests quit message
*   WM_CREATE   - post when window requests that a window be created
*
*

*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    /*Assgin a struct pointer to save user data*/
    struct PointerStruct *p1 = (struct PointerStruct *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    switch (msg)
    {
    /*Close the window and end the process*/
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        /*The BeginPaint function prepares the specified window for painting and fills a PAINTSTRUCT structure with information about the painting*/
        OutputDebugString("hey");
        HDC hdc = BeginPaint(hwnd, &ps);
        /* if its the game has stared then display the title*/
        if (p1->s1->isStart)
        {
            /*Create a font with custom configurations*/
            HFONT font = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, TITLE_FONT);
            /*Select the font*/
            SelectObject(hdc, font);
            /*Change the Bk color to our main color so it dosent show white backround behind */
            SetBkColor(hdc, mainBackgroundColorHex);
            /*display the title*/
            TextOut(hdc, p1->s1->positions[2] + 20, p1->s1->positions[3] - 80, "Sudoku", 6);
            /*Delete the font */
            DeleteObject(font);
            /*EndPaint function marks the end of painting of the window*/
            EndPaint(hwnd, &ps);
            /*exit*/
            return 0;
        }
        /*Fuction to display user game data*/
        DisplayUserGameData(hdc, p1);
        /*Fuction Draw the Sudoku grid*/
        drawSudokuGrid(hdc, p1);
        EndPaint(hwnd, &ps);
        return 0;
    }
        /*assign buttons handles/hwnd  */
        static HWND difficultyButtons[3];
    case WM_CREATE:
    {
        /*Handle to the icon*/
        HICON hicon;
        /*assign the hicon to the icon we want the window to have */
        hicon = (HICON)LoadImage(NULL, TEXT(iconFilePath), IMAGE_ICON, 500, 500, LR_LOADFROMFILE);
        /*send the message to set the icon*/
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
        /*retrive the struct we passed to createWindowEx*/
        CREATESTRUCT *cs = (CREATESTRUCT *)lp;
        struct PointerStruct *p1 = (struct PointerStruct *)cs->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)p1);

        if (p1->s1->isStart)
        {
            /*Function to create the difficulty buttons*/
            createDifficultyButtons(hwnd, difficultyButtons, p1);
        }

        return 0;
    }

    case WM_COMMAND:
    {
        /*Get the button id from the wparam*/
        int buttonID = LOWORD(wp);

        if (p1->s1->isStart)
        {
            /*Handle the difficulty buttons*/
            handleDifficultyButtons(buttonID, difficultyButtons, p1->s1);
            /*Create the Sudoku texboxes*/
            createSudokuTextbox(hwnd, p1->s1);
            /*set the empty cells to totalEmptyCells so we can decrement it later */
            p1->s1->emptyCells = p1->s1->totalEmptyCells;
            /*now after user selects the difficulty set isStart to 0 because we dont need it anymore*/
            p1->s1->isStart = 0;
            /*Forces window for an instant re drawing*/
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
            break;
        }

        /*Get the texbox ID*/
        HWND textbox = (HWND)lp;
        // /*Loop through totalEmptyCells */
        for (int i = 0; i < p1->s1->totalEmptyCells; i++)
        {
            if (LOWORD(wp) == i && HIWORD(wp) == EN_CHANGE)
            {
                /* Check if user typed a valid input*/
                if (validateUserInput(hwnd, textbox, i, p1->s1))
                {
                    break;
                }
            }
        }
        break;
    }

    default:
        /*This function ensures that every message is processed*/
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int main()
{
    srand(time(NULL));
    struct Game s1 = {};
    s1.totalEmptyCells = 0;
    /*Struct needed for user Game Data*/
    struct Gui g1 = {SUDOKU_GRID_COORDINATES, START_GAME_RECT};
    /*Pointer struct needed to store Gui & Game so we can pass to callback/WndProc*/
    struct PointerStruct p1 = {&s1, &g1};

    generateValidSudoku(0, 0, p1.s1); /* Genreate A Sudoku Grid */
    /*Formula to find the center of the window*/
    s1.positions[2] = (g1.Rect.left + g1.Rect.right) / 2 - 4;
    s1.positions[3] = (g1.Rect.left + g1.Rect.right) / 2 - 12;

    s1.positions[0] = (g1.gridCoordinates.left + g1.gridCoordinates.right) / 2 - 4;
    s1.positions[1] = (g1.gridCoordinates.top + g1.gridCoordinates.bottom) / 2 - 12;

    /*Indicates that the game is started*/
    s1.isStart = 1;

    /*Get an hinstance module */
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS class = {};

    /*Window Class configrations*/
    char className[] = "Sudoku";
    class.lpszClassName = className; /*Class Name */
    class.lpfnWndProc = WndProc;     /*Custom Callback Function*/
    class.hInstance = hInstance;
    int rgbColors[3] = mainBackgroundColorRGB; /*Set RGB Value Colors for the window*/
    class.hbrBackground = CreateSolidBrush(RGB(rgbColors[0], rgbColors[1], rgbColors[2]));
    class.hCursor = LoadCursor(NULL, IDC_ARROW); /*Used this so it dosent show's a spinning loader when its opens a window */

    /*If window couldn't be registered then show and message and exit*/
    if (!RegisterClass(&class)) /*Register a Class*/
    {
        MessageBox(NULL, "Window registration failed ", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    /*Create the Window */
    HWND hwnd = CreateWindowEx(0, className, "Sudoku App",
                               WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 500, 500, 500, 500, NULL,
                               NULL, hInstance, &p1); /*Pass our struct here*/
    /*Check if window couldn't be created then show a message and exit*/
    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window creation failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    /*Show the window*/
    ShowWindow(hwnd, 1);

    MSG msg;
    /*Message loop to handle window messages*/
    /*retrieves messages from the message queue and passes it to the callback function/window procedure */
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg); /*Translates Keyboard messages in to characters.Nesscary For user input*/
        DispatchMessage(&msg);  /*send the message to the window procedure*/
    }

    return 0;
}