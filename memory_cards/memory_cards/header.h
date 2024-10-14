#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <locale>
#include <windows.h>
#include <codecvt>
#include <conio.h>
#include <chrono>
#include <thread> 

using namespace std;

const int ROWS = 2;
const int COLUMNS = 5;
const int HEIGHT_CARD = 5;
const int WIDTH_CARD = 10;
extern bool isGame;
extern bool isNextLevel;

void startGame(int timer);
void hideCursor();
void setColor(int textColor, int bgColor);
void drawMainField(int x, int y, int height, int width, int textColor, int backgroundColor);
void drawSideBar(int x, int y, int height, int width, int textColor, int backgroundColor);
void drawCard(int x, int y, int height, int width, int textColor, const wstring& content, bool isActive, bool isChoose);
void drawRectangle(int x, int y, int height, int width, int textColor, int backgroundColor, wchar_t* symb_activity);
bool compareCards(int firstCardIndex[2], int secondCardIndex[2]);
void changePoints(int textColor, int backgroundColor, int gameAttempts, int points, int timer);
void drawWin();
int getConsoleWidth();
void displayMenu(int index);
void menu();
void drawTimer(int textColor, int backgroundColor, int counter);
void drawLose();
void consoleSetting();
void playNextLevel(int timer);

struct Card {
    wstring text;
    wstring translation;
    bool isActive;
    bool isChoosed;
    int x;
    int y;
};

// Массив карточек
extern Card cards[ROWS][COLUMNS];
extern Card cards2[ROWS][COLUMNS];

#endif
