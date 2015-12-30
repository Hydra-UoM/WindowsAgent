#include "stdafx.h"
#ifndef MySubject_H
#define MySubject_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;
using namespace std;

class MySubject
{
	public:
		MySubject(const wchar_t* Security_ID1, const wchar_t* Account_Name1, const wchar_t* Account_Domain1, const wchar_t* Logon_ID1);
		mySubject toLogStruct(int summarizationLevel);

		~MySubject(void);

		void print();

		const wchar_t* Security_ID;
		const wchar_t* Account_Name;
		const wchar_t* Account_Domain;
		const wchar_t* Logon_ID;
};
#endif