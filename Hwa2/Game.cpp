#pragma once
#include "Game.h"
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

// ���α׷� �ۼ� ����� Ŭ����

	void Game::gotoxy(int x, int y) {				//Ŀ�� ��ġ �̵�
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	int Game::random(int n) {					//���� ����
		return (rand() % (n));
	}

	void Game::delay(int tick) {					//�ð����� �Լ�
		for (int i = 0; i < tick; i++);
	}
