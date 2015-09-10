#include "stdafx.h"
#ifndef MyLogRetriever_H
#define MyLogRetriever_H

#include "MyLogStructureMaker.h"
#include "MyLogStructure.h"
#include "GetProcessImageNameFromPID.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include "WindowsAgentConstants.h"

#include <sddl.h>

using namespace std;

#pragma comment(lib, "wevtapi.lib")

class MyLogRetriever
{
public:
	MyLogRetriever();
	BOOL IsKeyEvent(HANDLE hStdIn);
	void getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery, int process_id);
	void printResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents);
	DWORD EnumerateResults(EVT_HANDLE hResults, int process_id);
	void releaseMemory();
	void handleLogs(std::string logType, int securityLevelConstraint,
		int securityLevel, int process_id, int timePeriodInMilliSeconds);
	std::wstring stringToWidestring(const std::string& s);

	// need to test
	void handleEventsOnEventIDs(std::string logType, LPCWSTR pwsQuery, DWORD eventIDs[]);
	void getEventsOnEventIDs(LPCWSTR pwsPath, LPCWSTR pwsQuery, DWORD eventIDs[]);
	DWORD EnumerateResultsOnEventIDs(EVT_HANDLE hResults, DWORD eventIDs[]);

	void handleEventsOnEventID(std::string logType, DWORD eventID);
	~MyLogRetriever(void);
};
#endif