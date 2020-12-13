#pragma once
#include "Game.h"
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

// 프로그램 작성 도우미 클래스

	void Game::gotoxy(int x, int y) {				//커서 위치 이동
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	int Game::random(int n) {					//랜덤 생성
		return (rand() % (n));
	}

	void Game::delay(int tick) {					//시간지연 함수
		for (int i = 0; i < tick; i++);
	}
