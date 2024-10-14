#include "header.h"

// �������� ��������
bool compareCards(int firstCardIndex[2], int secondCardIndex[2]) {
    int firstRow = firstCardIndex[0];
    int firstCol = firstCardIndex[1];
    int secondRow = secondCardIndex[0];
    int secondCol = secondCardIndex[1];

    // ���������� �������� ��������
    return cards[firstRow][firstCol].text == cards[secondRow][secondCol].translation;
}

// ������ ����, ������ �������
void startGame(int timer) {
    consoleSetting();

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // Draw main field, sidebar and points
    drawMainField(30, 1, 30, 100, 0, 9);
    drawSideBar(30, 1, 30, 20, 0, 14);
    changePoints(0, 14, 0, 0, 60);

    int startX = 62; // ��������� ���������� X ��� ��������
    int startY = 5;  // ��������� ���������� Y ��� ��������

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards[r][c].text, cards[r][c].isActive, cards[r][c].isChoosed);
        }
    }

    int choiseCount = 0;
    int arrChooseCards[2][2];
    int points = 0;
    int attempts = 0;

    int textColor = 13;
    int bgColor = 14;

    // ������
    int timerDuration = timer;
    auto startTime = chrono::steady_clock::now();

    while (true) {
        // ��������� ���������� �����
        auto currentTime = chrono::steady_clock::now();
        int elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int remainingTime = timerDuration - elapsedTime;

        // ��������� ������ �� ������
        if (isGame) {
            drawTimer(textColor, bgColor, remainingTime);
        }

        if (remainingTime == 0) {
            drawLose();
            isGame = false;
        }

        // ���������, �������� �� ��������� ������ ������ �� 1 �� 10
        if (_kbhit()) { // ���������, ������ �� �������
            int userChoice = _getch();

            if (userChoice >= 48 && userChoice <= 58) {
                int index;

                // ������������ ������ '0' ��� �������� 10
                if (userChoice == 48) {
                    index = (ROWS * COLUMNS) - 1; // ������ ��������� ��������  
                }
                else {
                    index = (char)userChoice - '1'; // ����������� ������ � ������
                }

                // ���������� ������ � ������� �� ������ �������
                int r = index / COLUMNS;
                int c = index % COLUMNS;

                // ����������� ��������� ��������
                if (choiseCount < 2) {
                    cards[r][c].isActive = !cards[r][c].isActive;

                    if (cards[r][c].isActive) {
                        arrChooseCards[choiseCount][0] = r; // ��������� ������ ������ ��������
                        arrChooseCards[choiseCount][1] = c;
                        choiseCount++;
                    }
                    else {
                        choiseCount--;
                    }

                    // �������������� ��������
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLUMNS; c++) {
                            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards[r][c].text, cards[r][c].isActive, cards[r][c].isChoosed);
                        }
                    }

                    // ���������, �������� �� �� ���� �������
                    if (choiseCount == 2) {
                        attempts++;

                        // ���������� ��������
                        if (compareCards(arrChooseCards[0], arrChooseCards[1])) {
                            cards[arrChooseCards[0][0]][arrChooseCards[0][1]].isChoosed = true;
                            cards[arrChooseCards[1][0]][arrChooseCards[1][1]].isChoosed = true;

                            points++;
                        }
                        else {
                            // ��������� ��������
                            cards[arrChooseCards[0][0]][arrChooseCards[0][1]].isActive = false;
                            cards[arrChooseCards[1][0]][arrChooseCards[1][1]].isActive = false;

                        }
                        // ���������� �������
                        choiseCount = 0;
                        changePoints(0, 14, attempts, points, remainingTime);
                    }
                }
            }

            // ������� ������� Esc - ����� �� ���������
            if (userChoice == 27) { // 27 - ��� ������� Esc
                break;
            }
        }
    }

    COORD pos = { 0, 35 };
    SetConsoleCursorPosition(output, pos);
}

// ��������
void drawCard(int x, int y, int height, int width, int textColor, const wstring& content, bool isActive, bool isChoose) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // ���������� �������� ����� ���������� � �������� � 1 ������
    int contentWidth = width - 2; // ��������� �������
    wstring truncatedContent = content;
    wchar_t withoutText[5] = L"card";

    // �������� ����������, ���� ��� ������� �������
    if (content.length() > contentWidth) {
        truncatedContent = content.substr(0, contentWidth);
    }

    // ��������� ����������� �������
    int contentStartX = x + (width - 2 - truncatedContent.length()) / 2 + 1; // ����� �� ��� X
    int contentStartY = y + (height - 1) / 2; // ����� �� ��� Y

    wchar_t isActSymb[3] = L"\u2261";
    wchar_t isNotActSymb[3] = L"\u2550";

    int backgroundColorActive = 13;
    int backgroundColorNotActive = 15;

    int textColorNotActive = 13;

    int textColorChoose = 0;
    int backgroundColorChoose = 14;

    // ������ ��������
    if (isChoose) {
        drawRectangle(x, y, height, width, textColorChoose, backgroundColorChoose, isActSymb);
        setColor(textColorChoose, backgroundColorChoose);
        COORD pos = { contentStartX, contentStartY };
        SetConsoleCursorPosition(output, pos);
        wcout << truncatedContent;
    }
    else if (isActive) {
        drawRectangle(x, y, height, width, textColor, backgroundColorActive, isActSymb);
        setColor(textColor, backgroundColorActive);
        COORD pos = { contentStartX, contentStartY };
        SetConsoleCursorPosition(output, pos);
        wcout << truncatedContent;
    }
    else {
        drawRectangle(x, y, height, width, textColorNotActive, backgroundColorNotActive, isNotActSymb);
        setColor(textColorNotActive, backgroundColorNotActive);
        COORD pos = { contentStartX, contentStartY };
        SetConsoleCursorPosition(output, pos);
        wcout << withoutText;
    }

    setColor(7, 0); // ����� �����
}

// �������� ��������� �����, ����� ������� drawWin() ���� points == 5,����� ������� playNextLevel(15);
void changePoints(int textColor, int backgroundColor, int gameAttempts, int points, int timer) {
    int x = 30;
    int y = 1;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (points == 5) {
        drawWin();
        isGame = false;
        if (!isNextLevel) {
            playNextLevel(15);
            isNextLevel = true;
        }
    }

    wstring sidebarTextPoints = L"Your points = ";
    wstring sidebarTextAttempts = L"Your attempts = ";

    int textStartX = x + 2; // ������ ������ �� ������ ����
    int textStartY = y + 5; // ������ ������ �� �������� ����

    int textStartX2 = x + 1; // ������ ������ �� ������ ����
    int textStartY2 = y + 8; // ������ ������ �� �������� ����

    // ����� ������ �� ��������� �����
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // ������������� ������� ��� ������
    setColor(textColor, backgroundColor); // ���� ��� ������
    wcout << sidebarTextPoints << points << endl;

    COORD textPos2 = { (short)textStartX2, (short)textStartY2 };
    SetConsoleCursorPosition(output, textPos2); // ������������� ������� ��� ������
    wcout << sidebarTextAttempts << gameAttempts << endl;

    setColor(7, 0); // ����� �����
}

// ��������� �������
void playNextLevel(int timer) {
    consoleSetting();
    isGame = true;

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // Draw main field, sidebar and points
    drawMainField(30, 1, 30, 100, 0, 9);
    drawSideBar(30, 1, 30, 20, 0, 14);
    changePoints(0, 14, 0, 0, 60);

    int startX = 62; // ��������� ���������� X ��� ��������
    int startY = 5;  // ��������� ���������� Y ��� ��������

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards2[r][c].text, cards2[r][c].isActive, cards2[r][c].isChoosed);
        }
    }

    int choiseCount = 0;
    int arrChooseCards[2][2];
    int points = 0;
    int attempts = 0;

    // ������
    int timerDuration = timer;
    auto startTime = chrono::steady_clock::now();

    while (true) {
        // ��������� ���������� �����
        auto currentTime = chrono::steady_clock::now();
        int elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int remainingTime = timerDuration - elapsedTime;

        // ��������� ������ �� ������
        if (isGame) {
            drawTimer(13, 14, remainingTime);
        }

        if (remainingTime == 0) {
            drawLose();
            isGame = false;
        }

        // ���������, �������� �� ��������� ������ ������ �� 1 �� 10
        if (_kbhit()) { // ���������, ������ �� �������
            int userChoice = _getch();

            if (userChoice >= 48 && userChoice <= 58) {
                int index;

                // ������������ ������ '0' ��� �������� 10
                if (userChoice == 48) {
                    index = (ROWS * COLUMNS) - 1; // ������ ��������� ��������  
                }
                else {
                    index = (char)userChoice - '1'; // ����������� ������ � ������
                }

                // ���������� ������ � ������� �� ������ �������
                int r = index / COLUMNS;
                int c = index % COLUMNS;

                // ����������� ��������� ��������
                if (choiseCount < 2) {
                    cards2[r][c].isActive = !cards2[r][c].isActive;

                    if (cards[r][c].isActive) {
                        arrChooseCards[choiseCount][0] = r; // ��������� ������ ������ ��������
                        arrChooseCards[choiseCount][1] = c;
                        choiseCount++;
                    }
                    else {
                        choiseCount--;
                    }

                    // �������������� ��������
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLUMNS; c++) {
                            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards2[r][c].text, cards2[r][c].isActive, cards2[r][c].isChoosed);
                        }
                    }

                    // ���������, �������� �� �� ���� �������
                    if (choiseCount == 2) {
                        attempts++;

                        // ���������� ��������
                        if (compareCards(arrChooseCards[0], arrChooseCards[1])) {
                            cards2[arrChooseCards[0][0]][arrChooseCards[0][1]].isChoosed = true;
                            cards2[arrChooseCards[1][0]][arrChooseCards[1][1]].isChoosed = true;

                            points++;
                        }
                        else {
                            // ��������� ��������
                            cards2[arrChooseCards[0][0]][arrChooseCards[0][1]].isActive = false;
                            cards2[arrChooseCards[1][0]][arrChooseCards[1][1]].isActive = false;

                        }
                        // ���������� �������
                        choiseCount = 0;
                        changePoints(0, 14, attempts, points, remainingTime);
                    }
                }
            }

            // ������� ������� Esc - ����� �� ���������
            if (userChoice == 27) { // 27 - ��� ������� Esc
                break;
            }
        }
    }

    COORD pos = { 0, 35 };
    SetConsoleCursorPosition(output, pos);
}

// ����� �����
void drawRectangle(int x, int y, int height, int width, int textColor, int backgroundColor, wchar_t* symb_activity) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    setColor(textColor, backgroundColor);

    for (int i = y; i < height + y; i++) {
        for (int j = x; j < width + x; j++) {
            COORD pos = { j, i };
            SetConsoleCursorPosition(output, pos);
            setColor(textColor, backgroundColor);

            if (i == y && j == x) {
                wcout << L"\u2554"; // ���� ����
            }
            else if (i == y && j == width + x - 1) {
                wcout << L"\u2557"; // ���� �����
            }
            else if (i == height + y - 1 && j == x) {
                wcout << L"\u255A"; // ��� ����
            }
            else if (i == height + y - 1 && j == width + x - 1) {
                wcout << L"\u255D"; // ��� �����
            }
            else if ((i < height + y - 1 && j == x) || (i < height + y - 1 && j == width + x - 1)) {
                wcout << L"\u2551"; // ����� ������������
            }
            else if ((i == height + y - 1 && j < width + x - 1) || (i == y && j < width + x - 1)) {
                wcout << symb_activity;
            }
            else {
                cout << " ";
            }
            setColor(7, 0);
        }
    }
}

// ����� ��������� ����
void drawMainField(int x, int y, int height, int width, int textColor, int backgroundColor) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = y; i < height + y; i++) {
        for (int j = x; j < width + x; j++) {
            COORD pos = { j, i };
            SetConsoleCursorPosition(output, pos);
            setColor(textColor, backgroundColor);

            if (i == y && j == x) {
                wcout << L"\u2554"; // ���� ����
            }
            else if (i == y && j == width + x - 1) {
                wcout << L"\u2557"; // ���� �����
            }
            else if (i == height + y - 1 && j == x) {
                wcout << L"\u255A"; // ��� ����
            }
            else if (i == height + y - 1 && j == width + x - 1) {
                wcout << L"\u255D"; // ��� �����
            }
            else if ((i < height + y - 1 && j == x) || (i < height + y - 1 && j == width + x - 1)) {
                wcout << L"\u2551"; // ����� ������������
            }
            else if ((i == height + y - 1 && j < width + x - 1) || (i == y && j < width + x - 1)) {
                wcout << L"\u2500"; // ����� ��������������
            }
            else {
                cout << " ";
            }
            setColor(7, 0);
        }
    }
}

// ����� ������� ������
void drawSideBar(int x, int y, int height, int width, int textColor, int backgroundColor) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = y; i < height + y; i++) {
        for (int j = x; j < width + x; j++) {
            COORD pos = { j, i };
            SetConsoleCursorPosition(output, pos); // ������������� ������� �������
            setColor(textColor, backgroundColor); // ������������� ����

            if (i == y && j == x) {
                wcout << L"\u2554"; // ���� ����
            }
            else if (i == height + y - 1 && j == x) {
                wcout << L"\u255A"; // ��� ����
            }
            else if (i < height + y && j == width + x - 1) {
                wcout << L"\u2593"; // ����� ������������
            }
            else if (i < height + y - 1 && j == x) {
                wcout << L"\u2551"; // ����� ������������
            }
            else if ((i == height + y - 1 && j < width + x - 1) || (i == y && j < width + x - 1)) {
                wcout << L"\u2500"; // ����� ��������������
            }
            else {
                cout << " ";
            }
            setColor(7, 0); // ����� �����
        }
    }
}

// ������� ������ ��� ���� �������
void drawWin() {
    int x = 80;
    int y = 20;
    int backgroundColor = 9;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    wstring text = L"You win!!!";

    int textStartX = x;
    int textStartY = y;
    Sleep(1500);

    // ����� ������ �� ��������� �����
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // ������������� ������� ��� ������
    setColor(0, backgroundColor); // ���� ��� ������
    wcout << text << endl;

    setColor(7, 0); // ����� �����
}

// ������� ������ ��� ���� ��������
void drawLose() {
    int x = 80;
    int y = 20;
    int backgroundColor = 9;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    wstring text = L"Game over.You lose...";

    int textStartX = x;
    int textStartY = y;
    Sleep(1500);

    // ����� ������ �� ��������� �����
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // ������������� ������� ��� ������
    setColor(0, backgroundColor); // ���� ��� ������
    wcout << text << endl;

    setColor(7, 0); // ����� �����
}

// ������� ������
void drawTimer(int textColor, int backgroundColor, int counter) {
    int x = 32;
    int y = 13;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    wstring timerText = L"You have = ";

    int textStartX = x;
    int textStartY = y;

    // ����� ������ �� ��������� �����
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // ������������� ������� ��� ������
    setColor(textColor, backgroundColor);
    wcout << timerText << counter << " s" << endl;

    setColor(7, 0); // ����� �����
}

// ������� ��� ��������� ������ �������
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1; // ������ ����
}

// ������� ��������� �������
void hideCursor() {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(output, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(output, &cursorInfo);
}

// ��������� �������
void consoleSetting() {
    locale::global(locale("en_US.UTF-8"));
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();

    // ������������� ������ ��� ��������� UTF-8
    setlocale(LC_ALL, "en_US.UTF-8");

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.dwSize = 100;
    SetConsoleCursorInfo(output, &info);
}
// ������� ��� ��������� �����
void setColor(int textColor, int backgroundColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = textColor + backgroundColor * 16;
    SetConsoleTextAttribute(hConsole, colorAttribute);
}

// ����
void menu() {
    int index = 0; // ������ ���������� �������� ����

    while (true) {
        displayMenu(index); // �������� ����

        // ��������� ������� �������
        int c = _getch();
        if (c == 72) { // �����
            index--;
        }
        if (c == 80) { // ����
            index++;
        }
        if (index < 0) {
            index = 4; // ������� � ����� �������
        }
        if (index > 4) {
            index = 0; // ������� � ������ �������
        }

        if (c == 13) { // Enter
            switch (index) {
            case 0:
                startGame(25);
                return;
            case 1:
                // ������ ����������� ����
                return;
            case 2:
                // ������ ����������� �����
                return;
            case 3:
                // ������ ���� �����
                return;
            case 4:
                exit(0); // ����� �� ���������
            }
        }
    }
}

// ������� ��� ����������� �������� ����
void displayMenu(int index) {
    const char* options[] = { "Start Game", "Continue", "Score", "Options", "Exit" };
    const int menuSize = sizeof(options) / sizeof(options[0]);

    system("cls"); // ������� �������

    // ���������
    setColor(14, 0);
    cout << "\n\n\n";
    cout << string((getConsoleWidth() - 24) / 2, ' ') << "=========================\n";
    cout << string((getConsoleWidth() - 19) / 2, ' ') << "    Memory Card Game\n";
    cout << string((getConsoleWidth() - 24) / 2, ' ') << "=========================\n\n";

    // ����������� ������� ����
    for (int i = 0; i < menuSize; i++) {
        setColor(index == i ? 5 : 7, 0);
        cout << string((getConsoleWidth() - strlen(options[i])) / 2, ' ') << options[i] << endl; // �������������
    }

    setColor(7, 0); // ����� �����
}

