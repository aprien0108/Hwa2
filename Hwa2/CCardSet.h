#pragma once
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
extern const int Speed;				//�� �ӵ�
extern const int PromptSpeed;			//�޽��� ��� �ӵ�
extern CCard HwaToo[48];
// ���� ������ �����ϴ� Ŭ����
class CCardSet {
protected:
	CCard Card[48];
	int Num;						//�� ������ ����
	const int sx, sy;					//ȭ����� ��ǥ��
	CCardSet(int asx, int asy) : sx(asx), sy(asy) { Num = 0; }

public:
	int GetNum();
	CCard GetCard(int idx);
	void Reset();
	void InsertCard(CCard C);				//�� ���ĵ� ��ġ�� ����
	CCard RemoveCard(int idx); //������ ÷���� �и� �о �����ϰ� �ش� �и� ���տ��� ����
	//�μ��� ���޵� �п� ���ڰ� ��ġ�ϴ� ���� ���� �� ÷�� ����� ����
	int FindSameCard(CCard C, int* pSame);
	//�κ� ���ڿ� �˻��� ���� ���ڳ� ������ ��ġ�ϴ� ������ �и� �˻�
	int FindFirstCard(const char* pName);
	int GetMaxSeries();
};

// ��� �߾ӿ� ī�带 �׾� ���� ��ũ(��� ǥ��)
class CDeck : public CCardSet {
public:
	Game G;
	CDeck(int asx, int asy) : CCardSet(asx, asy) { }
	void Shuffle();
	CCard Pop();		//���ʴ�� ī�带 ����
	bool IsEmpty();		//��ũ�� �����ִ� �� ���� ����
	bool IsNotLast();			//������ ������ �˻�
	void Draw(bool bFlip);					//'��'�� ��ũ�� ������ �� ǥ��
};

// ������ �ϴ� �÷��̾�
class CPlayer : public CCardSet {
public:
	Game G;
	CPlayer(int asx, int asy) : CCardSet(asx, asy) { }
	void Draw(bool MyTurn);
};

// ������ ����Ǵ� ���
class CBlanket : public CPlayer {
public:
	Game G;
	CBlanket(int asx, int asy) : CPlayer(asx, asy) { }
	void Draw();
	void DrawSelNum(int* pSame);			//����ī�� ������ ��ȣ ���
	void DrawTempCard(int idx, CCard C);			//�ӽ������� ��信 ���δ� ��
};

// �÷��̾ ���� ī���� ����(���� �п� ��������)
class CPlayerPae : public CCardSet {
private:
	int nGo;

public:
	Game G;
	int OldScore;			//���� ����
	int bShake;				//���
	CPlayerPae(int asx, int asy) : CCardSet(asx, asy) { OldScore = 6; nGo = 0; bShake = false; }
	void Reset();
	int GetGo();			//�� Ƚ��
	void IncreaseGo();				//�� ����
	void Draw();						//�����и� ȭ�鿡 �����Ͽ� ���
	int RemovePee(int n, CCard* pCard);			//�� ���� ����(��, ����, 3��Ա�)
	int CalcScore();						//�������
};
