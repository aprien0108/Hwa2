#pragma once
#include <iostream>
// ȭ�� ������ ǥ���ϴ� Ŭ����
class CCard {
public:
	char Name[4];
	CCard();
	CCard(const char* pName);
	int GetNumber() const;
	int GetKind() const;
	friend std::ostream& operator <<(std::ostream& c, const CCard& C) {	//ī�带 ȭ�鿡 ���
		return c << C.Name;
	}
	bool operator ==(const CCard& Other) const {			//�� ������ �����ε�
		return(strcmp(Name, Other.Name) == 0);
	}
	bool operator <(const CCard& Other) const {	//ȭ�� �� ��� ��(�����а� ������)
		if (GetNumber() < Other.GetNumber()) return true;
		if (GetNumber() > Other.GetNumber()) return false;
		if (GetKind() < Other.GetKind()) return true;
		return false;
	}
};

