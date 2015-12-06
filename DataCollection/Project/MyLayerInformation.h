#include "stdafx.h"
#ifndef MyLayerInformation_H
#define MyLayerInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyLayerInformation
{
	public:
		MyLayerInformation(const wchar_t* Layer_ID1,const wchar_t* Layer_Name1,const wchar_t* Layer_Runtime_ID1);
		~MyLayerInformation(void);

		void print();

		const wchar_t* Layer_ID;
		const wchar_t* Layer_Name;
		const wchar_t* Layer_Runtime_ID;
};
#endif