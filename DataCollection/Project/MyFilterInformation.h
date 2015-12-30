#include "stdafx.h"
#ifndef MyFilterInformation_H
#define MyFilterInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyFilterInformation
{
	public:
		MyFilterInformation(const wchar_t* Filter_ID1,const wchar_t* Filter_Name1,const wchar_t* Filter_Type1,
			const wchar_t* Runtime_ID1, const wchar_t* Filter_Runtime_ID1,
			const wchar_t* Filter_Layer_Name1,const wchar_t* Filter_Layer_Runtime_ID1);
		myFilterInformation toLogStruct(int summarizationLevel);
		~MyFilterInformation(void);

		void print();

		const wchar_t* Filter_ID;
		const wchar_t* Filter_Name;
		const wchar_t* Filter_Type;
		const wchar_t* Runtime_ID;
		const wchar_t* Filter_Runtime_ID;
		const wchar_t* Filter_Layer_Name;
		const wchar_t* Filter_Layer_Runtime_ID;
};
#endif