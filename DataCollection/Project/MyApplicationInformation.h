#include "stdafx.h"
#ifndef MyApplicationInformation_H
#define MyApplicationInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyApplicationInformation
{
	public:
		MyApplicationInformation(const wchar_t* Application_Process_ID1,const wchar_t* Application_Name1);
		~MyApplicationInformation(void);

		void print();

		const wchar_t* Application_Process_ID;
		const wchar_t* Application_Name;
};
#endif