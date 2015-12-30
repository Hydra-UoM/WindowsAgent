#include "stdafx.h"
#ifndef MyAdditionalInformation_H
#define MyAdditionalInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyAdditionalInformation
{
	public:
		MyAdditionalInformation(const wchar_t* Weight1,const wchar_t* Conditions1,
								const wchar_t* Condition_ID11,const wchar_t* Match_Value11,const wchar_t* Condition_Value11,
								const wchar_t* Condition_ID21,const wchar_t* Match_Value21,const wchar_t* Condition_Value21,
								const wchar_t* Condition_ID31,const wchar_t* Match_Value31,const wchar_t* Condition_Value31,
								const wchar_t* Filter_Action1);
		//myAdditionalInformation toLogStruct(int summarizationLevel);
		~MyAdditionalInformation(void);

		void print();

		const wchar_t* Weight;
		const wchar_t* Conditions;
		const wchar_t* Condition_ID1;
		const wchar_t* Match_Value1;
		const wchar_t* Condition_Value1;
		const wchar_t* Condition_ID2;
		const wchar_t* Match_Value2;
		const wchar_t* Condition_Value2;
		const wchar_t* Condition_ID3;
		const wchar_t* Match_Value3;
		const wchar_t* Condition_Value3;
		const wchar_t* Filter_Action;
};
#endif