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

	eventMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageEvent);
	levelMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageLevel);
	taskMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageTask);
	opCodeMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageOpcode);
	channelMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageChannel);
	providerMessageString = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageProvider);

	version = (EvtVarTypeNull == pRenderedValues[EvtSystemVersion].Type) ? 0 : pRenderedValues[EvtSystemVersion].ByteVal;
	level = (EvtVarTypeNull == pRenderedValues[EvtSystemLevel].Type) ? 0 : pRenderedValues[EvtSystemLevel].ByteVal;
	task = (EvtVarTypeNull == pRenderedValues[EvtSystemTask].Type) ? 0 : pRenderedValues[EvtSystemTask].UInt16Val;
	opCode = (EvtVarTypeNull == pRenderedValues[EvtSystemOpcode].Type) ? 0 : pRenderedValues[EvtSystemOpcode].ByteVal;
	keywords = pRenderedValues[EvtSystemKeywords].UInt64Val;
	ULONGLONG ullTimeStamp = pRenderedValues[EvtSystemTimeCreated].FileTimeVal;
	eventRecordID = pRenderedValues[EvtSystemEventRecordId].UInt64Val;
	executionProcessID = pRenderedValues[EvtSystemProcessID].UInt32Val;
	executionThreadID = pRenderedValues[EvtSystemThreadID].UInt32Val;
	channel = (EvtVarTypeNull == pRenderedValues[EvtSystemChannel].Type) ? L"" : pRenderedValues[EvtSystemChannel].StringVal;
	computer = pRenderedValues[EvtSystemComputer].StringVal;

	EventID = pRenderedValues[EvtSystemEventID].UInt16Val;
	EventID = (EvtVarTypeNull != pRenderedValues[EvtSystemQualifiers].Type) ? MAKELONG(pRenderedValues[EvtSystemEventID].UInt16Val, pRenderedValues[EvtSystemQualifiers].UInt16Val) : EventID;

    ft.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
    ft.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
    FileTimeToSystemTime(&ft, &st);
    ullNanoseconds = (ullTimeStamp % 10000000) * 100; // Display nanoseconds instead of milliseconds for higher resolution
	{
		MyTimeStamp timeStamp(ullTimeStamp, st, ft);
		MyLogStructure outputLog(eventMessageString, levelMessageString, taskMessageString, opCodeMessageString, channelMessageString,
			providerMessageString, version, level, task, opCode, keywords, eventRecordID, executionProcessID, executionThreadID,
			channel, computer, EventID, timeStamp/**,processName*/);
		*outputLogStructure = outputLog;
		//extractEventMessageString(outputLogStructure); /** Need to fix */
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

std::vector<wstring> MyLogStructureMaker::splitLPWSTRWithManyDelimiters(const wstring &original, const wstring &delimiters)
{
	std::wstringstream stream(original);
	std::wstring line;
	vector <wstring> wordVector;

	while (std::getline(stream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(delimiters, prev)) != std::wstring::npos)
		{
			if (pos > prev)
				wordVector.emplace_back(line.substr(prev, pos - prev));

			prev = pos + 1;
		}

		if (prev < line.length())
			wordVector.emplace_back(line.substr(prev, std::wstring::npos));
	}

	return wordVector;
}

void MyLogStructureMaker::extractEventMessageString(MyLogStructure* outputLog)
{
	LPWSTR separators = L":\n\t";
	vector<wstring> results = splitLPWSTRWithManyDelimiters(eventMessageString, separators);
	int noOfSplitedStrings = 0;
	enum attributeName{
		Message, Client_ID,
		Source_Handle_Information, Source_Handle_ID, Source_Process_ID,
		New_Handle_Information, Target_Handle_ID, Target_Process_ID
	};
	const wchar_t*arrayOfSplitted[200];
	for (auto const& w : results)
	{
		const wchar_t* st = w.c_str();
		//wprintf(L"Splitted: %ls\n", st);
		arrayOfSplitted[noOfSplitedStrings] = st;
		noOfSplitedStrings++;
	}
	//wprintf(L"Message: %ls\n", arrayOfSplitted[0]);
	outputLog->message = arrayOfSplitted[0];
	// Setup logs contains Client id
	int i = 1;

	// Subject type
	const wchar_t* Security_ID;	const wchar_t* Account_Name;	const wchar_t* Account_Domain;	const wchar_t* Logon_ID;

	// Object type
	const wchar_t* Object_Server;	const wchar_t* Object_Type;	const wchar_t* Object_Name;	const wchar_t* Handle_ID;	const wchar_t* Resource_Attributes;

	// Process Information type
	const wchar_t* Process_ID;	const wchar_t* Process_Name;

	// Application Information type
	const wchar_t* Application_Process_ID;	const wchar_t* Application_Name;

	// Network Information type
	const wchar_t* Direction;	const wchar_t* Source_Address;	const wchar_t* Source_Port;	const wchar_t* Destination_Address;	const wchar_t* Destination_Port;	const wchar_t* Protocol;

	// Provider Information type
	const wchar_t* Provider_ID;	const wchar_t* Provider_Name;

	// Change Information type
	const wchar_t* Change_Type;

	// Access Request Information type
	const wchar_t* Transaction_ID;	const wchar_t* Accesses;	const wchar_t* Access_Reasons;	const wchar_t* Access_Mask;	const wchar_t* Privileges_Used_For_Access_Check;	const wchar_t* Restricted_SID_Count;

	// Filter Information type
	const wchar_t* Filter_ID;	const wchar_t* Filter_Name;	const wchar_t* Filter_Type;	const wchar_t* Runtime_ID;	const wchar_t* Filter_Runtime_ID;	const wchar_t* Filter_Layer_Name;	const wchar_t* Filter_Layer_Runtime_ID;
	
	// Layer Information type
	const wchar_t* Layer_ID;	const wchar_t* Layer_Name;	const wchar_t* Layer_Runtime_ID;

	// Callout Information type
	const wchar_t* Callout_ID;	const wchar_t* Callout_Name;

	// Rule Information type
	const wchar_t* Rule_ID;	const wchar_t* Rule_Name;

	// Error Information type
	const wchar_t* Error_Reason;

	while (i < noOfSplitedStrings)
	{
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Subject") == 0)// There is Subject type
		{
			outputLog->isAvailableMySubject = true;
			i = i + 2;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Security ID") == 0)
			{
				i++;
				Security_ID = arrayOfSplitted[i];
				//sid = wchar_t_pointerToString(Security_ID);
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Security_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Account Name") == 0)
			{
				i++;
				Account_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Account_Name = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Account Domain") == 0)
			{
				i++;
				Account_Domain = arrayOfSplitted[i];
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Account_Domain = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Logon ID") == 0)
			{
				i++;
				Logon_ID = arrayOfSplitted[i];
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Logon_ID = L"Unavailable for this event";
			}
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Object") == 0)// There is Object type
		{
			outputLog->isAvailableMyObject = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Object Server") == 0)
			{
				i++;
				Object_Server = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Object_Server = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Object Type") == 0)
			{
				i++;
				Object_Type = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Object_Type = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Object Name") == 0)
			{
				i++;
				Object_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Object_Name = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Handle ID") == 0)
			{
				i++;
				Handle_ID = arrayOfSplitted[i];
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Handle_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Resource Attributes") == 0)
			{
				i++;
				Resource_Attributes = arrayOfSplitted[i];
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Resource_Attributes = L"Unavailable for this event";
			}
			MyObject myObject1(Object_Server, Object_Type, Object_Name, Handle_ID, Resource_Attributes);
			//*myObject1 = myObject;
			//myObject1.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Process Information") == 0)// There is Process Information type
		{
			outputLog->isAvailableMyProcessInformation = true;
			i = i + 2;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Process ID") == 0)
			{
				i++;
				Process_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Process_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Process Name") == 0)
			{
				i++;
				Process_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Process_Name = L"Unavailable for this event";
			}
			MyProcessInformation myProcessInformation1(Process_ID, Process_Name);
			//*myProcessInformation1 = myProcessInformation;
			//myProcessInformation1.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Application Information") == 0)// There is Application Information type
		{
			outputLog->isAvailableMyApplicationInformation = true;
			i = i + 2;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Process ID") == 0)
			{
				i++;
				Application_Process_ID = arrayOfSplitted[i];
				//wprintf(L"Application_Process_ID: %ls\n", Application_Process_ID);
				i++;
			}
			else
			{
				Application_Process_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Application Name") == 0)
			{
				i++;
				Application_Name = arrayOfSplitted[i];
				//wprintf(L"Application_Name: %ls\n", Application_Name);
				i++;
			}
			else
			{
				Application_Name = L"Unavailable for this event";
			}
			MyApplicationInformation myApplicationInformation(Application_Process_ID, Application_Name);
			//*mySubject = mySubject1;
			//myApplicationInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Network Information") == 0)// There is Network Information type
		{
			outputLog->isAvailableMyNetworkInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Direction") == 0)
			{
				i++;
				Direction = arrayOfSplitted[i];
				//wprintf(L"Direction: %ls\n", Direction);
				i++;
			}
			else
			{
				Direction = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Source Address") == 0)
			{
				i++;
				Source_Address = arrayOfSplitted[i];
				//wprintf(L"Source_Address: %ls\n", Source_Address);
				i++;
			}
			else
			{
				Source_Address = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Source Port") == 0)
			{
				i++;
				Source_Port = arrayOfSplitted[i];
				//wprintf(L"Source_Port: %ls\n", Source_Port);
				i++;
			}
			else
			{
				Source_Port = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Destination Address") == 0)
			{
				i++;
				Destination_Address = arrayOfSplitted[i];
				//wprintf(L"Destination_Address: %ls\n", Destination_Address);
				i++;
			}
			else
			{
				Destination_Address = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Destination Port") == 0)
			{
				i++;
				Destination_Port = arrayOfSplitted[i];
				//wprintf(L"Destination_Port: %ls\n", Destination_Port);
				i++;
			}
			else
			{
				Destination_Port = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Protocol") == 0)
			{
				i++;
				Protocol = arrayOfSplitted[i];
				//wprintf(L"Protocol: %ls\n", Protocol);
				i++;
			}
			else
			{
				Protocol = L"Unavailable for this event";
			}
			MyNetworkInformation myNetworkInformation(Direction, Source_Address, Source_Port, Destination_Address, Destination_Port, Protocol);
			//*mySubject = mySubject1;
			//myNetworkInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Provider Information") == 0)// There is Provider Information type
		{
			outputLog->isAvailableMyProviderInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"ID") == 0)
			{
				i++;
				Provider_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Provider_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Name") == 0)
			{
				i++;
				Provider_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Provider_Name = L"Unavailable for this event";
			}
			MyProviderInformation myProviderInformation(Provider_ID, Provider_Name);
			//*mySubject = mySubject1;
			//myProviderInformation.print();
		}
		if (wcscmp(arrayOfSplitted[i], L"Change Information") == 0)// There is Change Information type
		{
			outputLog->isAvailableMyChangeInformation = true;
			i = i + 2;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Change Type") == 0)
			{
				i++;
				Change_Type = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Change_Type = L"Unavailable for this event";
			}
			MyChangeInformation myChangeInformation(Change_Type);
			//*mySubject = mySubject1;
			//myChangeInformation.print();
		}
		if (wcscmp(arrayOfSplitted[i], L"Access Request Information") == 0)// There is Access Request Information type
		{
			outputLog->isAvailableMyAccessRequestInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Transaction ID") == 0)
			{
				i++;
				Transaction_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Transaction_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Accesses") == 0)
			{
				i++;
				Accesses = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Accesses = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Access Reasons") == 0)
			{
				i++;
				Access_Reasons = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Access_Reasons = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Access Mask") == 0)
			{
				i++;
				Access_Mask = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Access_Mask = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Privileges Used for Access Check") == 0)
			{
				i++;
				Privileges_Used_For_Access_Check = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Privileges_Used_For_Access_Check = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Restricted SID Count") == 0)
			{
				i++;
				Restricted_SID_Count = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Restricted_SID_Count = L"Unavailable for this event";
			}
			MyAccessRequestInformation myAccessRequestInformation(Transaction_ID, Accesses, Access_Reasons, Access_Mask,
				Privileges_Used_For_Access_Check, Restricted_SID_Count);
			//*mySubject = mySubject1;
			//myAccessRequestInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Filter Information") == 0)// There is Filter Information type
		{
			outputLog->isAvailableMyFilterInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"ID") == 0)
			{
				i++;
				Filter_ID = arrayOfSplitted[i];
				//wprintf(L"Filter_ID: %ls\n", Filter_ID);
				i++;
			}
			else
			{
				Filter_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Name") == 0)
			{
				i++;
				Filter_Name = arrayOfSplitted[i];
				//wprintf(L"Filter_Name: %ls\n", Filter_Name);
				i++;
			}
			else
			{
				Filter_Name = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Type") == 0)
			{
				i++;
				Filter_Type = arrayOfSplitted[i];
				//wprintf(L"Filter_Type: %ls\n", Filter_Type);
				i++;
			}
			else
			{
				Filter_Type = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Run-Time ID") == 0)
			{
				i++;
				Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Runtime_ID: %ls\n", Runtime_ID);
				i++;
			}
			else
			{
				Runtime_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Filter Run-Time ID") == 0)
			{
				i++;
				Filter_Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Filter_Runtime_ID: %ls\n", Filter_Runtime_ID);
				i++;
			}
			else
			{
				Filter_Runtime_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Layer Name") == 0)
			{
				i++;
				Filter_Layer_Name = arrayOfSplitted[i];
				//wprintf(L"Filter_Layer_Name: %ls\n", Filter_Layer_Name);
				i++;
			}
			else
			{
				Filter_Layer_Name = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Layer Run-Time ID") == 0)
			{
				i++;
				Filter_Layer_Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Filter_Layer_Runtime_ID: %ls\n", Filter_Layer_Runtime_ID);
				i++;
			}
			else
			{
				Filter_Layer_Runtime_ID = L"Unavailable for this event";
			}
			MyFilterInformation myFilterInformation(Filter_ID, Filter_Name, Filter_Type, Runtime_ID, Filter_Runtime_ID, Filter_Layer_Name,
				Filter_Layer_Runtime_ID);
			//*mySubject = mySubject1;
			//myFilterInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Layer Information") == 0)// There is Layer Information type
		{
			outputLog->isAvailableMyLayerInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"ID") == 0)
			{
				i++;
				Layer_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Layer_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Name") == 0)
			{
				i++;
				Layer_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Layer_Name = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Run-Time ID") == 0)
			{
				i++;
				Layer_Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Layer_Runtime_ID = L"Unavailable for this event";
			}
			MyLayerInformation myLayerInformation(Layer_ID, Layer_Name, Layer_Runtime_ID);
			//*mySubject = mySubject1;
			//myLayerInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Callout Information") == 0)// There is Callout Information type
		{
			outputLog->isAvailableMyCalloutInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"ID") == 0)
			{
				i++;
				Callout_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Callout_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Name") == 0)
			{
				i++;
				Callout_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Callout_Name = L"Unavailable for this event";
			}
			MyCalloutInformation myCalloutInformation(Callout_ID, Callout_Name);
			//*mySubject = mySubject1;
			//myCalloutInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Rule Information") == 0)// There is Rule Information type
		{
			outputLog->isAvailableMyRuleInformation = true;
			i = i + 2;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"ID") == 0)
			{
				i++;
				Rule_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Rule_ID = L"Unavailable for this event";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Name") == 0)
			{
				i++;
				Rule_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Rule_Name = L"Unavailable for this event";
			}
			MyRuleInformation myRuleInformation(Rule_ID, Rule_Name);
			//*mySubject = mySubject1;
			//myRuleInformation.print();
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Error Information") == 0)// There is Error Information type
		{
			outputLog->isAvailableMyErrorInformation = true;
			i = i + 2;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i], L"Reason") == 0)
			{
				i++;
				Error_Reason = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Error_Reason = L"Unavailable for this event";
			}
			MyErrorInformation myErrorInformation(Error_Reason);
			//*mySubject = mySubject1;
			//myErrorInformation.print();
		}
		i++;
	}
	MySubject*mySubject;
	if ((outputLog->isAvailableMySubject))
	{
		mySubject = new MySubject(Security_ID, Account_Name, Account_Domain, Logon_ID);
		//mySubject1->setElements(Security_ID, Account_Name, Account_Domain, Logon_ID);
	}
}

MyLogStructureMaker::~MyLogStructureMaker(void)
{

}