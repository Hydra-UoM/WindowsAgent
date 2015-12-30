#include "stdafx.h"
#ifndef MyChangeInformation_H
#define MyChangeInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyChangeInformation
{
	public:
		MyChangeInformation(const wchar_t* Change_Type1);
		myChangeInformation toLogStruct(int summarizationLevel);
		~MyChangeInformation(void);

		void print();

		const wchar_t* Change_Type;
};
#endif