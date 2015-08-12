#include "stdafx.h"
#ifndef MyProcessInformation_H
#define MyProcessInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyProcessInformation
{
	public:
		MyProcessInformation(const wchar_t* Process_ID1,const wchar_t* Process_Name1);
		~MyProcessInformation(void);

		void print();

		const wchar_t* Process_ID;
		const wchar_t* Process_Name;
};
#endif