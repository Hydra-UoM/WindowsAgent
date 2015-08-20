#include "stdafx.h"
#ifndef MyErrorInformation_H
#define MyErrorInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyErrorInformation
{
	public:
		MyErrorInformation(const wchar_t* Reason1);
		~MyErrorInformation(void);

		void print();

		const wchar_t* Reason;
};
#endif