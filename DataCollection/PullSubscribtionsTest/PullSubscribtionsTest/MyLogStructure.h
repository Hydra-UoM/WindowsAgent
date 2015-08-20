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
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include "MyTimeStamp.h"
#include "MySubject.h"
#include "MyObject.h"
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
#include "MyAdditionalInformation.h" // haven't implemented in extracting part. condition value, id needs to be defined as arrays

#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyLogStructure
{
	public:
		MyLogStructure(LPWSTR eventMessageString1,LPWSTR levelMessageString1,LPWSTR taskMessageString1,LPWSTR opCodeMessageString1,
			LPWSTR channelMessageString1,LPWSTR providerMessageString1,int version1,int level1,int task1,int opCode1,UINT64 keywords1,
			UINT64 eventRecordID1,UINT32 executionProcessID1,UINT32 executionThreadID1,const wchar_t* channel1,
			LPCWSTR computer1,DWORD EventID1,MyTimeStamp& timeStamp1/**,WCHAR* processImageName1*/);
			
		~MyLogStructure(void);

		void print();
		void printExtractedEventMessageString();
		void extractEventMessageString();
		void initializeAvailableInformation();
		std::vector<wstring> splitLPWSTRWithManyDelimiters(const wstring &original, const wstring &delimiters);

	//private:
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

		MySubject*mySubject;
		const wchar_t*message;
		/**
		MyProviderInformation myProviderInformation;
		MyProcessInformation myProcessInformation;
		MyObject myObject;
		MyNetworkInformation myNetworkInformation;
		MyLayerInformation myLayerInformation;
		MyFilterInformation myFilterInformation;
		MyChangeInformation myChangeInformation;
		MyCalloutInformation myCalloutInformation;
		MyApplicationInformation myApplicationInformation;
		MyAdditionalInformation myAdditionalInformation;
		MyAccessRequestInformation myAccessRequestInformation;
		MyRuleInformation myRuleInformation;
		MyErrorInformation myErrorInformation;
		*/
};
#endif