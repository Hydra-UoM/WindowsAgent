#include "stdafx.h"
#ifndef MyNewLogon_H
#define MyNewLogon_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;


class MyNewLogon
{
public:
	MyNewLogon(const wchar_t* Security_ID1, const wchar_t* Account_Name1, const wchar_t* Account_Domain1, const wchar_t* Logon_ID1, const wchar_t* Logon_GUID1);
	list<string>toLogString(list<string>prepart, int summarizationLevel);
	~MyNewLogon(void);

	void print();

	const wchar_t* Security_ID;
	const wchar_t* Account_Name;
	const wchar_t* Account_Domain;
	const wchar_t* Logon_ID;
	const wchar_t* Logon_GUID;
};
#endif