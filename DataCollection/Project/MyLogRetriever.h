#include "stdafx.h"
#ifndef MyLogRetriever_H
#define MyLogRetriever_H

#include "MyLogStructureMaker.h"
#include "MyLogStructure.h"
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "GetProcessID.h"
#include "WindowsAgentConstants.h"

#include <sddl.h>
using namespace std;
#include "MyLogStruct.h"
#pragma comment(lib, "wevtapi.lib")

class MyLogRetriever
{
public:
	MyLogRetriever(std::string eventCategory1);

	BOOL IsKeyEvent(HANDLE hStdIn);
	void getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery, std::vector<DWORD>*process_id);
	void getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery);
	void getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery, int logonType);

	DWORD EnumerateResults(EVT_HANDLE hResults,std::vector<DWORD>*process_id);
	DWORD EnumerateResults(EVT_HANDLE hResults);
	DWORD EnumerateResults(EVT_HANDLE hResults, int logonType);

	bool getSetOfProcessIDs(string process_name);
	void releaseMemory();
	void handleLogRetrivalInfo(std::string logType, std::string strSecurityLevelConstraint,
		string process_name, int timePeriodInMilliSeconds1);
	void handleLogRetrivalInfo(std::string logType, std::string strSecurityLevelConstraint,
		int timePeriodInMilliSeconds1);
	std::wstring stringToWidestring(const std::string& s);
	string assignSecurityConstraint(string logType, string str_securityLevel);
	//DWORD EnumerateResultsOnEventIDs(EVT_HANDLE hResults, vector<int>eventIDs);
	vector<myStruct::myLogStructure>handleSuccessLoginEvents(int timePeriodInMilliSeconds1, int summarizationLevel);
	vector<myStruct::myLogStructure>handleFailedLoginEvents(int timePeriodInMilliSeconds1, int summarizationLevel);
	void handleFirewallEvents(int timePeriodInMilliSeconds1);
	void handleAccountUsage(int timePeriodInMilliSeconds1);
	void groupPolicyEditorsEvents(int timePeriodInMilliSeconds1);
	void windowsDefenderEvents(int timePeriodInMilliSeconds1);
	void mobileDeviceEvents(int timePeriodInMilliSeconds1);
	void printingServicesEvents(int timePeriodInMilliSeconds1);
	void systemOrServiceFailures(int timePeriodInMilliSeconds1);
	vector<myStruct::myLogStructure> clearingEventLogs(int timePeriodInMilliSeconds1, int summarizationLevel);
	vector<myStruct::myLogStructure> windowsUpdateErrors(int timePeriodInMilliSeconds1, int summarizationLevel);
	vector<myStruct::myLogStructure> applicationCrashes(int timePeriodInMilliSeconds1, int summarizationLevel);
	vector<myStruct::myLogStructure> softwareAndServicesInstallation(int timePeriodInMilliSeconds1, int summarizationLevel);
	vector<myStruct::myLogStructure> applicationWhitelisting(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1);
	vector<myStruct::myLogStructure> getRemoteLoginEvents(int timePeriodInMilliSeconds1, int summarizationLevel);

	void printResultedEvent(list<MyLogStructure*>myResultedLogStructures);
	vector<myStruct::myLogStructure> returnResultedEventWithStruct(list<MyLogStructure*>myResultedLogStructures);
	vector<myStruct::myLogStructure> returnResultedEventWithStruct(list<MyLogStructure*>myResultedLogStructures, int summarizationLevel);
	//summarization getSummarization();
	//void getLogs(int summarizationLevel1, int timePeriodInMilliSeconds1);

	~MyLogRetriever(void);

	string string_query;
	boolean isLevelConstraintAvailable;
	boolean isProcessIDConstraintAvailable;

	wstring wsLogType;
	LPCWSTR lpcwstrLogType;
	wstring wsCons;
	LPCWSTR pwsQuery;
	std::vector<DWORD>*process_id;
	int timePeriodInMilliSeconds;

	list<MyLogStructure*>myLogStructures;
	string processName = "";
	string eventCategory = "";
};
#endif