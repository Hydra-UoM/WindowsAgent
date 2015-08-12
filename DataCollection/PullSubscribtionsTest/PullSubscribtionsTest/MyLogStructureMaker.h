#include "stdafx.h"
#ifndef MyLogStructureMaker_H
#define MyLogStructureMaker_H

#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include "MyLogStructure.h"
#include "GetProcessImageNameFromPID.h"

#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyLogStructureMaker
{
	public:
		MyLogStructureMaker(EVT_HANDLE& hEvent1);
		~MyLogStructureMaker(void);

		DWORD getStatus(MyLogStructure*outputLogStructure);
		BOOL IsKeyEvent(HANDLE hStdIn);
		LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);

	private:
		EVT_HANDLE hEvent;
		DWORD status;

		LPWSTR eventMessageString;
		LPWSTR levelMessageString;
		LPWSTR taskMessageString;
		LPWSTR opCodeMessageString;
		LPWSTR channelMessageString;
		LPWSTR providerMessageString;

		int version;
		int level;
		int task;
		int opCode;
		UINT64 keywords;
		ULONGLONG ullTimeStamp;
		UINT64 eventRecordID;
		UINT32 executionProcessID;
		UINT32 executionThreadID;
		const wchar_t* channel;
		LPCWSTR computer;
		DWORD EventID;
		SYSTEMTIME st;

		FILETIME ft;
		ULONGLONG ullNanoseconds;
};
#endif