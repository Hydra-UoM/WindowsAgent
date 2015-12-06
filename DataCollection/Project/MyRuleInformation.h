#include "stdafx.h"
#ifndef MyRuleInformation_H
#define MyRuleInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyRuleInformation
{
	public:
		MyRuleInformation(const wchar_t* ID1,const wchar_t* Name1);
		~MyRuleInformation(void);

		void print();

		const wchar_t* ID;
		const wchar_t* Name;
};
#endif