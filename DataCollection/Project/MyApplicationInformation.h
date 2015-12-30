#include "stdafx.h"
#ifndef MyApplicationInformation_H
#define MyApplicationInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyApplicationInformation
{
	public:
		MyApplicationInformation(const wchar_t* Application_Process_ID1,const wchar_t* Application_Name1);
		myApplicationInformation toLogStruct(int summarizationLevel);
		~MyApplicationInformation(void);

		void print();

		const wchar_t* Application_Process_ID;
		const wchar_t* Application_Name;
};
#endif