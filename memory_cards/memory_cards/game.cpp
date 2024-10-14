#include "header.h"

Card cards[ROWS][COLUMNS]{
{ {L"Cat", L"Кошка", false, false, 0, 0}, {L"Кошка", L"Cat", false, false, 1, 0}, {L"Dog", L"Собака", false, false, 2, 0}, {L"Собака", L"Dog", false, false, 3, 0}, {L"Лев", L"Lion", false, false, 4, 0} },
{ {L"Pig", L"Свинка", false, false, 0, 1}, {L"Свинка", L"Pig", false, false, 1, 1}, {L"Zebra", L"Зебра", false, false, 2, 1}, {L"Lion", L"Лев", false, false, 3, 1}, {L"Зебра", L"Zebra", false, false, 4, 1} },
};

Card cards2[ROWS][COLUMNS]{
{ {L"Mouse ", L"Мышка", false, false, 0, 0}, {L"Мышка", L"Mouse", false, false, 1, 0}, {L"Giraffe", L"Жираф", false, false, 2, 0}, {L"Жираф", L"Giraffe", false, false, 3, 0}, {L"Корова", L"Cow", false, false, 4, 0} },
{ {L"Bird", L"Птичка", false, false, 0, 1}, {L"Птичка", L"Bird", false, false, 1, 1}, {L"Donkey", L"Ослик", false, false, 2, 1}, {L"Cow", L"Корова", false, false, 3, 1}, {L"Ослик", L"Donkey", false, false, 4, 1} },
};

// Обьявление глобальной переменной которая указывает продолжается ли игра и запускать ли второй уровень
bool isGame = true;
bool isNextLevel = false;;


int main()
{
    // Подключение музыки
    PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    menu();
    return 0;
}

