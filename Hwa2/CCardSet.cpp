#pragma once
#include "CCardSet.h"
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
extern const int Speed;						//패 속도
extern const int PromptSpeed;				//메시지 출력 속도
extern CCard HwaToo[48];
											// 패의 집합을 관리하는 클래스

int CCardSet::GetNum() { return Num; }
CCard CCardSet::GetCard(int idx) { return Card[idx]; }
void CCardSet::Reset() {
for (int i = 0; i < MaxCard; i++) Card[i].Name[0] = 0;
	Num = 0;
}

void CCardSet::InsertCard(CCard C) {
	int i;

	if (C.Name[0] == 0) return;
	for (i = 0; i < Num; i++) {
		if (C < CCardSet::Card[i]) break;
	}
	memmove(&CCardSet::Card[i + 1], &CCardSet::Card[i], sizeof(CCard) * (CCardSet::Num - i));
	CCardSet::Card[i] = C;
	CCardSet::Num++;
}
CCard CCardSet::RemoveCard(int idx) {
	assert(idx < CCardSet::Num);
	CCard C = CCardSet::Card[idx];
	memmove(&CCardSet::Card[idx], &CCardSet::Card[idx + 1], sizeof(CCard) * (Num - idx - 1));
	Num--;
	return C;
}

int CCardSet::FindSameCard(CCard C, int* pSame) {
	int i, num;
	int* p = pSame;

	for (i = 0, num = 0; i < CCardSet::Num; i++) {
		if (CCardSet::Card[i].GetNumber() == C.GetNumber()) {
			num++;
			*p++ = i;
		}
	}
	*p = -1;
	return num;
}

int CCardSet::FindFirstCard(const char* pName) {
	int i;

	for (i = 0; i < CCardSet::Num; i++) {
		if (strstr(CCardSet::Card[i].Name, pName) != NULL) {
			return i;
		}
	}
	return -1;
}

int CCardSet::GetMaxSeries() {
	int i, n, m, old = -1;

	for (i = 0, n = 1, m = 1; i < CCardSet::Num; i++) {
		if (old == CCardSet::Card[i].GetNumber()) {
			n++;
			m = max(n, m);
		}
		else {
			n = 1;
			old = CCardSet::Card[i].GetNumber();
		}
	}
	return m;
}


	void CDeck::Shuffle() {					//랜덤으로 패 초기화 후 배치
		int i, n;
		for (i = 0; i < MaxCard; i++) {
			do {
				n = G.random(MaxCard);
			} while (CDeck::Card[n].Name[0] != NULL);
			CDeck::Card[n] = HwaToo[i];
			CDeck::Num++;
		}
	}
	CCard CDeck::Pop() { return RemoveCard(CDeck::Num - 1); }		//차례대로 카드를 빼냄
	bool CDeck::IsEmpty() { return CDeck::Num == 0; }			//데크에 남아있는 패 개수 조사
	bool CDeck::IsNotLast() { return CDeck::Num > 1; }			//마지막 판인지 검사
	void CDeck::Draw(bool bFlip) {					//'■'로 데크에 뒤집을 패 표시
		G.gotoxy(CDeck::sx, CDeck::sy);
		std::cout << "■" << (bFlip ? CDeck::Card[CDeck::Num - 1].Name : "   ");
	}


// 게임을 하는 플레이어


void CPlayer::Draw(bool MyTurn) {
	int i, x;
	//플레이어의 턴일때 패 아래쪽 번호 표시
	for (i = 0, x = CPlayer::sx; i < CPlayer::Num; i++, x += CardGap) {
		G.gotoxy(x, CPlayer::sy);
		std::cout << CPlayer::Card[i];
		if (MyTurn) {
			G.gotoxy(x, CPlayer::sy + 1);
			std::cout << '[' << i + 1 << ']';
		}
	}
}


void CBlanket::Draw() {
	CPlayer::Draw(false);						//번호표시 X, 인자는 항상 false
}
void CBlanket::DrawSelNum(int* pSame) {			//동일카드 있을시 번호 출력
	int n;
	int* p;
	for (n = 1, p = pSame; *p != -1; p++, n++) {
		G.gotoxy(sx + *p * CardGap, sy - 1);
		std::cout << '[' << n << ']';
	}
}
void CBlanket::DrawTempCard(int idx, CCard C) {			//임시적으로 담요에 놔두는 패
	G.gotoxy(sx + idx * CardGap, sy + 1);
	std::cout << C;
}


// 플레이어가 먹은 카드의 집합(먹은 패와 점수관리)

	void CPlayerPae::Reset() { CCardSet::Reset(); OldScore = 6; nGo = 0; bShake = false; }
	int CPlayerPae::GetGo() { return nGo; }				//고 횟수
	void CPlayerPae::IncreaseGo() { nGo++; }			//고 증가

void CPlayerPae::Draw() {
	int i, kind;
	int x[4] = { sx, sx, sx, sx }, py = sy + 3;

	for (i = 0; i < Num; i++) {
		kind = Card[i].GetKind();
		if (kind >= 3) kind = 3;
		if (kind < 3) {
			G.gotoxy(x[kind], sy + kind);
			x[kind] += CardGap;
		}
		else {
			G.gotoxy(x[3], py);
			x[3] += CardGap;
			if (x[3] > 75) {
				x[3] = sx;
				py++;
			}
		}
		std::cout << Card[i];
	}
	G.gotoxy(sx + 20, sy);
	std::cout << "점수:" << CalcScore() << "점, " << nGo << "고 " << (bShake ? "흔듬" : "");
}

int CPlayerPae::RemovePee(int n, CCard* pCard) {
	int ns = 0, np = 0, tp;
	int i, idx, num = 0;
	CCard* p = pCard;

	for (i = 0; i < Num; i++) {
		if (Card[i].GetKind() == 3) ns++;
		if (Card[i].GetKind() == 4) np++;
	}
	tp = ns + np;
	if (tp == 0) return 0;

	switch (n) {
	case 1:
		if (np != 0) {
			*p++ = RemoveCard(FindFirstCard("피"));
			return 1;
		}
		else {
			*p++ = RemoveCard(FindFirstCard("쌍"));
			return 1;
		}
		break;
	case 2:
		if (ns != 0) {
			*p = RemoveCard(FindFirstCard("쌍"));
			return 1;
		}
		else {
			*p++ = RemoveCard(FindFirstCard("피"));
			num = 1;
			if (np >= 2) {
				*p++ = RemoveCard(FindFirstCard("피"));
				num = 2;
			}
			return num;
		}
	case 3:
		i = RemovePee(2, p);
		p += i;
		idx = RemovePee(1, p);
		return i + idx;
	default:
		return 0;
	}
}

int CPlayerPae::CalcScore() {
	int i, kind, n[4] = { 0, };
	int NewScore;
	static int gscore[] = { 0,0,0,3,4,15 };

	for (i = 0; i < Num; i++) {
		kind = Card[i].GetKind();
		if (kind == 3) n[kind]++;
		if (kind >= 3) kind = 3;
		n[kind]++;
	}
	NewScore = gscore[n[0]];
	if (n[0] == 5) NewScore += 5;                          // 5광 15점
	if (n[0] == 3 || n[0] == 4 && FindFirstCard("B광") != -1) NewScore--;			//비광포함이면 -1점(3,4광)
	if (n[1] >= 5) NewScore += (n[1] - 4);			//십, 오 5장 넘으면 1점
	if (n[2] >= 5) NewScore += (n[2] - 4);
	if (n[3] >= 10) NewScore += (n[3] - 9);			//피 10장 넘으면 1점
	if (FindFirstCard("8십") != -1 && FindFirstCard("5십") != -1 && FindFirstCard("2십") != -1) NewScore += 5;	//고도리
	if (FindFirstCard("1오") != -1 && FindFirstCard("2오") != -1 && FindFirstCard("3오") != -1) NewScore += 3;	//홍단
	if (FindFirstCard("4오") != -1 && FindFirstCard("5오") != -1 && FindFirstCard("7오") != -1) NewScore += 3;	//초단
	if (FindFirstCard("9오") != -1 && FindFirstCard("장오") != -1 && FindFirstCard("6오") != -1) NewScore += 3;	//청단
	return NewScore;
}