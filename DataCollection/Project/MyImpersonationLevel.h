#include "stdafx.h"
#ifndef MyImpersonationLevel_H
#define MyImpersonationLevel_H


#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;
using namespace std;


class MyImpersonationLevel
{
public:
	MyImpersonationLevel(const wchar_t* Impersonation_Level1);
	list<string>toLogString(list<string>prepart, int summarizationLevel);
	~MyImpersonationLevel(void);

	void print();

	const wchar_t* Impersonation_Level;
};
#endif