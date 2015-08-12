#include "stdafx.h"
#include "MyLogStructureMaker.h"
#include "MyTimeStamp.h"

MyLogStructureMaker::MyLogStructureMaker(EVT_HANDLE& hEvent1):hEvent(hEvent1)
{
	hEvent = hEvent1;
}

DWORD MyLogStructureMaker::getStatus(MyLogStructure*outputLogStructure)
{
	status = ERROR_SUCCESS;
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
		//outputLogStructure = nullptr;
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
				//outputLogStructure = nullptr;
                goto cleanup;
            }
        }

        if (ERROR_SUCCESS != (status = GetLastError()))
        {
            wprintf(L"EvtRender failed with %d\n", GetLastError());
			//outputLogStructure = nullptr;
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
		//outputLogStructure = nullptr;
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
	{
		MyTimeStamp timeStamp(ullTimeStamp,st,ft);
		WCHAR* processName = (WCHAR*)malloc(sizeof(WCHAR));
		if(GetProcessImageNameFromPID::getProcessNameFromProcessID(executionProcessID,&processName) == TRUE)
		{
			//_tprintf( TEXT("PROCESS NAME:  %s"),processName);
			// process name has been assigned
		}
		else
		{
			processName = L"couldn't found";
		}
		/**
		if (eventMessageString)
		{
			wprintf(L"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
			wprintf(L"EventID: %lu\n", EventID);
			wprintf(L"Event message string: %ls\n", eventMessageString);
			wprintf(L"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
		}
		*/
		MyLogStructure outputLog(eventMessageString,levelMessageString,taskMessageString,opCodeMessageString,channelMessageString,
			providerMessageString,version,level,task,opCode,keywords,eventRecordID,executionProcessID,executionThreadID,
			channel,computer,EventID,timeStamp,processName);
		*outputLogStructure = outputLog;

		/**
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
        wprintf(L"Event message string: %ls\n", eventMessageString);
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
	
	wprintf(L"***************************************\n");
	wprintf(L"TimeCreated SystemTime: %02d/%02d/%02d %02d:%02d:%02d.%I64u)\n", 
        st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond, ullNanoseconds);
	wprintf(L"\n\n");
	*/
		return status;
	}

cleanup:

   if (hContext)
        EvtClose(hContext);

    if (pRenderedValues)
        free(pRenderedValues);
	return status;
}

// Gets the specified message string from the event. If the event does not
// contain the specified message, the function returns NULL.
LPWSTR MyLogStructureMaker::GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId)
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
BOOL MyLogStructureMaker::IsKeyEvent(HANDLE hStdIn)
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

MyLogStructureMaker::~MyLogStructureMaker(void)
{

}