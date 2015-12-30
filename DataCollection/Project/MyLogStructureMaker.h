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
#include <string>
#include <atlstr.h>
#include<memory> // unique_ptr
#include <sddl.h>

using namespace std;
#pragma comment(lib, "wevtapi.lib")

class MyLogStructureMaker
{
	public:
		MyLogStructureMaker(EVT_HANDLE& hEvent1);
		~MyLogStructureMaker(void);

		DWORD getStatus(MyLogStructure*outputLogStructure);
		BOOL IsKeyEvent(HANDLE hStdIn);
		LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);
		void extractEventMessageString(std::unique_ptr<MyLogStructure> & outputLog);
		std::vector<wstring> splitLPWSTRWithManyDelimiters(const wstring &original, const wstring &delimiters);
		//bool splitString(string str_eventMessageString, char delimiters[], int *result, string*splitted);
		//int findOneOfDelimitersFirstPosition(string remain, char delimiters[]);

		LPWSTR eventMessageString;
		LPWSTR levelMessageString;
		LPWSTR taskMessageString;
		LPWSTR opCodeMessageString;
		LPWSTR channelMessageString;
		LPWSTR providerMessageString;
		//wstring arrayOfSplitted[200];

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
	private:
		EVT_HANDLE hEvent;
		DWORD status;
};
#endif