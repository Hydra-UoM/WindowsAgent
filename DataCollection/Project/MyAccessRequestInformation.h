#include "stdafx.h"
#ifndef MyAccessRequestInformation_H
#define MyAccessRequestInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyAccessRequestInformation
{
	public:
		MyAccessRequestInformation(const wchar_t* Transaction_ID1,const wchar_t* Accesses1,const wchar_t* Access_Reasons1,const wchar_t* Access_Mask1,
					const wchar_t* Privileges_Used_For_Access_Check1,const wchar_t* Restricted_SID_Count1);
		myAccessRequestInformation toLogStruct(int summarizationLevel);
		~MyAccessRequestInformation(void);

		void print();

		const wchar_t* Transaction_ID;
		const wchar_t* Accesses;
		const wchar_t* Access_Reasons;
		const wchar_t* Access_Mask;
		const wchar_t* Privileges_Used_For_Access_Check;
		const wchar_t* Restricted_SID_Count;
};
#endif