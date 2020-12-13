// Hwa2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "CCard.h"
#include "CCardSet.h"
#include "Game.h"
const int MaxCard = 48;					//패의 총 갯수
const int CardGap = 4;					//패사이의 간격
const int Speed = 10000;				//패 속도
const int PromptSpeed = 20000;			//메시지 출력 속도
// 화투의 초기 카드 목록
CCard HwaToo[MaxCard] = {
	"1광","1오","1피","1피","2십","2오","2피","2피","3광","3오","3피","3피",
	"4십","4오","4피","4피","5십","5오","5피","5피","6십","6오","6피","6피",
	"7십","7오","7피","7피","8광","8십","8피","8피","9십","9오","9피","9피",
	"J십","J오","J피","J피","D광","D쌍","D피","D피","B광","B십","B오","B쌍"
};

// 함수 원형
void Initialize();
void DrawScreen();
void OutPrompt(const char* Mes, int Wait = 0);
int InputInt(const char* Mes, int start, int end);

// 전역 변수
CDeck Deck(18, 9);						//위치만 인자로 받음
CPlayer South(5, 20), North(5, 1);
CBlanket Blanket(5, 12);
CPlayerPae SouthPae(40, 14), NorthPae(40, 4);
bool SouthTurn;

// 프로그램을 총지휘하는 main 함수

void main() {
	int South_cnt = 0;
	int North_cnt = 0;
	int i, ch;
	int arSame[4], SameNum;
	char Mes[256];
	CPlayer* Turn;
	CPlayerPae* TurnPae, * OtherPae;
	int UserIdx, UserSel, DeckSel; //사용자가낸 카드번호, 카드자체, 바닥패
	CCard UserCard, DeckCard;
	bool UserTriple, DeckTriple; //세장 한꺼번에 먹기
	int nSnatch; //뺏어올 피의 갯수
	int NewScore;
	CCard arPee[3];
	int nPee;
	CPlayer* LastGo = NULL;
	Game G;
	srand((unsigned)time(NULL));

	Initialize();
	DrawScreen();
	if (South.GetMaxSeries() == 3) {
		ch = InputInt("같은 패가 세 장입니다. (1:흔들기, 2:그냥 하기) ", 1, 2);
		if (ch == 1) SouthPae.bShake = true;
	}
	if (North.GetMaxSeries() == 3) {
		ch = InputInt("같은 패가 세 장입니다. (1:흔들기, 2:그냥 하기) ", 1, 2);
		if (ch == 1) NorthPae.bShake = true;
	}
	for (SouthTurn = true; !Deck.IsEmpty(); SouthTurn = !SouthTurn) {
		DrawScreen();
		if (SouthTurn) {
			Turn = &South;
			TurnPae = &SouthPae;
			OtherPae = &NorthPae;
		}
		else {
			Turn = &North;
			TurnPae = &NorthPae;
			OtherPae = &SouthPae;
		}

		sprintf_s(Mes, "내고 싶은 화투를 선택하세요(1~%d, 0:종료) ", Turn->GetNum());
		ch = InputInt(Mes, 0, Turn->GetNum());
		if (ch == 0) {
			if (InputInt("정말 끝낼겁니까?(0:예, 1:아니오)", 0, 1) == 0)
				return;
			else
				continue;
		}
		// 플레이어가 패를 한장 낸다.
		UserTriple = DeckTriple = false;
		UserIdx = ch - 1;
		UserCard = Turn->GetCard(UserIdx);
		SameNum = Blanket.FindSameCard(UserCard, arSame);
		switch (SameNum) {
		case 0:
			UserSel = -1;
			Blanket.InsertCard(Turn->RemoveCard(UserIdx));
			DrawScreen();
			break;
		case 1:
			UserSel = arSame[0];
			break;
		case 2:
			if (Blanket.GetCard(arSame[0]) == Blanket.GetCard(arSame[1])) {
				UserSel = arSame[0];
			}
			else {
				Blanket.DrawSelNum(arSame);
				sprintf_s(Mes, "어떤 패를 선택하시겠습니까?(1~%d)", SameNum);
				UserSel = arSame[InputInt(Mes, 1, SameNum) - 1];
			}
			break;
		case 3:
			UserSel = arSame[1];
			UserTriple = true;
			break;
		}
		if (UserSel != -1) {
			Blanket.DrawTempCard(UserSel, UserCard);
		}
		G.delay(Speed);

		// 데크에서 한장을 뒤집는다.
		Deck.Draw(true);
		G.delay(Speed);
		DeckCard = Deck.Pop();
		SameNum = Blanket.FindSameCard(DeckCard, arSame);
		switch (SameNum) {
		case 0:
			DeckSel = -1;
			break;
		case 1:
			DeckSel = arSame[0];
			if (SouthTurn) {
				if (DeckSel == UserSel) {
					if (Deck.IsNotLast()) {
						Blanket.InsertCard(DeckCard);
						Blanket.InsertCard(Turn->RemoveCard(UserIdx));
						OutPrompt("설사했습니다.", PromptSpeed);
						South_cnt++;
						if (South_cnt == 3) {
							OutPrompt("1P가 3번 설사했습니다.", PromptSpeed);
							exit(0);
						}
						continue;
					}
					else {
						DeckSel = -1;
					}
				}
			}
			else {
				if (DeckSel == UserSel) {
					if (Deck.IsNotLast()) {
						Blanket.InsertCard(DeckCard);
						Blanket.InsertCard(Turn->RemoveCard(UserIdx));
						OutPrompt("설사했습니다.", PromptSpeed);
						North_cnt++;
						if (North_cnt == 3) {
							OutPrompt("2P가 3번 설사했습니다.", PromptSpeed);
							exit(0);
						}
						continue;
					}
					else {
						DeckSel = -1;
					}
				}
			}
			break;
		case 2:
			if (UserSel == arSame[0]) {
				DeckSel = arSame[1];
			}
			else if (UserSel == arSame[1]) {
				DeckSel = arSame[0];
			}
			else {
				if (Blanket.GetCard(arSame[0]) == Blanket.GetCard(arSame[1])) {
					DeckSel = arSame[0];
				}
				else {
					Blanket.DrawSelNum(arSame);
					sprintf_s(Mes, "어떤 패를 선택하시겠습니까?(1~%d)", SameNum);
					DeckSel = arSame[InputInt(Mes, 1, SameNum) - 1];
				}
			}
			break;
		case 3:
			DeckSel = arSame[1];
			DeckTriple = true;
			break;
		}
		if (DeckSel != -1) {
			Blanket.DrawTempCard(DeckSel, DeckCard);
		}
		Deck.Draw(false);
		G.delay(Speed);
		// 일치하는 카드를 거둬 들인다. 세 장을 먹은 경우는 전부 가져 온다.
		if (UserSel != -1) {
			if (UserTriple) {
				for (i = 0; i < 3; i++) {
					TurnPae->InsertCard(Blanket.RemoveCard(UserSel - 1));
				}
			}
			else {
				TurnPae->InsertCard(Blanket.RemoveCard(UserSel));
			}
			TurnPae->InsertCard(Turn->RemoveCard(UserIdx));
			if (DeckSel != -1 && DeckSel > UserSel) {
				DeckSel -= (UserTriple ? 3 : 1);
			}
		}
		if (DeckSel != -1) {
			if (DeckTriple) {
				for (i = 0; i < 3; i++) {
					TurnPae->InsertCard(Blanket.RemoveCard(DeckSel - 1));
				}
			}
			else {
				TurnPae->InsertCard(Blanket.RemoveCard(DeckSel));
			}
			TurnPae->InsertCard(DeckCard);
		}
		else {
			Blanket.InsertCard(DeckCard);
		}

		// 쪽, 따닥, 싹쓸이 조건을 점검하고 상대방의 피를 뺏는다.
		nSnatch = 0;
		if (Deck.IsNotLast()) {
			if (UserSel == -1 && SameNum == 1 && DeckCard.GetNumber() == UserCard.GetNumber()) {
				nSnatch++;
				OutPrompt("쪽입니다.", PromptSpeed);
			}
			if (UserSel != -1 && SameNum == 2 && DeckCard.GetNumber() == UserCard.GetNumber()) {
				nSnatch++;
				OutPrompt("따닥입니다.", PromptSpeed);
			}
			if (Blanket.GetNum() == 0) {
				nSnatch++;
				OutPrompt("싹쓸이입니다.", PromptSpeed);
			}
			if (UserTriple || DeckTriple) {
				OutPrompt("한꺼번에 세 장을 먹었습니다.", PromptSpeed);
				nSnatch += UserTriple + DeckTriple;
			}
		}
		nPee = OtherPae->RemovePee(nSnatch, arPee);
		for (i = 0; i < nPee; i++) {
			TurnPae->InsertCard(arPee[i]);
		}
		// 점수를 계산하고 고, 스톱 여부를 질문한다.
		NewScore = TurnPae->CalcScore();
		if (Deck.IsNotLast() && NewScore > TurnPae->OldScore) {
			DrawScreen();
			if (InputInt("추가 점수를 획득했습니다.(0:스톱, 1:계속)", 0, 1) == 1) {
				TurnPae->OldScore = NewScore;
				TurnPae->IncreaseGo();
				LastGo = Turn;
			}
			else {
				break;
			}
		}
	}
	DrawScreen();

	// 승부와 피박, 광박, 독박 여부를 판정한다.
	bool SouthWin;
	int SouthScore, NorthScore;
	int TurnPee = 0, TurnLight = 0, OtherPee = 0, OtherLight = 0;

	if (Deck.IsEmpty()) {
		if (LastGo != NULL)
			SouthWin = (LastGo == &North);
		else {
			SouthScore = SouthPae.CalcScore();
			NorthScore = NorthPae.CalcScore();
			if (SouthScore < 7 && NorthScore < 7) {
				OutPrompt("양쪽 모두 기본 점수를 얻지 못해 비겼습니다.");
				return;
			}
			SouthWin = (SouthScore > NorthScore);
		}
	}
	else
		SouthWin = SouthTurn;
	sprintf_s(Mes, "%s가 이겼습니다. ", SouthWin ? "1P" : "2P");

	if (SouthWin) {
		TurnPae = &SouthPae;
		OtherPae = &NorthPae;
	}
	else {
		TurnPae = &NorthPae;
		OtherPae = &SouthPae;
	}
	for (i = 0; i < TurnPae->GetNum(); i++) {
		if (TurnPae->GetCard(i).GetKind() >= 3) TurnPee++;
		if (TurnPae->GetCard(i).GetKind() == 0) TurnLight++;
	}
	for (i = 0; i < OtherPae->GetNum(); i++) {
		if (OtherPae->GetCard(i).GetKind() >= 3) OtherPee++;
		if (OtherPae->GetCard(i).GetKind() == 0) OtherLight++;
	}
	if (TurnPee >= 10 && OtherPee <= 6 && OtherPee != 0)
		strcat_s(Mes, "진쪽이 피박입니다. ");
	if (TurnLight >= 3 && OtherLight == 0)
		strcat_s(Mes, "진쪽이 광박입니다. ");
	if (OtherPae->GetGo() != 0)
		strcat_s(Mes, "진쪽이 독박입니다. ");
	OutPrompt(Mes);
}
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
void Initialize() {//게임을 초기화, 데크의 패를 무작위로 섞은 후 플레이어와 담요로 패를 분배
	int i;

	for (;;) {
		Deck.Reset();
		South.Reset();
		North.Reset();
		Blanket.Reset();
		Deck.Shuffle();
		for (i = 0; i < 10; i++) {
			//플레이어 10장의 패
			South.InsertCard(Deck.Pop());
			North.InsertCard(Deck.Pop());
			if (i < 8) Blanket.InsertCard(Deck.Pop());
			//담요 8장의 패
		}
		if (South.GetMaxSeries() != 4 && North.GetMaxSeries() != 4 &&
			Blanket.GetMaxSeries() != 4)
			break;
	}
}

void DrawScreen() {

	//게임화면 구성
	system("cls");
	South.Draw(SouthTurn);
	North.Draw(!SouthTurn);
	Blanket.Draw();
	Deck.Draw(false);
	SouthPae.Draw();
	NorthPae.Draw();
}

void OutPrompt(const char* Mes, int Wait) {
	//게임 안내 메시지 출력
	Game G;

	G.gotoxy(5, 23);
	for (int i = 5; i < 79; i++) { std::cout << ' '; }
	G.gotoxy(5, 23);
	std::cout << Mes;
	G.delay(Wait);
}

int InputInt(const char* Mes, int start, int end) {
	//요구사항 입력받음
	int ch;

	OutPrompt(Mes);
	for (;;) {
		ch = tolower(_getch());
		if (ch == 0xE0 || ch == 0) {
			ch = _getch();
			continue;
		}
		if (!(isdigit(ch) || ch == 'a')) continue;
		if (ch == 'a') ch = 10; else ch = ch - '0';
		if (ch >= start && ch <= end) {
			return ch;
		}
		OutPrompt("무효한 번호입니다. 지정한 범위에 맞게 다시 입력해 주세요.");
	}
}