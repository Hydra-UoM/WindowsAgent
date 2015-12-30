#include "stdafx.h"
#ifndef MyFailureInformation_H
#define MyFailureInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyFailureInformation
{
public:
	MyFailureInformation(const wchar_t* Failure_Reason1, const wchar_t* Status1, const wchar_t* Sub_Status1);
	myFailureInformation toLogStruct(int summarizationLevel);
	~MyFailureInformation(void);

	void print();

	const wchar_t* Failure_Reason;
	const wchar_t* Status;
	const wchar_t* Sub_Status;		
};
#endif