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
LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);

//DWORD PrintEvent(EVT_HANDLE hEvent);
//DWORD PrintEventValues(EVT_HANDLE hEvent);

void filterEvents(LPWSTR pwsPath,LPWSTR pwsQuery)
{
	DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hSubscription = NULL;
    HANDLE hEventLog = NULL;

	/**
	The following query selects all events from the channel or log file where the severity level is
	less than or equal to 3 and the event occurred in the last 24 hour period.
	*/
	// *[System[(Level &lt;= 3) and TimeCreated[timediff(@SystemTime) &lt;= 86400000]]]
	//*[System/EventID= 4624 and System/TimeCreated[timediff(@SystemTime) < 86400000]]
	/**
	Application Level:
	0 -Information, 1 - Information, 2 - Error, 3 – Warning, 4 – Information
	Security
	0 -Information, 1 -, 2 - Error, 3 –, 4 – Information
	System
	0 -, 1 - Critical, 2 - Error, 3 – Warning, 4 – Information
	*/
	/**
	Application : 0 - The software protection service has been started or stopped
	2: The open procedure for service <> failed
	3: The rule engine reported a failed VL activation attempt
	4: Starting, ending session & installation
	*/
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
		getchar();
        goto cleanup;
    }

    wprintf(L"Press any key to quit.\n");

    // Loop until the user presses a key or there is an error.
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

    if (aWaitHandles[0])
        CloseHandle(aWaitHandles[0]);

    if (aWaitHandles[1])
        CloseHandle(aWaitHandles[1]);
}

void __cdecl wmain()
{
	LPWSTR pwsPath = L"Security"; // channel name// "Application","Security","System","Setup","Operational" ;security works when admin previlege
    LPWSTR pwsQuery = L"*";// xpath query // , [System/EventID!= 100]
	filterEvents(pwsPath,pwsQuery);
	// L"*[System/Level= 2]" 
	// L"*[System/EventID= 4624]" // success login
	// L"*[System/Level < 1]";
	/**
	4625,4626,4627,4628,4630,4635,4649,4740,4771,4772,4777

4624	An account was successfully logged on
4625	An account failed to log on
4647	User initiated logoff
4720	A user account was created
4723	An attempt was made to change an account's password
4724	An attempt was made to reset an account's password
4726	A user account was deleted
4800	PC was locked
4801	PC was unlocked
4802	Screensaver ON
4803	Screensaver OFF
4672: Special privileges assigned to new logon

4673: A privileged service was called



logclear - system - 104 ***
security log clear - 1102 ***
process creation - 4688

	4625: An account failed to log on
	Ref:
	https://www.ultimatewindowssecurity.com/securitylog/encyclopedia/event.aspx?eventID=4625
	4648: A logon was attempted using explicit credentials	
	Ref:
	https://www.ultimatewindowssecurity.com/securitylog/encyclopedia/event.aspx?eventID=4648
	537: Logon failure - The logon attempt failed for other reasons.
	Ref:
	https://www.ultimatewindowssecurity.com/securitylog/encyclopedia/event.aspx?eventid=537
	*/
	/**
	WCHAR* processName = (WCHAR*)malloc(sizeof(WCHAR));
	if(GetProcessImageNameFromPID::getProcessNameFromProcessID(580,&processName) == TRUE)
	{
		_tprintf( TEXT("***********\n"));
		_tprintf( TEXT("PROCESS NAME:  %s"),processName);
	}
	getchar();
	*/
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
			MyLogStructure*outputLogStructure = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			status = l.getStatus(outputLogStructure);
			(*outputLogStructure).print();	
			std::cout << "*******************************************************************************\n";
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
		getchar();
		goto cleanup;
    }
	//getchar();
	//goto cleanup;
cleanup:

    // Closes any events in case an error occurred above.
    for (DWORD i = 0; i < dwReturned; i++)
    {
        if (NULL != hEvents[i])
            EvtClose(hEvents[i]);
    }

    return status;
}

/****************************************************************************************************************/
DWORD PrintEventSystemData(EVT_HANDLE hEvent)
{
    DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hContext = NULL;
    DWORD dwBufferSize = 0;
    DWORD dwBufferUsed = 0;
    DWORD dwPropertyCount = 0;
    PEVT_VARIANT pRenderedValues = NULL; // Event Format
    
    LPWSTR pwsSid = NULL;

    ULONGLONG ullNanoseconds = 0;
    
    
	// Identify the components of the event that you want to render. In this case,
    // render the system section of the event.
    hContext = EvtCreateRenderContext(0, NULL, EvtRenderContextSystem);
    if (NULL == hContext)
    {
        wprintf(L"EvtCreateRenderContext failed with %lu\n", status = GetLastError());
        goto cleanup;
    }

    // When you render the user data or system section of the event, you must specify
    // the EvtRenderEventValues flag. The function returns an array of variant values 
    // for each element in the user data or system section of the event. For user data
    // or event data, the values are returned in the same order as the elements are 
    // defined in the event. For system data, the values are returned in the order defined
    // in the EVT_SYSTEM_PROPERTY_ID enumeration.
    if (!EvtRender(hContext, hEvent, EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount))
    {
        if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
        {
            dwBufferSize = dwBufferUsed;
            pRenderedValues = (PEVT_VARIANT)malloc(dwBufferSize);
            if (pRenderedValues)
            {
                EvtRender(hContext, hEvent, EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount);
            }
            else
            {
                wprintf(L"malloc failed\n");
                status = ERROR_OUTOFMEMORY;
                goto cleanup;
            }
        }

        if (ERROR_SUCCESS != (status = GetLastError()))
        {
            wprintf(L"EvtRender failed with %d\n", GetLastError());
            goto cleanup;
        }
    }

    // Print the values from the System section of the element.
	LPCWSTR providerName = pRenderedValues[EvtSystemProviderName].StringVal;
	EVT_HANDLE hProviderMetadata = EvtOpenPublisherMetadata(NULL, pRenderedValues[EvtSystemProviderName].StringVal, NULL, 0, 0);
	LPWSTR pwsMessage = NULL; // message
	if (NULL == hProviderMetadata)
    {
        wprintf(L"EvtOpenPublisherMetadata failed with %d\n", GetLastError());
        goto cleanup;
    }

	LPWSTR eventMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageEvent);
	LPWSTR levelMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageLevel);
	LPWSTR taskMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageTask);
	LPWSTR opCodeMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageOpcode);
	LPWSTR channelMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageChannel);
	LPWSTR providerMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageProvider);

	int version = (EvtVarTypeNull == pRenderedValues[EvtSystemVersion].Type) ? 0 : pRenderedValues[EvtSystemVersion].ByteVal;
	int level = (EvtVarTypeNull == pRenderedValues[EvtSystemLevel].Type) ? 0 : pRenderedValues[EvtSystemLevel].ByteVal;
	int task = (EvtVarTypeNull == pRenderedValues[EvtSystemTask].Type) ? 0 : pRenderedValues[EvtSystemTask].UInt16Val;
	int opCode = (EvtVarTypeNull == pRenderedValues[EvtSystemOpcode].Type) ? 0 : pRenderedValues[EvtSystemOpcode].ByteVal;
	UINT64 keywords = pRenderedValues[EvtSystemKeywords].UInt64Val;
	ULONGLONG ullTimeStamp = pRenderedValues[EvtSystemTimeCreated].FileTimeVal;
	UINT64 eventRecordID = pRenderedValues[EvtSystemEventRecordId].UInt64Val;
	UINT32 executionProcessID = pRenderedValues[EvtSystemProcessID].UInt32Val;
	UINT32 executionThreadID = pRenderedValues[EvtSystemThreadID].UInt32Val;
	const wchar_t* channel = (EvtVarTypeNull == pRenderedValues[EvtSystemChannel].Type) ? L"" : pRenderedValues[EvtSystemChannel].StringVal;
	LPCWSTR computer = pRenderedValues[EvtSystemComputer].StringVal;

	DWORD EventID = pRenderedValues[EvtSystemEventID].UInt16Val;
	EventID = (EvtVarTypeNull != pRenderedValues[EvtSystemQualifiers].Type) ? MAKELONG(pRenderedValues[EvtSystemEventID].UInt16Val, pRenderedValues[EvtSystemQualifiers].UInt16Val) : EventID;

	FILETIME ft;
    ft.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
    ft.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
	SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);
    ullNanoseconds = (ullTimeStamp % 10000000) * 100; // Display nanoseconds instead of milliseconds for higher resolution

	wprintf(L"EventRecordID: %I64u\n",eventRecordID);
	wprintf(L"EventID: %lu\n", EventID);
	wprintf(L"Provider Name: %s\n", providerName);
    wprintf(L"Version: %u\n",version);
    wprintf(L"Level: %u\n",level);
    wprintf(L"Task: %hu\n",task);
    wprintf(L"Opcode: %u\n",opCode);
	wprintf(L"Keywords: 0x%I64x\n",keywords);
	wprintf(L"Execution ProcessID: %lu\n",executionProcessID);
    wprintf(L"Execution ThreadID: %lu\n",executionThreadID);
    wprintf(L"Channel: %s\n",channel);
    wprintf(L"Computer: %s\n",computer);
	// Get the various message strings from the event.
	if (eventMessageString)
    {
        wprintf(L"Event message string: %s\n", eventMessageString);
        free(eventMessageString);
        eventMessageString = NULL;
    }
    if (levelMessageString)
    {
        wprintf(L"Level message string: %s\n", levelMessageString);
        free(levelMessageString);
        levelMessageString = NULL;
    }
    if (taskMessageString)
    {
        wprintf(L"Task message string: %s\n", taskMessageString);
        free(taskMessageString);
        taskMessageString = NULL;
    }
	if (opCodeMessageString)
    {
        wprintf(L"Opcode message string: %s\n", opCodeMessageString);
        free(opCodeMessageString);
        opCodeMessageString = NULL;
    }
    if (channelMessageString)
    {
        wprintf(L"Channel message string: %s\n\n", channelMessageString);
        free(channelMessageString);
        channelMessageString = NULL;
    }
	if (providerMessageString)
    {
        wprintf(L"Provider message string: %s\n\n", providerMessageString);
        free(providerMessageString);
        providerMessageString = NULL;
    }
	
	wprintf(L"TimeCreated SystemTime: %02d/%02d/%02d %02d:%02d:%02d.%I64u)\n", 
        st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond, ullNanoseconds);
	wprintf(L"\n\n");
cleanup:

    if (hContext)
        EvtClose(hContext);

    if (pRenderedValues)
        free(pRenderedValues);

    return status;
}

// Gets the specified message string from the event. If the event does not
// contain the specified message, the function returns NULL.
LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId)
{
    LPWSTR pBuffer = NULL;
    DWORD dwBufferSize = 0;
    DWORD dwBufferUsed = 0;
    DWORD status = 0;

    if (!EvtFormatMessage(hMetadata, hEvent, 0, 0, NULL, FormatId, dwBufferSize, pBuffer, &dwBufferUsed))
    {
        status = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == status)
        {
            // An event can contain one or more keywords. The function returns keywords
            // as a list of keyword strings. To process the list, you need to know the
            // size of the buffer, so you know when you have read the last string, or you
            // can terminate the list of strings with a second null terminator character 
            // as this example does.
            if ((EvtFormatMessageKeyword == FormatId))
                pBuffer[dwBufferSize-1] = L'\0';
            else
                dwBufferSize = dwBufferUsed;

            pBuffer = (LPWSTR)malloc(dwBufferSize * sizeof(WCHAR));

            if (pBuffer)
            {
                EvtFormatMessage(hMetadata, hEvent, 0, 0, NULL, FormatId, dwBufferSize, pBuffer, &dwBufferUsed);

                // Add the second null terminator character.
                if ((EvtFormatMessageKeyword == FormatId))
                    pBuffer[dwBufferUsed-1] = L'\0';
            }
            else
            {
                wprintf(L"malloc failed\n");
            }
        }
        else if (ERROR_EVT_MESSAGE_NOT_FOUND == status || ERROR_EVT_MESSAGE_ID_NOT_FOUND == status)
            ;
        else
        {
            wprintf(L"EvtFormatMessage failed with %u\n", status);
        }
    }

    return pBuffer;
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
/*************************************************************************************************************/