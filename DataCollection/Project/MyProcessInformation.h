#include "stdafx.h"
#ifndef MyProcessInformation_H
#define MyProcessInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyProcessInformation
{
	public:
		MyProcessInformation(const wchar_t* Process_ID1,const wchar_t* Process_Name1);
		~MyProcessInformation(void);
		myProcessInformation toLogStruct(int summarizationLevel);

		void print();

		const wchar_t* Process_ID;
		const wchar_t* Process_Name;
};
#endif