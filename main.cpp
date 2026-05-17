#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>
#include <conio.h>
#include <queue>
#include <fstream>
#include <vector>   

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 

using namespace std;

#define H 22
#define W 12
#define CELL "  "
#define BLOCK "██"
#define WALL "██"
#define GHOST "░░"

char board[H][W];
int score = 0, level = 1, linesClearedTotal = 0;
vector<int> highScores;
class Piece
{
protected:
    char shape[4][4];

public:
    virtual ~Piece() {}
    char getCell(int i, int j) const { return shape[i][j]; }
    void clearShape()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }
    virtual void getRotatedShape(char dest[4][4]) const
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                dest[j][3 - i] = shape[i][j];
    }
    void applyRotation(char newShape[4][4])
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = newShape[i][j];
    }
    virtual void reset() = 0;
};

class PieceI : public Piece
{
public:
    PieceI() { reset(); }
    void reset() override
    {
        clearShape();
        shape[0][1] = shape[1][1] = shape[2][1] = shape[3][1] = 'I';
    }
};
class PieceO : public Piece
{
public:
    PieceO() { reset(); }
    void reset() override
    {
        clearShape();
        shape[1][1] = shape[1][2] = shape[2][1] = shape[2][2] = 'O';
    }
    void getRotatedShape(char dest[4][4]) const override
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                dest[i][j] = shape[i][j];
    }
};
class PieceT : public Piece
{
public:
    PieceT() { reset(); }
    void reset() override
    {
        clearShape();
        shape[1][0] = shape[1][1] = shape[1][2] = shape[0][1] = 'T';
    }
};
class PieceS : public Piece
{
public:
    PieceS() { reset(); }
    void reset() override
    {
        clearShape();
        shape[1][0] = shape[1][1] = shape[0][1] = shape[0][2] = 'S';
    }
};
class PieceZ : public Piece
{
public:
    PieceZ() { reset(); }
    void reset() override
    {
        clearShape();
        shape[0][0] = shape[0][1] = shape[1][1] = shape[1][2] = 'Z';
    }
};
class PieceJ : public Piece
{
public:
    PieceJ() { reset(); }
    void reset() override
    {
        clearShape();
        shape[0][1] = shape[1][1] = shape[2][1] = shape[2][0] = 'J';
    }
};
class PieceL : public Piece
{
public:
    PieceL() { reset(); }
    void reset() override
    {
        clearShape();
        shape[0][1] = shape[1][1] = shape[2][1] = shape[2][2] = 'L';
    }
};

int x = 5, y = 0, speed = 400;
Piece *currentPiece = NULL;

queue<Piece*> nextQueue;
Piece* holdPiece = NULL;
bool canHold = true;

void gotoxy(int x, int y)
{
    COORD c = {(short)(x * 2), (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void loadHighScores() {
    highScores.clear();
    ifstream file("highscore.txt");
    int s;
    while (file >> s) {
        highScores.push_back(s);
    }
    file.close();
    
    while (highScores.size() < 5) {
        highScores.push_back(0);
    }
}

void saveHighScore(int currentScore) {
    highScores.push_back(currentScore); 
    
    for (size_t i = 0; i < highScores.size(); i++) {
        for (size_t j = i + 1; j < highScores.size(); j++) {
            if (highScores[i] < highScores[j]) {
                int temp = highScores[i];
                highScores[i] = highScores[j];
                highScores[j] = temp;
            }
        }
    }
    
    if (highScores.size() > 5) {
        highScores.resize(5);
    }
    
    ofstream file("highscore.txt");
    for (int s : highScores) {
        file << s << "\n";
    }
    file.close();
}


void enableANSIColors(HANDLE hOut)
{
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

string getColorCode(char c)
{
    switch (c)
    {
    case 'I':
        return "\x1b[96m";
    case 'O':
        return "\x1b[93m";
    case 'T':
        return "\x1b[95m";
    case 'S':
        return "\x1b[92m";
    case 'Z':
        return "\x1b[91m";
    case 'J':
        return "\x1b[94m";
    case 'L':
        return "\x1b[38;5;208m";
    case 'G':
        return "\x1b[90m";
    case '#':
        return "\x1b[37m";
    default:
        return "\x1b[0m";
    }
}

string getPieceRowUI(Piece *p, int row)
{
    if (p == NULL)
        return "        ";
    string res = "";
    for (int j = 0; j < 4; j++)
    {
        char c = p->getCell(row, j);
        if (c != ' ')
            res += getColorCode(c) + BLOCK + "\x1b[0m";
        else
            res += "  ";
    }
    return res;
}

void initBoard()
{
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            board[i][j] = ((i == H - 1) || (j == 0) || (j == W - 1)) ? '#' : ' ';
}

void boardDelBlock()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ' && y + i < H)
                board[y + i][x + j] = ' ';
}

void block2Board()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->getCell(i, j) != ' ')
                board[y + i][x + j] = currentPiece->getCell(i, j);
}

bool canMove(int dx, int dy, int testX, int testY, char customShape[4][4] = NULL)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            char cell = (customShape) ? customShape[i][j] : currentPiece->getCell(i, j);
            if (cell != ' ')
            {
                int tx = testX + j + dx, ty = testY + i + dy;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1 || board[ty][tx] != ' ')
                    return false;
            }
        }
    return true;
}

void removeLine()
{
    int linesThisTurn = 0;
    for (int i = H - 2; i > 0; i--)
    {
        bool full = true;
        for (int j = 1; j < W - 1; j++)
            if (board[i][j] == ' ')
                full = false;
        if (full)
        {
            linesThisTurn++;
            for (int ii = i; ii > 0; ii--)
                for (int jj = 1; jj < W - 1; jj++)
                    board[ii][jj] = board[ii - 1][jj];
            i++;
        }
    }
    if (linesThisTurn > 0)
    {
        linesClearedTotal += linesThisTurn;
        if (linesThisTurn == 1)
            score += 100 * level;
        else if (linesThisTurn == 2)
            score += 300 * level;
        else if (linesThisTurn == 3)
            score += 500 * level;
        else if (linesThisTurn == 4)
            score += 800 * level;
        level = (linesClearedTotal / 10) + 1;
        speed = max(100, 1000 - (level - 1) * 30);
    }
}

void draw()
{
    gotoxy(0, 0);
    string s = "";
    char renderBoard[H][W];

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            renderBoard[i][j] = board[i][j];

    if (currentPiece != NULL){
            boardDelBlock();
        int ghostY = y;
        while (canMove(0, 1, x, ghostY)) ghostY++;
            block2Board();
        for (int i=0; i<4; i++) for (int j=0; j<4; j++)
            if (currentPiece->getCell(i, j) != ' ' && ghostY + i < H)
                if (renderBoard[ghostY+i][x+j] == ' ') renderBoard[ghostY+i][x+j] = 'G'; 
                
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (currentPiece->getCell(i, j) != ' ' && y + i < H)
                    renderBoard[y + i][x + j] = currentPiece->getCell(i, j);
    }
    for (int i = 0; i < H; i++)
    {
        string leftStr = "                        ";
        if (i == 2) {
            if (canHold) leftStr = "   [ HOLD PIECE ] (C)   ";
            else         leftStr = "   \x1b[91m[ HOLD (LOCKED) ]\x1b[0m    ";
        }
        else if (i >= 3 && i <= 6) {
            leftStr = "        " + getPieceRowUI(holdPiece, i - 3) + "        ";
        }
        if (i == 9)
            leftStr = "     [ CONTROLS ]       ";
        else if (i == 11)
            leftStr = "   < / > : Di chuyen    ";
        else if (i == 12)
            leftStr = "     ^   : Xoay block   ";
        else if (i == 13)
            leftStr = "     v   : Roi nhanh    ";
        else if (i == 14)
            leftStr = "   SPACE : Hard Drop    ";
        else if (i == 15)
            leftStr = "     C   : Hold Piece   ";
        else if (i == 16)
            leftStr = "     P   : Pause Game   ";
        else if (i == 17)
            leftStr = "     Q   : Quit Game    ";
        s += leftStr;

        for (int j = 0; j < W; j++)
        {
            char cell = renderBoard[i][j];
            if (cell == '#')
                s += getColorCode(cell) + WALL + "\x1b[0m";
            else if (cell == 'G') 
                s += getColorCode(cell) + GHOST + "\x1b[0m";
            else if (cell != ' ')
                s += getColorCode(cell) + BLOCK + "\x1b[0m";
            else
                s += CELL;
        }
        if (i == 2) s += "    \x1b[96mSCORE:\x1b[0m " + to_string(score);
        if (i == 3) s += "    \x1b[93mLEVEL:\x1b[0m " + to_string(level);
        if (i == 4) s += "    LINES: " + to_string(linesClearedTotal);
        if (i == 7) s += "    [ NEXT 3 PIECES ]";
        if (i >= 8 && i <= 11) {
            string nextUI = "   ";
            queue<Piece*> tempQ = nextQueue;
            while (!tempQ.empty()) {
                nextUI += getPieceRowUI(tempQ.front(), i - 8) + "  ";
                tempQ.pop();
            }
            s += nextUI;
        }
        if (i == 13) s += "    \x1b[91m[ TOP 5 HIGH SCORES ]\x1b[0m";
        
        
        if (i >= 14 && i <= 18) {
            int rank = i - 13; 
            if ((size_t)(rank - 1) < highScores.size()) {
                s += "     " + to_string(rank) + ". " + to_string(highScores[rank - 1]);
            }
        }
        s += "\n";
    }
    cout << s;
}

Piece *getRandomPiece()
{
    switch (rand() % 7)
    {
    case 0:
        return new PieceI();
    case 1:
        return new PieceO();
    case 2:
        return new PieceT();
    case 3:
        return new PieceS();
    case 4:
        return new PieceZ();
    case 5:
        return new PieceJ();
    default:
        return new PieceL();
    }
}

void spawnNextPiece() {
    if (nextQueue.empty()) nextQueue.push(getRandomPiece());
    currentPiece = nextQueue.front();
    nextQueue.pop();
    nextQueue.push(getRandomPiece());
    x = 4; y = 0;
    canHold = true;
}

void initGame() {
    score = 0; level = 1; linesClearedTotal = 0; speed = 1000;
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            board[i][j] = ((i == H - 1) || (j == 0) || (j == W - 1)) ? '#' : ' ';
    while (!nextQueue.empty()) { delete nextQueue.front(); nextQueue.pop(); }
    if (holdPiece != NULL) { delete holdPiece; holdPiece = NULL; }
    for (int i = 0; i < 3; i++) nextQueue.push(getRandomPiece());
    spawnNextPiece();
}

int main()
{
    PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned)time(0));
    loadHighScores();
    initBoard();
    initGame();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hIn, &mode);
    SetConsoleMode(hIn, mode & ~ENABLE_QUICK_EDIT_MODE);

    bool playAgain = true;
    while (playAgain) {
        initGame();
        draw();           
        DWORD lastFall = GetTickCount();
        DWORD lastMove = GetTickCount();
        while (true)
    {
        bool changed = false;

        if (GetTickCount() - lastMove > 60)
        {

            boardDelBlock();

            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                if (canMove(-1, 0, x, y))
                {
                    x--;
                    changed = true;
                    FlushConsoleInputBuffer(hIn);
                    while (_kbhit())
                        _getch();
                }
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                if (canMove(1, 0, x, y))
                {
                    x++;
                    changed = true;
                    FlushConsoleInputBuffer(hIn);
                    while (_kbhit())
                        _getch();
                }
            }
            if (GetAsyncKeyState('C') & 1) {
                if (canHold) {
                    if (holdPiece == NULL) {
                        holdPiece = currentPiece;
                        holdPiece->reset();
                        spawnNextPiece();
                    }
                    else {
                        Piece* temp = currentPiece;
                        currentPiece = holdPiece;
                        holdPiece = temp;
                        holdPiece->reset();
                        x = 4; y = 0;
                        canHold = false;
                    }
                    changed = true;
                    FlushConsoleInputBuffer(hIn); while (_kbhit()) _getch();
                }
            }
            if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            {
                if (canMove(0, 1, x, y))
                {
                    y++;
                    changed = true;
                    FlushConsoleInputBuffer(hIn);
                    while (_kbhit())
                        _getch();
                }
            }
            if (GetAsyncKeyState(VK_UP) & 1)
            {
                char rotated[4][4];
                currentPiece->getRotatedShape(rotated);
                if (canMove(0, 0, x, y, rotated))
                {
                    currentPiece->applyRotation(rotated);
                    changed = true;
                    FlushConsoleInputBuffer(hIn);
                    while (_kbhit())
                        _getch();
                }
            }
            if (GetAsyncKeyState('P') & 1) {
                gotoxy(15, H / 2); cout << " \x1b[93m[ PAUSED ]\x1b[0m ";
                Sleep(300);
                FlushConsoleInputBuffer(hIn); while (_kbhit()) _getch();
                while (!(GetAsyncKeyState('P') & 0x8000)) { Sleep(50); }
                gotoxy(15, H / 2); cout << "            ";
                lastMove = GetTickCount();
                lastFall = GetTickCount();
                changed = true;
                FlushConsoleInputBuffer(hIn); while (_kbhit()) _getch();
            }
            if (GetAsyncKeyState(VK_SPACE) & 1) {
                int droppedCells = 0;
                while (canMove(0, 1, x, y)) { y++; droppedCells++; }
                score += droppedCells * 2;
                block2Board();
                removeLine();
                delete currentPiece;
                spawnNextPiece();
                if (!canMove(0, 0, x, y)) { 
                    saveHighScore(score);
                    draw();
                    break;
                }
                changed = true;
                lastFall = GetTickCount();
                FlushConsoleInputBuffer(hIn); while (_kbhit()) _getch();
            }

            if (GetAsyncKeyState('Q') & 0x8000) {
            saveHighScore(score); // <- Thêm dòng này ở đây
            break;
            }

            lastMove = GetTickCount();
            block2Board();
        }

        if (GetTickCount() - lastFall > (DWORD)speed)
        {
            boardDelBlock();
            if (canMove(0, 1, x, y))
            {
                y++;
                block2Board();
            }
            else
            {
                block2Board();
                removeLine();
                delete currentPiece;
              spawnNextPiece();
                if (!canMove(0, 0, x, y))
                {
                    saveHighScore(score);
                    system("cls");
                    cout << "GAME OVER!";
                    break;
                }
            }
            changed = true;
            lastFall = GetTickCount();
        }

        if (changed)
            draw();

        Sleep(10);
    }
        gotoxy(13, H / 2); cout << "   \x1b[91mGAME OVER!\x1b[0m   ";
        gotoxy(13, H / 2 + 1); cout << " Play Again? (Y/N): ";
        char choice;
        do { choice = _getch(); } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
        if (choice == 'n' || choice == 'N') playAgain = false;
        delete currentPiece;
        system("cls");
    }
    return 0;
}
