#include "stdafx.h"
#ifndef MyDetailedAuthenticationInformation_H
#define MyDetailedAuthenticationInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyDetailedAuthenticationInformation
{
public:
	MyDetailedAuthenticationInformation(const wchar_t* Logon_Process1, const wchar_t* Authentication_Package1, const wchar_t* Transited_Services1, const wchar_t* Package_Name1, const wchar_t* Key_Length1);
	myDetailedAuthenticationInformation toLogStruct(int summarizationLevel);
	~MyDetailedAuthenticationInformation(void);

	void print();

	const wchar_t* Logon_Process;
	const wchar_t* Authentication_Package;
	const wchar_t* Transited_Services;
	const wchar_t* Package_Name;
	const wchar_t* Key_Length;
};
#endif