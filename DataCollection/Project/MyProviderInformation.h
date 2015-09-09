#include "stdafx.h"
#ifndef MyProviderInformation_H
#define MyProviderInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyProviderInformation
{
	public:
		MyProviderInformation(const wchar_t* Provider_ID1,const wchar_t* Provider_Name1);
		~MyProviderInformation(void);

		void print();

		const wchar_t* Provider_ID;
		const wchar_t* Provider_Name;
};
#endif