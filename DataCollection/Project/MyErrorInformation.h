#include "stdafx.h"
#ifndef MyErrorInformation_H
#define MyErrorInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyErrorInformation
{
	public:
		MyErrorInformation(const wchar_t* Reason1);
		myErrorInformation toLogStruct(int summarizationLevel);
		~MyErrorInformation(void);

		void print();

		const wchar_t* Reason;
};
#endif