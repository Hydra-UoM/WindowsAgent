#include "stdafx.h"
#ifndef MyLogStructure_H
#define MyLogStructure_H

#include <codecvt>
#include <cstdio>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <windows.h>
#include <stdio.h>
#include "MyTimeStamp.h"
#include "MySubject.h"
#include "MyObject.h"
#include <atlstr.h> // CW2A
#include <winevt.h>
#include "MyProcessInformation.h"
#include "MyApplicationInformation.h"
#include "MyNetworkInformation.h"
#include "MyAccessRequestInformation.h"
#include "MyProviderInformation.h"
#include "MyChangeInformation.h"
#include "MyFilterInformation.h"
#include "MyLayerInformation.h"
#include "MyCalloutInformation.h"
#include "MyRuleInformation.h"
#include "MyErrorInformation.h"
#include "MyLogonType.h"
#include "MyImpersonationLevel.h"
#include "MyAccountForWhichLogonFailed.h"
#include "MyFailureInformation.h"
#include "MyNewLogon.h"
#include "MyDetailedAuthenticationInformation.h"
#include "MyAdditionalInformation.h"
#include "MyLogStruct.h"
#include <sddl.h>

#pragma comment(lib, "wevtapi.lib")

class MyLogStructure
{
public:
	MyLogStructure(/**LPWSTR eventMessageString1, */LPWSTR levelMessageString1, LPWSTR taskMessageString1, LPWSTR opCodeMessageString1,
		LPWSTR channelMessageString1, LPWSTR providerMessageString1, int version1, int level1, int task1, int opCode1, UINT64 keywords1,
		UINT64 eventRecordID1, UINT32 executionProcessID1, UINT32 executionThreadID1, const wchar_t* channel1,
		LPCWSTR computer1, DWORD EventID1, MyTimeStamp& timeStamp1/*MySubject*mySubject1*/);
	void releaseMemory();
	~MyLogStructure(void);

	void print();
	void printExtractedEventMessageString();
	//void extractEventMessageString();
	void initializeAvailableInformation();
	//std::vector<wstring> splitLPWSTRWithManyDelimiters(const wstring &original, const wstring &delimiters);
	string wchar_t_pointerToString(const wchar_t*text);
	myStruct::myLogStructure toStruct(int summarizationLevel, string processName1, string eventCategory1);

	//private:
	const wchar_t* message; // change it into wstring from const wchar_t* makes exception
	//LPWSTR eventMessageString;
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
	UINT64 eventRecordID;
	UINT32 executionProcessID;
	UINT32 executionThreadID;
	const wchar_t* channel;
	LPCWSTR computer;
	DWORD EventID;
	MyTimeStamp timeStamp;
	//WCHAR* processImageName;

	bool isAvailableMySubject;
	bool isAvailableMyProviderInformation;
	bool isAvailableMyProcessInformation;
	bool isAvailableMyObject;
	bool isAvailableMyNetworkInformation;
	bool isAvailableMyLayerInformation;
	bool isAvailableMyFilterInformation;
	bool isAvailableMyChangeInformation;
	bool isAvailableMyCalloutInformation;
	bool isAvailableMyApplicationInformation;
	bool isAvailableMyAdditionalInformation;
	bool isAvailableMyAccessRequestInformation;
	bool isAvailableMyRuleInformation;
	bool isAvailableMyErrorInformation;

	bool isAvailableMyLogonType;
	bool isAvailableMyImpersonationLevel;
	bool isAvailableMyAccountForWhichLogonFailed;
	bool isAvailableMyFailureInformation;
	bool isAvailableMyNewLogon;
	bool isAvailableMyDetailedAuthenticationInformation;

	int sizeOfEvent = 0;

	MySubject*mySubject;
	MyProviderInformation*myProviderInformation;
	MyObject*myObject;
	MyNetworkInformation*myNetworkInformation;
	MyLayerInformation*myLayerInformation;
	MyFilterInformation*myFilterInformation;
	MyChangeInformation*myChangeInformation;
	MyCalloutInformation*myCalloutInformation;
	MyApplicationInformation*myApplicationInformation;
	MyAdditionalInformation*myAdditionalInformation;
	MyAccessRequestInformation*myAccessRequestInformation;
	MyRuleInformation*myRuleInformation;
	MyErrorInformation*myErrorInformation;
	MyProcessInformation*myProcessInformation;

	MyLogonType*myLogonType;
	MyImpersonationLevel*myImpersonationLevel;
	MyAccountForWhichLogonFailed*myAccountForWhichLogonFailed;
	MyFailureInformation*myFailureInformation;
	MyNewLogon*myNewLogon;
	MyDetailedAuthenticationInformation*myDetailedAuthenticationInformation;
};
#endif