#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;
#define H 20
#define W 15
#define CELL "  "  // [THÊM] ô trống (2 khoảng trắng)
#define BLOCK "██" // [THÊM] khối vuông đặc
#define WALL "██"  // [THÊM] viền liền mạch
char board[H][W] = {};
char blocks[][4][4] = {
    {{' ', 'I', ' ', ' '},
     {' ', 'I', ' ', ' '},
     {' ', 'I', ' ', ' '},
     {' ', 'I', ' ', ' '}},
    {{' ', 'I', ' ', ' '},
     {' ', 'I', ' ', ' '},
     {' ', 'I', ' ', ' '},
     {' ', 'I', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {'I', 'I', 'I', 'I'},
     {' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'O', 'O', ' '},
     {' ', 'O', 'O', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'T', ' ', ' '},
     {'T', 'T', 'T', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', 'S', 'S', ' '},
     {'S', 'S', ' ', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {'Z', 'Z', ' ', ' '},
     {' ', 'Z', 'Z', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {'J', ' ', ' ', ' '},
     {'J', 'J', 'J', ' '},
     {' ', ' ', ' ', ' '}},
    {{' ', ' ', ' ', ' '},
     {' ', ' ', 'L', ' '},
     {'L', 'L', 'L', ' '},
     {' ', ' ', ' ', ' '}}};

int x = 4, y = 0, b = 1;
void gotoxy(int x, int y)
{
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void boardDelBlock()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ' && y + i < H) // [SỬA]
                board[y + i][x + j] = ' ';
}
void block2Board()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ')
                board[y + i][x + j] = blocks[b][i][j];
}
void initBoard()
{
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if ((i == H - 1) || (j == 0) || (j == W - 1))
                board[i][j] = '#';
            else
                board[i][j] = ' ';
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = FALSE;
    cursor.dwSize = sizeof(cursor);
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(handle, &cursor);
}

void draw()
{
    gotoxy(0, 0);

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (board[i][j] == '#')
                cout << WALL; // [THÊM] vẽ viền

            else if (board[i][j] != ' ')
                cout << BLOCK; // [THÊM] vẽ block đặc

            else
                cout << CELL; // [THÊM] ô trống
        }
        cout << endl;
    }
}
bool canMove(int dx, int dy)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ')
            {
                int tx = x + j + dx;
                int ty = y + i + dy;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1)
                    return false;
                if (board[ty][tx] != ' ')
                    return false;
            }
    return true;
}
void removeLine()
{
    int linesCleared = 0;

    // Quét từ áp chót (H-2) lên trên
    for (int i = H - 2; i > 0; i--)
    {
        bool isFull = true;
        for (int j = 1; j < W - 1; j++)
        {
            if (board[i][j] == ' ')
            {
                isFull = false;
                break;
            }
        }

        // Nếu dòng đầy -> ăn điểm
        if (isFull)
        {
            linesCleared++;
            // Dịch toàn bộ các dòng phía trên xuống 1 bậc
            for (int ii = i; ii > 0; ii--)
                for (int j = 1; j < W - 1; j++)
                    board[ii][j] = board[ii - 1][j];

            i++;        // Sau khi dịch xuống, phải kiểm tra lại chính index dòng này
            draw();     // Vẽ lại để tạo hiệu ứng ăn điểm
            _sleep(50); // Khựng lại một chút cho người chơi nhận ra
        }
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    hideCursor();
    srand(time(0));
    b = rand() % 7;
    system("cls");
    initBoard();
    while (1)
    {
        boardDelBlock();
        if (kbhit())
        {
            char c = getch();
            if (c == 'a' && canMove(-1, 0))
                x--;
            if (c == 'd' && canMove(1, 0))
                x++;
            if (c == 'x' && canMove(0, 1))
                y++;
            if (c == 'q')
                break;
        }
        if (canMove(0, 1))
            y++;
        else
        {
            block2Board();
            removeLine(); // Gọi hàm xóa dòng ở đây, ngay khi khối bị chạm đáy đóng băng lại
            x = 5;
            y = 0;
            b = rand() % 7;
        }
        block2Board();
        draw();
        _sleep(200);
    }
    return 0;
}
