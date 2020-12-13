#pragma once
#include "CCard.h"
#include "Game.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

extern const int MaxCard;					//패의 총 갯수
extern const int CardGap;					//패사이의 간격
extern const int Speed;				//패 속도
extern const int PromptSpeed;			//메시지 출력 속도
extern CCard HwaToo[48];
// 패의 집합을 관리하는 클래스
class CCardSet {
protected:
	CCard Card[48];
	int Num;						//패 집합의 갯수
	const int sx, sy;					//화면상의 좌표값
	CCardSet(int asx, int asy) : sx(asx), sy(asy) { Num = 0; }

public:
	int GetNum();
	CCard GetCard(int idx);
	void Reset();
	void InsertCard(CCard C);				//패 정렬된 위치에 삽입
	CCard RemoveCard(int idx); //지정한 첨자의 패를 읽어서 리턴하고 해당 패를 집합에서 제거
	//인수로 전달된 패와 숫자가 일치하는 패의 개수 및 첨자 목록을 조사
	int FindSameCard(CCard C, int* pSame);
	//부분 문자열 검색을 통해 숫자나 종류가 일치하는 최초의 패를 검색
	int FindFirstCard(const char* pName);
	int GetMaxSeries();
};

// 담요 중앙에 카드를 쌓아 놓는 데크(■로 표시)
class CDeck : public CCardSet {
public:
	Game G;
	CDeck(int asx, int asy) : CCardSet(asx, asy) { }
	void Shuffle();
	CCard Pop();		//차례대로 카드를 빼냄
	bool IsEmpty();		//데크에 남아있는 패 개수 조사
	bool IsNotLast();			//마지막 판인지 검사
	void Draw(bool bFlip);					//'■'로 데크에 뒤집을 패 표시
};

// 게임을 하는 플레이어
class CPlayer : public CCardSet {
public:
	Game G;
	CPlayer(int asx, int asy) : CCardSet(asx, asy) { }
	void Draw(bool MyTurn);
};

// 게임이 진행되는 담요
class CBlanket : public CPlayer {
public:
	Game G;
	CBlanket(int asx, int asy) : CPlayer(asx, asy) { }
	void Draw();
	void DrawSelNum(int* pSame);			//동일카드 있을시 번호 출력
	void DrawTempCard(int idx, CCard C);			//임시적으로 담요에 놔두는 패
};

// 플레이어가 먹은 카드의 집합(먹은 패와 점수관리)
class CPlayerPae : public CCardSet {
private:
	int nGo;

public:
	Game G;
	int OldScore;			//이전 점수
	int bShake;				//흔듬
	CPlayerPae(int asx, int asy) : CCardSet(asx, asy) { OldScore = 6; nGo = 0; bShake = false; }
	void Reset();
	int GetGo();			//고 횟수
	void IncreaseGo();				//고 증가
	void Draw();						//먹은패를 화면에 정렬하여 출력
	int RemovePee(int n, CCard* pCard);			//피 한장 제거(쪽, 따닥, 3장먹기)
	int CalcScore();						//점수계산
};
