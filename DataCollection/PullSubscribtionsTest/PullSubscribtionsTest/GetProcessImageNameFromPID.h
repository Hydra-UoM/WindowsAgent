#include "stdafx.h"
#ifndef GetProcessImageNameFromPID_H
#define GetProcessImageNameFromPID_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <sddl.h>

#pragma comment(lib, "wevtapi.lib")

class GetProcessImageNameFromPID
{
	public:
		GetProcessImageNameFromPID();
		~GetProcessImageNameFromPID(void);

		static BOOL getProcessNameFromProcessID(DWORD processId,WCHAR**processName);
		static void printError( TCHAR* msg );
};
#endif