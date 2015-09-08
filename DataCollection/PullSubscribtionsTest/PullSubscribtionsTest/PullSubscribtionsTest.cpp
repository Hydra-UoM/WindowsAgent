// PullSubscribtionsTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MyLogStructureMaker.h"
#include "MyLogStructure.h"
#include "GetProcessImageNameFromPID.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <thread>
#include<string>
#include<sstream>
#include "WindowsAgentConstants.h"

#include <sddl.h>

using namespace std;


#pragma comment(lib, "wevtapi.lib")

#define ARRAY_SIZE 1 // earlier was 10, handle by 10 of blocks
#define TIME_OUT INFINITE

DWORD EnumerateResults(EVT_HANDLE hResults,int process_id);
BOOL IsKeyEvent(HANDLE hStdIn);
DWORD PrintEventSystemData(EVT_HANDLE hEvent);

MyLogStructure*myLogStructures[100];
int numberOfAvailableEvents = 0;

void getEvents(LPCWSTR pwsPath,LPCWSTR pwsQuery,int process_id)
{
	DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hSubscription = NULL;
    HANDLE hEventLog = NULL;

    HANDLE aWaitHandles[2];
    DWORD dwWait = 0;
    DWORD dwLastRecordNumber = 0;

    // Get a handle for console input, so you can break out of the loop.
    aWaitHandles[0] = GetStdHandle(STD_INPUT_HANDLE);
    if (INVALID_HANDLE_VALUE == aWaitHandles[0])
    {
        wprintf(L"GetStdHandle failed with %lu.\n", GetLastError());
        goto cleanup;
    }

    // Get a handle to a manual reset event object that the subscription will signal
    // when events become available that match your query criteria.
    aWaitHandles[1] = CreateEvent(NULL, TRUE, TRUE, NULL);
    if (NULL == aWaitHandles[1])
    {
        wprintf(L"CreateEvent failed with %lu.\n", GetLastError());
        goto cleanup;
    }

    // Subscribe to events.
    hSubscription = EvtSubscribe(NULL, aWaitHandles[1], pwsPath, pwsQuery, NULL, NULL, NULL, EvtSubscribeStartAtOldestRecord);
    if (NULL == hSubscription)
    {
        status = GetLastError();

        if (ERROR_EVT_CHANNEL_NOT_FOUND == status)
            wprintf(L"Channel %s was not found.\n", pwsPath);
        else if (ERROR_EVT_INVALID_QUERY == status)
            wprintf(L"The query %s was not found.\n", pwsQuery);
        else
            wprintf(L"EvtSubscribe failed with %lu.\n", status);
        goto cleanup;
    }

    // Loop until the user presses a key or there is an error && no more waits. Only the existing past events
    while (true)
    {
        dwWait = WaitForMultipleObjects(sizeof(aWaitHandles)/sizeof(HANDLE), aWaitHandles, FALSE, INFINITE);

        if (0 == dwWait - WAIT_OBJECT_0)  // Console input. If press key go out
        {
            if (IsKeyEvent(aWaitHandles[0]))
                break;
        }
        else if (1 == dwWait - WAIT_OBJECT_0) // Query results
        {
			std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
			status = EnumerateResults(hSubscription,process_id);
            if (ERROR_NO_MORE_ITEMS != status)
            {
				std::cout << "***********End Before Break******************\n";
                break;
				
            }
			break;// no more waits. Only the existing past events
            ResetEvent(aWaitHandles[1]);
        }
        else
        {
            if (WAIT_FAILED == dwWait)
            {
                wprintf(L"WaitForSingleObject failed with %lu\n", GetLastError());
            }
            break;
        }
    }
cleanup:

    if (hSubscription)
        EvtClose(hSubscription);

   /** if (aWaitHandles[0])
        CloseHandle(aWaitHandles[0]);*/
	// it will close the cmd window

    if (aWaitHandles[1])
        CloseHandle(aWaitHandles[1]);
}

void printResultedEvent(MyLogStructure*myResultedLogStructures[],int numberOfFilteredEvents)
{
	for(int i = 0; i < numberOfFilteredEvents; i++)
	{
		myResultedLogStructures[i]->print();
		std::cout << "*******************************************************************************\n";
	}
}

// Enumerate the events in the result set.
DWORD EnumerateResults(EVT_HANDLE hResults,int process_id)
{
    DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hEvents[ARRAY_SIZE];
    DWORD dwReturned = 0;
    while (true)
    {
        // Get a block of events from the result set.
		BOOL e = EvtNext(hResults, ARRAY_SIZE, hEvents, TIME_OUT, 0, &dwReturned);
        if (!e)
        {
			status = GetLastError();
            if (ERROR_NO_MORE_ITEMS != status)
            {
                wprintf(L"EvtNext failed with %lu\n", status);
            }
            goto cleanup;
        }

        // For each event, call the PrintEvent function which renders the
        // event for display.
		//wprintf(L"dwReturned %lu\n", dwReturned);
        for (DWORD i = 0; i < dwReturned; i++)
        {
			//status = PrintEventSystemData(hEvents[i]);

			MyLogStructureMaker l(hEvents[i]);
			//pBuffer = (LPWSTR)malloc(dwBufferSize * sizeof(WCHAR));
			
			MyLogStructure*outputLogStructure = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			//myLogStructures[numberOfAvailableEvents] = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			status = l.getStatus(outputLogStructure);
			if(process_id != -1)
			{
				if(outputLogStructure->executionProcessID == process_id)
				{
					myLogStructures[numberOfAvailableEvents] = outputLogStructure;
					numberOfAvailableEvents++;
				}
				else
				{
					// just leave the event
				}
			}
			else
			{
				myLogStructures[numberOfAvailableEvents] = outputLogStructure;
				numberOfAvailableEvents++;
			}
			//std::cout << "*******************************************************************************\n";
			//myLogStructures[numberOfAvailableEvents]->print();
			//std::cout << "*******************************************************************************\n";
            if (ERROR_SUCCESS == status) // PrintEvent
            {
                EvtClose(hEvents[i]);
                hEvents[i] = NULL;
            }
            else
            {
                goto cleanup;
            }
        }
		// I put to get only first ARRAY_SIZE number of events
		//getchar();
		//goto cleanup;
    }
cleanup:

    // Closes any events in case an error occurred above.
    for (DWORD i = 0; i < dwReturned; i++)
    {
        if (NULL != hEvents[i])
            EvtClose(hEvents[i]);
    }

    return status;
}

// Determines whether the console input was a key event.
BOOL IsKeyEvent(HANDLE hStdIn)
{
    INPUT_RECORD Record[128];
    DWORD dwRecordsRead = 0;
    BOOL fKeyPress = FALSE;

    if (ReadConsoleInput(hStdIn, Record, 128, &dwRecordsRead))
    {
        for (DWORD i = 0; i < dwRecordsRead; i++)
        {
            if (KEY_EVENT == Record[i].EventType)
            {
                fKeyPress = TRUE;
                break;
            }
        }
    }

    return fKeyPress;
}

std::wstring stringToWidestring(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

void releaseMemory()
{
	for(int i = 0; i < numberOfAvailableEvents; i++)
	{
		free(myLogStructures[i]);
	}
}

void __cdecl wmain()
{
	//acceptParameters(Log type, level cons, level,time period,pid);
	/** parameters to accept 
			i.  logType - from WindowsAgentConstants.h,
			ii. securityLevelConstraint - from WindowsAgentConstants.h, take all events ==> give ALL
			iii.securityLevel - 
					Application Level:
					0 -Information, 1 - Information, 2 - Error, 3 – Warning, 4 – Information
					Security
					0 -Information, 1 -, 2 - Error, 3 –, 4 – Information
					System
					0 -, 1 - Critical, 2 - Error, 3 – Warning, 4 – Information
			iv. timePeriodInMilliSeconds - monitoring time period
			v. process_id - if not want to filter enter -1
			
	*/
	std::string logType = MY_SECURITY;
	SecurityLevelRelationshipConstraints securityLevelConstraint = EQUAL_TO;
	int securityLevel = 0;
	int timePeriodInMilliSeconds = 10000;
	int process_id = 4;

	// test with App : 8404 && 1000000ms
	// test with Sec: 4 && 10000ms
	// test with Sys: 4 && 10000000ms

	wstring wsLogType = stringToWidestring(logType);
    LPCWSTR lpcwstrLogType = wsLogType.c_str();
    //wcout << lpcwstrLogType;

	string string_query = "";
	boolean isLevelConstraintAvailable = false;
	boolean isProcessIDConstraintAvailable = false;
	if(securityLevelConstraint != ALL)	isLevelConstraintAvailable		= true;
	if(process_id != -1)				isProcessIDConstraintAvailable	= true;

	if(!isLevelConstraintAvailable)
	{
		string_query = string_query + "*[System/TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	}
	else
	{
		string_query = string_query + "*[System[(Level ";
		string sec_lev_cons = "";
		switch(securityLevelConstraint)
		{
			case LESS_THAN:					sec_lev_cons = "< ";		break;
			case LESS_THAN_OR_EQUAL_TO:		sec_lev_cons = "<= ";		break;
			case GREATER_THAN:				sec_lev_cons = "> ";		break;
			case GREATER_THAN_OR_EQUAL_TO:	sec_lev_cons = ">= ";		break;
			case EQUAL_TO:					sec_lev_cons = "= ";		break;
		}
		string_query = string_query + sec_lev_cons + to_string(securityLevel) + ") and TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]]";
	}	

	wstring wsCons = stringToWidestring(string_query);
	LPCWSTR pwsQuery = wsCons.c_str();
	//wcout << pwsQuery;

	std::cout << "*********** Query Results *****************************\n";
	while(true)
	{
		getEvents(lpcwstrLogType,pwsQuery,process_id);
		printResultedEvent(myLogStructures,numberOfAvailableEvents);
		numberOfAvailableEvents = 0;
		//releaseMemory();
		Sleep(timePeriodInMilliSeconds);
	}
	getchar();
}

/**********************************************************************************************************************************/
/**
void filterLogsWithProcessID(int process_id,MyLogStructure*myResultedLogStructures[],int*numberOfFilteredEvents)
{
	LPWSTR pwsPath = L"Security"; // channel name// "Application","Security","System","Setup","Operational" ;security works when admin previlege
    //LPWSTR pwsQuery = L"*[System/TimeCreated[timediff(@SystemTime) <= 10000]]";// xpath query // , [System/EventID!= 100]
	LPWSTR pwsQuery = L"*[System[(Level <= 3) and TimeCreated[timediff(@SystemTime) <= 10000]]]";
	getEvents(pwsPath,pwsQuery,process_id);
	filterResultedEvents(process_id,myResultedLogStructures,numberOfFilteredEvents);
}

void filterResultedEvents(int process_id,MyLogStructure*myResultedLogStructures[],int*numberOfFilteredEvents)
{
	int numberOfResultedEvents = 0;
	for(int i = 0; i < numberOfAvailableEvents; i++)
	{
		//myLogStructures[i]->print();
		if(myLogStructures[i]->executionProcessID == process_id)
		{
			//std::cout << "True\n";
			myResultedLogStructures[numberOfResultedEvents] = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			myResultedLogStructures[numberOfResultedEvents] = myLogStructures[i];
			//myResultedLogStructures[numberOfResultedEvents]->print();
			numberOfResultedEvents++;
		}
	}
	*numberOfFilteredEvents = numberOfResultedEvents;
	//std::getchar();
}
*/