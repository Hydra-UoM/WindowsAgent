#include "stdafx.h"
#ifndef MyFilterInformation_H
#define MyFilterInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyFilterInformation
{
	public:
		MyFilterInformation(const wchar_t* Filter_ID1,const wchar_t* Filter_Name1,const wchar_t* Filter_Type1,
			const wchar_t* Runtime_ID1, const wchar_t* Filter_Runtime_ID1,
			const wchar_t* Filter_Layer_Name1,const wchar_t* Filter_Layer_Runtime_ID1);
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