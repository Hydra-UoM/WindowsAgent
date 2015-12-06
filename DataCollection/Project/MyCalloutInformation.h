#include "stdafx.h"
#ifndef MyCalloutInformation_H
#define MyCalloutInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyCalloutInformation
{
	public:
		MyCalloutInformation(const wchar_t* Callout_ID1,const wchar_t* Callout_Name1);
		~MyCalloutInformation(void);

		void print();

		const wchar_t* Callout_ID;
		const wchar_t* Callout_Name;
};
#endif