#pragma once
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

// 프로그램 작성 도우미 클래스
class Game {
public:
	void gotoxy(int x, int y);

	int random(int n);

	void delay(int tick);
};