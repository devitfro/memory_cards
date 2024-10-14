#include "header.h"

// Сравнить карточки
bool compareCards(int firstCardIndex[2], int secondCardIndex[2]) {
    int firstRow = firstCardIndex[0];
    int firstCol = firstCardIndex[1];
    int secondRow = secondCardIndex[0];
    int secondCol = secondCardIndex[1];

    // Сравниваем переводы карточек
    return cards[firstRow][firstCol].text == cards[secondRow][secondCol].translation;
}

// Начало игры, первый уровень
void startGame(int timer) {
    consoleSetting();

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // Draw main field, sidebar and points
    drawMainField(30, 1, 30, 100, 0, 9);
    drawSideBar(30, 1, 30, 20, 0, 14);
    changePoints(0, 14, 0, 0, 60);

    int startX = 62; // Начальная координата X для карточек
    int startY = 5;  // Начальная координата Y для карточек

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

    // Таймер
    int timerDuration = timer;
    auto startTime = chrono::steady_clock::now();

    while (true) {
        // Вычисляем оставшееся время
        auto currentTime = chrono::steady_clock::now();
        int elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int remainingTime = timerDuration - elapsedTime;

        // Обновляем таймер на экране
        if (isGame) {
            drawTimer(textColor, bgColor, remainingTime);
        }

        if (remainingTime == 0) {
            drawLose();
            isGame = false;
        }

        // Проверяем, является ли введенный символ цифрой от 1 до 10
        if (_kbhit()) { // Проверяем, нажата ли клавиша
            int userChoice = _getch();

            if (userChoice >= 48 && userChoice <= 58) {
                int index;

                // Обрабатываем кнопку '0' как карточку 10
                if (userChoice == 48) {
                    index = (ROWS * COLUMNS) - 1; // Индекс последней карточки  
                }
                else {
                    index = (char)userChoice - '1'; // Преобразуем символ в индекс
                }

                // Определяем строку и столбец на основе индекса
                int r = index / COLUMNS;
                int c = index % COLUMNS;

                // Переключаем состояние карточки
                if (choiseCount < 2) {
                    cards[r][c].isActive = !cards[r][c].isActive;

                    if (cards[r][c].isActive) {
                        arrChooseCards[choiseCount][0] = r; // Сохраняем индекс первой карточки
                        arrChooseCards[choiseCount][1] = c;
                        choiseCount++;
                    }
                    else {
                        choiseCount--;
                    }

                    // Перерисовываем карточки
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLUMNS; c++) {
                            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards[r][c].text, cards[r][c].isActive, cards[r][c].isChoosed);
                        }
                    }

                    // Проверяем, достигли ли мы двух выборов
                    if (choiseCount == 2) {
                        attempts++;

                        // Сравниваем карточки
                        if (compareCards(arrChooseCards[0], arrChooseCards[1])) {
                            cards[arrChooseCards[0][0]][arrChooseCards[0][1]].isChoosed = true;
                            cards[arrChooseCards[1][0]][arrChooseCards[1][1]].isChoosed = true;

                            points++;
                        }
                        else {
                            // Закрываем карточки
                            cards[arrChooseCards[0][0]][arrChooseCards[0][1]].isActive = false;
                            cards[arrChooseCards[1][0]][arrChooseCards[1][1]].isActive = false;

                        }
                        // Сбрасываем счётчик
                        choiseCount = 0;
                        changePoints(0, 14, attempts, points, remainingTime);
                    }
                }
            }

            // Нажатие клавиши Esc - выход из программы
            if (userChoice == 27) { // 27 - код клавиши Esc
                break;
            }
        }
    }

    COORD pos = { 0, 35 };
    SetConsoleCursorPosition(output, pos);
}

// Карточка
void drawCard(int x, int y, int height, int width, int textColor, const wstring& content, bool isActive, bool isChoose) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // Содержимое карточки будет выводиться с отступом в 1 символ
    int contentWidth = width - 2; // Учитываем границы
    wstring truncatedContent = content;
    wchar_t withoutText[5] = L"card";

    // Обрезаем содержимое, если оно слишком длинное
    if (content.length() > contentWidth) {
        truncatedContent = content.substr(0, contentWidth);
    }

    // Вычисляем центральные позиции
    int contentStartX = x + (width - 2 - truncatedContent.length()) / 2 + 1; // Центр по оси X
    int contentStartY = y + (height - 1) / 2; // Центр по оси Y

    wchar_t isActSymb[3] = L"\u2261";
    wchar_t isNotActSymb[3] = L"\u2550";

    int backgroundColorActive = 13;
    int backgroundColorNotActive = 15;

    int textColorNotActive = 13;

    int textColorChoose = 0;
    int backgroundColorChoose = 14;

    // Рисуем карточку
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

    setColor(7, 0); // Сброс цвета
}

// Изменить состояние очков, вызов функции drawWin() если points == 5,вызов функции playNextLevel(15);
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

    int textStartX = x + 2; // Отступ текста от левого края
    int textStartY = y + 5; // Отступ текста от верхнего края

    int textStartX2 = x + 1; // Отступ текста от левого края
    int textStartY2 = y + 8; // Отступ текста от верхнего края

    // Вывод текста за пределами цикла
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // Устанавливаем позицию для текста
    setColor(textColor, backgroundColor); // Цвет для текста
    wcout << sidebarTextPoints << points << endl;

    COORD textPos2 = { (short)textStartX2, (short)textStartY2 };
    SetConsoleCursorPosition(output, textPos2); // Устанавливаем позицию для текста
    wcout << sidebarTextAttempts << gameAttempts << endl;

    setColor(7, 0); // Сброс цвета
}

// Следующий уровень
void playNextLevel(int timer) {
    consoleSetting();
    isGame = true;

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // Draw main field, sidebar and points
    drawMainField(30, 1, 30, 100, 0, 9);
    drawSideBar(30, 1, 30, 20, 0, 14);
    changePoints(0, 14, 0, 0, 60);

    int startX = 62; // Начальная координата X для карточек
    int startY = 5;  // Начальная координата Y для карточек

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards2[r][c].text, cards2[r][c].isActive, cards2[r][c].isChoosed);
        }
    }

    int choiseCount = 0;
    int arrChooseCards[2][2];
    int points = 0;
    int attempts = 0;

    // Таймер
    int timerDuration = timer;
    auto startTime = chrono::steady_clock::now();

    while (true) {
        // Вычисляем оставшееся время
        auto currentTime = chrono::steady_clock::now();
        int elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int remainingTime = timerDuration - elapsedTime;

        // Обновляем таймер на экране
        if (isGame) {
            drawTimer(13, 14, remainingTime);
        }

        if (remainingTime == 0) {
            drawLose();
            isGame = false;
        }

        // Проверяем, является ли введенный символ цифрой от 1 до 10
        if (_kbhit()) { // Проверяем, нажата ли клавиша
            int userChoice = _getch();

            if (userChoice >= 48 && userChoice <= 58) {
                int index;

                // Обрабатываем кнопку '0' как карточку 10
                if (userChoice == 48) {
                    index = (ROWS * COLUMNS) - 1; // Индекс последней карточки  
                }
                else {
                    index = (char)userChoice - '1'; // Преобразуем символ в индекс
                }

                // Определяем строку и столбец на основе индекса
                int r = index / COLUMNS;
                int c = index % COLUMNS;

                // Переключаем состояние карточки
                if (choiseCount < 2) {
                    cards2[r][c].isActive = !cards2[r][c].isActive;

                    if (cards[r][c].isActive) {
                        arrChooseCards[choiseCount][0] = r; // Сохраняем индекс первой карточки
                        arrChooseCards[choiseCount][1] = c;
                        choiseCount++;
                    }
                    else {
                        choiseCount--;
                    }

                    // Перерисовываем карточки
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLUMNS; c++) {
                            drawCard(startX + c * (WIDTH_CARD + 2), startY + r * (HEIGHT_CARD + 1), HEIGHT_CARD, WIDTH_CARD, 15, cards2[r][c].text, cards2[r][c].isActive, cards2[r][c].isChoosed);
                        }
                    }

                    // Проверяем, достигли ли мы двух выборов
                    if (choiseCount == 2) {
                        attempts++;

                        // Сравниваем карточки
                        if (compareCards(arrChooseCards[0], arrChooseCards[1])) {
                            cards2[arrChooseCards[0][0]][arrChooseCards[0][1]].isChoosed = true;
                            cards2[arrChooseCards[1][0]][arrChooseCards[1][1]].isChoosed = true;

                            points++;
                        }
                        else {
                            // Закрываем карточки
                            cards2[arrChooseCards[0][0]][arrChooseCards[0][1]].isActive = false;
                            cards2[arrChooseCards[1][0]][arrChooseCards[1][1]].isActive = false;

                        }
                        // Сбрасываем счётчик
                        choiseCount = 0;
                        changePoints(0, 14, attempts, points, remainingTime);
                    }
                }
            }

            // Нажатие клавиши Esc - выход из программы
            if (userChoice == 27) { // 27 - код клавиши Esc
                break;
            }
        }
    }

    COORD pos = { 0, 35 };
    SetConsoleCursorPosition(output, pos);
}

// Вывод рамки
void drawRectangle(int x, int y, int height, int width, int textColor, int backgroundColor, wchar_t* symb_activity) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    setColor(textColor, backgroundColor);

    for (int i = y; i < height + y; i++) {
        for (int j = x; j < width + x; j++) {
            COORD pos = { j, i };
            SetConsoleCursorPosition(output, pos);
            setColor(textColor, backgroundColor);

            if (i == y && j == x) {
                wcout << L"\u2554"; // ВЕРХ ЛЕВО
            }
            else if (i == y && j == width + x - 1) {
                wcout << L"\u2557"; // ВЕРХ ПРАВО
            }
            else if (i == height + y - 1 && j == x) {
                wcout << L"\u255A"; // НИЗ ЛЕВО
            }
            else if (i == height + y - 1 && j == width + x - 1) {
                wcout << L"\u255D"; // НИЗ ПРАВО
            }
            else if ((i < height + y - 1 && j == x) || (i < height + y - 1 && j == width + x - 1)) {
                wcout << L"\u2551"; // ПАЛКА ВЕРТИКАЛЬНАЯ
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

// Вывод основного поля
void drawMainField(int x, int y, int height, int width, int textColor, int backgroundColor) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = y; i < height + y; i++) {
        for (int j = x; j < width + x; j++) {
            COORD pos = { j, i };
            SetConsoleCursorPosition(output, pos);
            setColor(textColor, backgroundColor);

            if (i == y && j == x) {
                wcout << L"\u2554"; // ВЕРХ ЛЕВО
            }
            else if (i == y && j == width + x - 1) {
                wcout << L"\u2557"; // ВЕРХ ПРАВО
            }
            else if (i == height + y - 1 && j == x) {
                wcout << L"\u255A"; // НИЗ ЛЕВО
            }
            else if (i == height + y - 1 && j == width + x - 1) {
                wcout << L"\u255D"; // НИЗ ПРАВО
            }
            else if ((i < height + y - 1 && j == x) || (i < height + y - 1 && j == width + x - 1)) {
                wcout << L"\u2551"; // ПАЛКА ВЕРТИКАЛЬНАЯ
            }
            else if ((i == height + y - 1 && j < width + x - 1) || (i == y && j < width + x - 1)) {
                wcout << L"\u2500"; // ПАЛКА ГОРИЗОНТАЛЬНАЯ
            }
            else {
                cout << " ";
            }
            setColor(7, 0);
        }
    }
}

// Вывод боковой панели
void drawSideBar(int x, int y, int height, int width, int textColor, int backgroundColor) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = y; i < height + y; i++) {
        for (int j = x; j < width + x; j++) {
            COORD pos = { j, i };
            SetConsoleCursorPosition(output, pos); // Устанавливаем позицию курсора
            setColor(textColor, backgroundColor); // Устанавливаем цвет

            if (i == y && j == x) {
                wcout << L"\u2554"; // ВЕРХ ЛЕВО
            }
            else if (i == height + y - 1 && j == x) {
                wcout << L"\u255A"; // НИЗ ЛЕВО
            }
            else if (i < height + y && j == width + x - 1) {
                wcout << L"\u2593"; // ПАЛКА ВЕРТИКАЛЬНАЯ
            }
            else if (i < height + y - 1 && j == x) {
                wcout << L"\u2551"; // ПАЛКА ВЕРТИКАЛЬНАЯ
            }
            else if ((i == height + y - 1 && j < width + x - 1) || (i == y && j < width + x - 1)) {
                wcout << L"\u2500"; // ПАЛКА ГОРИЗОНТАЛЬНАЯ
            }
            else {
                cout << " ";
            }
            setColor(7, 0); // Сброс цвета
        }
    }
}

// Выводим данные что юзер победил
void drawWin() {
    int x = 80;
    int y = 20;
    int backgroundColor = 9;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    wstring text = L"You win!!!";

    int textStartX = x;
    int textStartY = y;
    Sleep(1500);

    // Вывод текста за пределами цикла
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // Устанавливаем позицию для текста
    setColor(0, backgroundColor); // Цвет для текста
    wcout << text << endl;

    setColor(7, 0); // Сброс цвета
}

// Выводим данные что юзер проиграл
void drawLose() {
    int x = 80;
    int y = 20;
    int backgroundColor = 9;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    wstring text = L"Game over.You lose...";

    int textStartX = x;
    int textStartY = y;
    Sleep(1500);

    // Вывод текста за пределами цикла
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // Устанавливаем позицию для текста
    setColor(0, backgroundColor); // Цвет для текста
    wcout << text << endl;

    setColor(7, 0); // Сброс цвета
}

// Выводим таймер
void drawTimer(int textColor, int backgroundColor, int counter) {
    int x = 32;
    int y = 13;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    wstring timerText = L"You have = ";

    int textStartX = x;
    int textStartY = y;

    // Вывод текста за пределами цикла
    COORD textPos = { (short)textStartX, (short)textStartY };
    SetConsoleCursorPosition(output, textPos); // Устанавливаем позицию для текста
    setColor(textColor, backgroundColor);
    wcout << timerText << counter << " s" << endl;

    setColor(7, 0); // Сброс цвета
}

// Функция для получения ширины консоли
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1; // ширина окна
}

// Убираем видимость курсора
void hideCursor() {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(output, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(output, &cursorInfo);
}

// Настройки консоли
void consoleSetting() {
    locale::global(locale("en_US.UTF-8"));
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();

    // Устанавливаем локаль для поддержки UTF-8
    setlocale(LC_ALL, "en_US.UTF-8");

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.dwSize = 100;
    SetConsoleCursorInfo(output, &info);
}
// Функция для настройки цвета
void setColor(int textColor, int backgroundColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = textColor + backgroundColor * 16;
    SetConsoleTextAttribute(hConsole, colorAttribute);
}

// Меню
void menu() {
    int index = 0; // Индекс выбранного элемента меню

    while (true) {
        displayMenu(index); // Показать меню

        // Получение нажатой клавиши
        int c = _getch();
        if (c == 72) { // Вверх
            index--;
        }
        if (c == 80) { // Вниз
            index++;
        }
        if (index < 0) {
            index = 4; // Переход в конец массива
        }
        if (index > 4) {
            index = 0; // Переход в начало массива
        }

        if (c == 13) { // Enter
            switch (index) {
            case 0:
                startGame(25);
                return;
            case 1:
                // Логика продолжения игры
                return;
            case 2:
                // Логика отображения счета
                return;
            case 3:
                // Логика меню опций
                return;
            case 4:
                exit(0); // Выход из программы
            }
        }
    }
}

// Функция для отображения главного меню
void displayMenu(int index) {
    const char* options[] = { "Start Game", "Continue", "Score", "Options", "Exit" };
    const int menuSize = sizeof(options) / sizeof(options[0]);

    system("cls"); // Очистка консоли

    // Заголовок
    setColor(14, 0);
    cout << "\n\n\n";
    cout << string((getConsoleWidth() - 24) / 2, ' ') << "=========================\n";
    cout << string((getConsoleWidth() - 19) / 2, ' ') << "    Memory Card Game\n";
    cout << string((getConsoleWidth() - 24) / 2, ' ') << "=========================\n\n";

    // Отображение пунктов меню
    for (int i = 0; i < menuSize; i++) {
        setColor(index == i ? 5 : 7, 0);
        cout << string((getConsoleWidth() - strlen(options[i])) / 2, ' ') << options[i] << endl; // Центрирование
    }

    setColor(7, 0); // Сброс цвета
}

