#include "stdafx.h"
#ifndef MyLogonType_H
#define MyLogonType_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;
using namespace std;

class MyLogonType
{
public:
	MyLogonType(const wchar_t* Logon_Type1);
	myLogonType toLogStruct(int summarizationLevel);
	~MyLogonType(void);

	void print();
	
	const wchar_t* Logon_Type;
};
#endif