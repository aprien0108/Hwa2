#pragma once
#include "CCard.h"
#include <iostream>
// ȭ�� ������ ǥ���ϴ� Ŭ����

	CCard::CCard() { CCard::Name[0] = 0; }
	CCard::CCard(const char* pName) {
		strcpy_s(Name, pName);
	}
	int CCard::GetNumber() const {				//���� ����
		if (isdigit(CCard::Name[0])) return CCard::Name[0] - '0';
		if (CCard::Name[0] == 'J') return 10;
		if (CCard::Name[0] == 'D') return 11;
		return 12;
	}
	int CCard::GetKind() const {					//���� ����
		if (strcmp(CCard::Name + 1, "��") == 0) return 0;
		else if (strcmp(CCard::Name + 1, "��") == 0) return 1;
		else if (strcmp(CCard::Name + 1, "��") == 0) return 2;
		else if (strcmp(CCard::Name + 1, "��") == 0) return 3;
		else return 4;
	}
	

