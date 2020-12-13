#pragma once
#include "CCard.h"
#include <iostream>
// 화투 한장을 표현하는 클래스

	CCard::CCard() { CCard::Name[0] = 0; }
	CCard::CCard(const char* pName) {
		strcpy_s(Name, pName);
	}
	int CCard::GetNumber() const {				//패의 숫자
		if (isdigit(CCard::Name[0])) return CCard::Name[0] - '0';
		if (CCard::Name[0] == 'J') return 10;
		if (CCard::Name[0] == 'D') return 11;
		return 12;
	}
	int CCard::GetKind() const {					//패의 종료
		if (strcmp(CCard::Name + 1, "광") == 0) return 0;
		else if (strcmp(CCard::Name + 1, "십") == 0) return 1;
		else if (strcmp(CCard::Name + 1, "오") == 0) return 2;
		else if (strcmp(CCard::Name + 1, "쌍") == 0) return 3;
		else return 4;
	}
	

