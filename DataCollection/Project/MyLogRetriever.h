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
#include <list>
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
	DWORD EnumerateResults(EVT_HANDLE hResults, int process_id);
	void releaseMemory();
	/**void handleLogs(std::string logType, int securityLevelConstraint,
	int securityLevel, int process_id, int timePeriodInMilliSeconds);*/
	void handleLogRetrivalInfo(std::string logType, std::string strSecurityLevelConstraint,
		int process_id1, int timePeriodInMilliSeconds1);
	std::wstring stringToWidestring(const std::string& s);
	string assignSecurityConstraint(string logType, string str_securityLevel);

	DWORD EnumerateResultsOnEventIDs(EVT_HANDLE hResults, DWORD eventIDs[]);
	list<string>handleSuccessLoginEvents();
	list<string>handleFailedLoginEvents();
	//void getEventsOnEventIDs(LPCWSTR pwsPath, LPCWSTR pwsQuery, DWORD eventIDs[]);
	//void handleEventsOnEventIDs(std::string logType, LPCWSTR pwsQuery, DWORD eventIDs[]);
	//void handleEventsOnEventID(std::string logType, DWORD eventID);
	//void getLogs();

	void printResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents);
	list<string> returnResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents);

	~MyLogRetriever(void);

	string string_query;
	boolean isLevelConstraintAvailable;
	boolean isProcessIDConstraintAvailable;

	wstring wsLogType;
	LPCWSTR lpcwstrLogType;
	wstring wsCons;
	LPCWSTR pwsQuery;
	int process_id;
	int timePeriodInMilliSeconds;

	MyLogStructure*myLogStructures[100];
	int numberOfAvailableEvents = 0;
};
#endif