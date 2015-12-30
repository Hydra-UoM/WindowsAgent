#include "stdafx.h"
#ifndef MyLayerInformation_H
#define MyLayerInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyLayerInformation
{
	public:
		MyLayerInformation(const wchar_t* Layer_ID1,const wchar_t* Layer_Name1,const wchar_t* Layer_Runtime_ID1);
		myLayerInformation toLogStruct(int summarizationLevel);
		~MyLayerInformation(void);

		void print();

		const wchar_t* Layer_ID;
		const wchar_t* Layer_Name;
		const wchar_t* Layer_Runtime_ID;
};
#endif