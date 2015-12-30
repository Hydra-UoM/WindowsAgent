#include "stdafx.h"
#ifndef MyObject_H
#define MyObject_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;

using namespace std;

class MyObject
{
	public:
		MyObject(const wchar_t* Object_Server1,const wchar_t* Object_Type1,const wchar_t* Object_Name1,const wchar_t* Handle_ID1,
			const wchar_t* Resource_Attributes1);
		myObject toLogStruct(int summarizationLevel);
		~MyObject(void);

		void print();

		const wchar_t* Object_Server;
		const wchar_t* Object_Type;
		const wchar_t* Object_Name;
		const wchar_t* Handle_ID;
		const wchar_t* Resource_Attributes;
};
#endif