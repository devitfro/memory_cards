#include "header.h"

Card cards[ROWS][COLUMNS]{
{ {L"Cat", L"�����", false, false, 0, 0}, {L"�����", L"Cat", false, false, 1, 0}, {L"Dog", L"������", false, false, 2, 0}, {L"������", L"Dog", false, false, 3, 0}, {L"���", L"Lion", false, false, 4, 0} },
{ {L"Pig", L"������", false, false, 0, 1}, {L"������", L"Pig", false, false, 1, 1}, {L"Zebra", L"�����", false, false, 2, 1}, {L"Lion", L"���", false, false, 3, 1}, {L"�����", L"Zebra", false, false, 4, 1} },
};

Card cards2[ROWS][COLUMNS]{
{ {L"Mouse ", L"�����", false, false, 0, 0}, {L"�����", L"Mouse", false, false, 1, 0}, {L"Giraffe", L"�����", false, false, 2, 0}, {L"�����", L"Giraffe", false, false, 3, 0}, {L"������", L"Cow", false, false, 4, 0} },
{ {L"Bird", L"������", false, false, 0, 1}, {L"������", L"Bird", false, false, 1, 1}, {L"Donkey", L"�����", false, false, 2, 1}, {L"Cow", L"������", false, false, 3, 1}, {L"�����", L"Donkey", false, false, 4, 1} },
};

// ���������� ���������� ���������� ������� ��������� ������������ �� ���� � ��������� �� ������ �������
bool isGame = true;
bool isNextLevel = false;;


int main()
{
    // ����������� ������
    PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    menu();
    return 0;
}

