#include "stdafx.h"
#ifndef MyCalloutInformation_H
#define MyCalloutInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;


class MyCalloutInformation
{
	public:
		MyCalloutInformation(const wchar_t* Callout_ID1,const wchar_t* Callout_Name1);
		myCalloutInformation toLogStruct(int summarizationLevel);
		~MyCalloutInformation(void);

		void print();

		const wchar_t* Callout_ID;
		const wchar_t* Callout_Name;
};
#endif