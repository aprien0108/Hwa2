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

extern const int MaxCard;					//���� �� ����
extern const int CardGap;					//�л����� ����
extern const int Speed;						//�� �ӵ�
extern const int PromptSpeed;				//�޽��� ��� �ӵ�
extern CCard HwaToo[48];
											// ���� ������ �����ϴ� Ŭ����

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


	void CDeck::Shuffle() {					//�������� �� �ʱ�ȭ �� ��ġ
		int i, n;
		for (i = 0; i < MaxCard; i++) {
			do {
				n = G.random(MaxCard);
			} while (CDeck::Card[n].Name[0] != NULL);
			CDeck::Card[n] = HwaToo[i];
			CDeck::Num++;
		}
	}
	CCard CDeck::Pop() { return RemoveCard(CDeck::Num - 1); }		//���ʴ�� ī�带 ����
	bool CDeck::IsEmpty() { return CDeck::Num == 0; }			//��ũ�� �����ִ� �� ���� ����
	bool CDeck::IsNotLast() { return CDeck::Num > 1; }			//������ ������ �˻�
	void CDeck::Draw(bool bFlip) {					//'��'�� ��ũ�� ������ �� ǥ��
		G.gotoxy(CDeck::sx, CDeck::sy);
		std::cout << "��" << (bFlip ? CDeck::Card[CDeck::Num - 1].Name : "   ");
	}


// ������ �ϴ� �÷��̾�


void CPlayer::Draw(bool MyTurn) {
	int i, x;
	//�÷��̾��� ���϶� �� �Ʒ��� ��ȣ ǥ��
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
	CPlayer::Draw(false);						//��ȣǥ�� X, ���ڴ� �׻� false
}
void CBlanket::DrawSelNum(int* pSame) {			//����ī�� ������ ��ȣ ���
	int n;
	int* p;
	for (n = 1, p = pSame; *p != -1; p++, n++) {
		G.gotoxy(sx + *p * CardGap, sy - 1);
		std::cout << '[' << n << ']';
	}
}
void CBlanket::DrawTempCard(int idx, CCard C) {			//�ӽ������� ��信 ���δ� ��
	G.gotoxy(sx + idx * CardGap, sy + 1);
	std::cout << C;
}


// �÷��̾ ���� ī���� ����(���� �п� ��������)

	void CPlayerPae::Reset() { CCardSet::Reset(); OldScore = 6; nGo = 0; bShake = false; }
	int CPlayerPae::GetGo() { return nGo; }				//�� Ƚ��
	void CPlayerPae::IncreaseGo() { nGo++; }			//�� ����

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
	std::cout << "����:" << CalcScore() << "��, " << nGo << "�� " << (bShake ? "���" : "");
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
			*p++ = RemoveCard(FindFirstCard("��"));
			return 1;
		}
		else {
			*p++ = RemoveCard(FindFirstCard("��"));
			return 1;
		}
		break;
	case 2:
		if (ns != 0) {
			*p = RemoveCard(FindFirstCard("��"));
			return 1;
		}
		else {
			*p++ = RemoveCard(FindFirstCard("��"));
			num = 1;
			if (np >= 2) {
				*p++ = RemoveCard(FindFirstCard("��"));
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
	if (n[0] == 5) NewScore += 5;                          // 5�� 15��
	if (n[0] == 3 || n[0] == 4 && FindFirstCard("B��") != -1) NewScore--;			//�������̸� -1��(3,4��)
	if (n[1] >= 5) NewScore += (n[1] - 4);			//��, �� 5�� ������ 1��
	if (n[2] >= 5) NewScore += (n[2] - 4);
	if (n[3] >= 10) NewScore += (n[3] - 9);			//�� 10�� ������ 1��
	if (FindFirstCard("8��") != -1 && FindFirstCard("5��") != -1 && FindFirstCard("2��") != -1) NewScore += 5;	//����
	if (FindFirstCard("1��") != -1 && FindFirstCard("2��") != -1 && FindFirstCard("3��") != -1) NewScore += 3;	//ȫ��
	if (FindFirstCard("4��") != -1 && FindFirstCard("5��") != -1 && FindFirstCard("7��") != -1) NewScore += 3;	//�ʴ�
	if (FindFirstCard("9��") != -1 && FindFirstCard("���") != -1 && FindFirstCard("6��") != -1) NewScore += 3;	//û��
	return NewScore;
}