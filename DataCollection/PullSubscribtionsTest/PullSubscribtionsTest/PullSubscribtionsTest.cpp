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

#include <sddl.h>

using namespace std;


#pragma comment(lib, "wevtapi.lib")

#define ARRAY_SIZE 1 // earlier was 10, handle by 10 of blocks
#define TIME_OUT INFINITE

DWORD EnumerateResults(EVT_HANDLE hResults);
BOOL IsKeyEvent(HANDLE hStdIn);
DWORD PrintEventSystemData(EVT_HANDLE hEvent);

MyLogStructure*myLogStructures[100];
int numberOfAvailableEvents = 0;

void filterEvents(LPWSTR pwsPath,LPWSTR pwsQuery)
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

    wprintf(L"Press any key to quit.\n");

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
			std::cout << "*********** Query Results ******************\n";
			status = EnumerateResults(hSubscription);
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

bool filterResultedEvents(int process_id,MyLogStructure*myResultedLogStructures[],int*numberOfFilteredEvents)
{
	bool isAvailable = false;
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
			//std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
			numberOfResultedEvents++;
			isAvailable = true;
		}
	}
	*numberOfFilteredEvents = numberOfResultedEvents;
	return isAvailable;
	//std::getchar();
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
DWORD EnumerateResults(EVT_HANDLE hResults)
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
			
			//MyLogStructure*outputLogStructure = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			myLogStructures[numberOfAvailableEvents] = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			status = l.getStatus(myLogStructures[numberOfAvailableEvents]);
			//std::cout << "*******************************************************************************\n";
			//myLogStructures[numberOfAvailableEvents]->print();
			//std::cout << "*******************************************************************************\n";
			numberOfAvailableEvents++;
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

void filterLogs(int process_id,MyLogStructure*myResultedLogStructures[],int*numberOfFilteredEvents)
{
	LPWSTR pwsPath = L"Security"; // channel name// "Application","Security","System","Setup","Operational" ;security works when admin previlege
    //LPWSTR pwsQuery = L"*[System/TimeCreated[timediff(@SystemTime) <= 10000]]";// xpath query // , [System/EventID!= 100]
	LPWSTR pwsQuery = L"*[System[(Level <= 3) and TimeCreated[timediff(@SystemTime) <= 10000]]]";
	filterEvents(pwsPath,pwsQuery);
	filterResultedEvents(process_id,myResultedLogStructures,numberOfFilteredEvents);
}

void __cdecl wmain()
{
	MyLogStructure*myResultedLogStructures[100];
	int numberOfFilteredEvents = 0;
	filterLogs(4,myResultedLogStructures,&numberOfFilteredEvents);
	// test with App : 8404 && 1000000ms
	// test with Sec: 4 && 10000ms
	// test with Sys: 4 && 10000000ms
	
	//std::cout << numberOfFilteredEvents << std::endl;
	printResultedEvent(myResultedLogStructures,numberOfFilteredEvents);
	getchar();
}