#include "stdafx.h"
#ifndef MyChangeInformation_H
#define MyChangeInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyChangeInformation
{
	public:
		MyChangeInformation(const wchar_t* Change_Type1);
		~MyChangeInformation(void);

		void print();

		const wchar_t* Change_Type;
};
#endif