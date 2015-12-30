#include "stdafx.h"
#ifndef MyProviderInformation_H
#define MyProviderInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyProviderInformation
{
	public:
		MyProviderInformation(const wchar_t* Provider_ID1,const wchar_t* Provider_Name1);
		myProviderInformation toLogStruct(int summarizationLevel);
		~MyProviderInformation(void);

		void print();

		const wchar_t* Provider_ID;
		const wchar_t* Provider_Name;
};
#endif