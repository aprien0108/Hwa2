#pragma once
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

// ���α׷� �ۼ� ����� Ŭ����
class Game {
public:
	void gotoxy(int x, int y);

	int random(int n);

	void delay(int tick);
};