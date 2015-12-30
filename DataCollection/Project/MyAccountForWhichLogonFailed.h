#include "stdafx.h"
#ifndef MyAccountForWhichLogonFailed_H
#define MyAccountForWhichLogonFailed_H


#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>

#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyAccountForWhichLogonFailed
{
public:
	MyAccountForWhichLogonFailed(const wchar_t* Security_ID1, const wchar_t* Account_Name1, const wchar_t* Account_Domain1);
	myAccountForWhichLogonFailed toLogStruct(int summarizationLevel);
	~MyAccountForWhichLogonFailed(void);

	void print();

	const wchar_t* Security_ID;
	const wchar_t* Account_Name;
	const wchar_t* Account_Domain;
};
#endif