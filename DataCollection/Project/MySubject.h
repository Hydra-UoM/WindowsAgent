#include "stdafx.h"
#ifndef MySubject_H
#define MySubject_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MySubject
{
	public:
		MySubject();
		void setElements(const wchar_t* Security_ID1,const wchar_t* Account_Name1,const wchar_t* Account_Domain1,const wchar_t* Logon_ID1);
		~MySubject(void);

		void print();

		const wchar_t* Security_ID;
		const wchar_t* Account_Name;
		const wchar_t* Account_Domain;
		const wchar_t* Logon_ID;
};
#endif