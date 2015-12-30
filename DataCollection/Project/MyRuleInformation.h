#include "stdafx.h"
#ifndef MyRuleInformation_H
#define MyRuleInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyRuleInformation
{
	public:
		MyRuleInformation(const wchar_t* ID1,const wchar_t* Name1);
		myRuleInformation toLogStruct(int summarizationLevel);
		~MyRuleInformation(void);

		void print();

		const wchar_t* ID;
		const wchar_t* Name;
};
#endif