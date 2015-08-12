#include "stdafx.h"
#ifndef MyObject_H
#define MyObject_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyObject
{
	public:
		MyObject(const wchar_t* Object_Server1,const wchar_t* Object_Type1,const wchar_t* Object_Name1,const wchar_t* Handle_ID1,
			const wchar_t* Resource_Attributes1);
		~MyObject(void);

		void print();

		const wchar_t* Object_Server;
		const wchar_t* Object_Type;
		const wchar_t* Object_Name;
		const wchar_t* Handle_ID;
		const wchar_t* Resource_Attributes;
};
#endif