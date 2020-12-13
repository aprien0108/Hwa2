#pragma once
#include <iostream>
// 화투 한장을 표현하는 클래스
class CCard {
public:
	char Name[4];
	CCard();
	CCard(const char* pName);
	int GetNumber() const;
	int GetKind() const;
	friend std::ostream& operator <<(std::ostream& c, const CCard& C) {	//카드를 화면에 출력
		return c << C.Name;
	}
	bool operator ==(const CCard& Other) const {			//비교 연산자 오버로딩
		return(strcmp(Name, Other.Name) == 0);
	}
	bool operator <(const CCard& Other) const {	//화투 패 대소 비교(작은패가 앞으로)
		if (GetNumber() < Other.GetNumber()) return true;
		if (GetNumber() > Other.GetNumber()) return false;
		if (GetKind() < Other.GetKind()) return true;
		return false;
	}
};

