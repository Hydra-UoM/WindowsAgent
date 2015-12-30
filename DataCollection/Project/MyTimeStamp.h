#include "stdafx.h"
#ifndef MyTimeStamp_H
#define MyTimeStamp_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include "MyLogStruct.h"
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyTimeStamp
{
	public:
		MyTimeStamp(ULONGLONG ullTimeStamp1,SYSTEMTIME& st1,FILETIME& ft1);
		~MyTimeStamp(void);
		void ToPrintTimeStamp();
		myStruct::myTimeStamp toTimeStampStruct();

	private:
		SYSTEMTIME st;
		ULONGLONG ullTimeStamp;
		FILETIME ft;
		ULONGLONG ullNanoseconds;
		WORD month;
		WORD day;
		WORD year;
		WORD hour;
		WORD minute;
		WORD second;
};
#endif