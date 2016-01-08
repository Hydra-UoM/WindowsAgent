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
		/**
		MyTimeStamp timeStamp(ullTimeStamp, st, ft);
		const wchar_t* message = extractEventMessageString();
		MyLogStructure outputLog(eventMessageString, levelMessageString, taskMessageString, opCodeMessageString, channelMessageString,
		providerMessageString, version, level, task, opCode, keywords, eventRecordID, executionProcessID, executionThreadID,
		channel, computer, EventID, timeStamp, message);
		*outputLogStructure = outputLog;
		return status;
		*/
		MyTimeStamp timeStamp(ullTimeStamp, st, ft);
		auto outputLog = std::make_unique<MyLogStructure>(/**eventMessageString, */levelMessageString, taskMessageString, opCodeMessageString, channelMessageString,
			providerMessageString, version, level, task, opCode, keywords, eventRecordID, executionProcessID, executionThreadID,
			channel, computer, EventID, timeStamp);
		extractEventMessageString(outputLog);

		*outputLogStructure = *(outputLog.get());
		//wprintf(L"Message: %ls\n", outputLog->message);
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

MyLogStructureMaker::~MyLogStructureMaker(void)
{

}

void MyLogStructureMaker::extractEventMessageString(std::unique_ptr<MyLogStructure> & outputLog)
{
	LPWSTR separators = L"\n\t";
	vector<wstring> results = splitLPWSTRWithManyDelimiters(eventMessageString, separators);
	int noOfSplitedStrings = 0;
	enum attributeName{
		Message, Client_ID,
		Source_Handle_Information, Source_Handle_ID, Source_Process_ID,
		New_Handle_Information, Target_Handle_ID, Target_Process_ID
	};
	wstring*arrayOfSplitted;
	arrayOfSplitted = new wstring[200];
	for (auto const& w : results)
	{
		const wchar_t* st = w.c_str();
		arrayOfSplitted[noOfSplitedStrings] = st;
		wstring temp = arrayOfSplitted[noOfSplitedStrings];
		int length = arrayOfSplitted[noOfSplitedStrings].length();

		int colonPosition = arrayOfSplitted[noOfSplitedStrings].find_last_of(':'); // if ':' is not exists it will result -1
		if (colonPosition != 1 && colonPosition != -1) // if 1 - process name, if -1 - not exists
		{
			temp = arrayOfSplitted[noOfSplitedStrings].substr(0, colonPosition);
		}
		if (temp.compare(L"\r") != 0)
		{
			arrayOfSplitted[noOfSplitedStrings] = temp;
			//wprintf(L"Splitted: %ls\n", arrayOfSplitted[noOfSplitedStrings].c_str());
			noOfSplitedStrings++;
		}
	}
	outputLog->message = arrayOfSplitted[0].c_str();
	//wprintf(L"Splitted: %ls\n", outputLog->message);
	//wprintf(L"Splitted: %d\n", wcscmp(arrayOfSplitted[1].c_str(), L"Subject"));
	//wprintf(L"Splitted: %d\n", arrayOfSplitted[1].compare(L"Subject")); // same - 0 // Appl : -1

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
	const wchar_t* Workstation_Name;	const wchar_t* Direction;	const wchar_t* Source_Address;	const wchar_t* Source_Port;	const wchar_t* Destination_Address;	const wchar_t* Destination_Port;	const wchar_t* Protocol;

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

	// LogonType
	const wchar_t* Logon_Type;

	// ImpersonationLevel
	const wchar_t* Impersonation_Level;

	// AccountForWhichLogonFailed
	const wchar_t* AccountForWhichLogonFailed_Security_ID;	const wchar_t* AccountForWhichLogonFailed_Account_Name;	const wchar_t* AccountForWhichLogonFailed_Account_Domain;

	// FailureInformation
	const wchar_t* Failure_Reason;	const wchar_t* Status;	const wchar_t* Sub_Status;

	// NewLogon
	const wchar_t* NewLogon_Security_ID;	const wchar_t* NewLogon_Account_Name;	const wchar_t* NewLogon_Account_Domain;
	const wchar_t* NewLogon_Logon_ID;	const wchar_t* NewLogon_Logon_GUID;

	// DetailedAuthenticationInformation
	const wchar_t* Logon_Process;	const wchar_t* Authentication_Package;	const wchar_t* Transited_Services;	const wchar_t* Package_Name;	const wchar_t* Key_Length;

	while (i < noOfSplitedStrings)
	{
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Subject") == 0)// There is Subject type
		{
			outputLog->isAvailableMySubject = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Security ID") == 0)
			{
				i++;
				Security_ID = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				Security_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account Name") == 0)
			{
				i++;
				Account_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Account_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account Domain") == 0)
			{
				i++;
				Account_Domain = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Account_Domain = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Logon ID") == 0)
			{
				i++;
				Logon_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Logon_ID = L"";
			}
			outputLog->mySubject = new MySubject(Security_ID, Account_Name, Account_Domain, Logon_ID);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Logon Type") == 0)
		{
			outputLog->isAvailableMyLogonType = true;
			i++;
			Logon_Type = arrayOfSplitted[i].c_str();
			i++;
			outputLog->myLogonType = new MyLogonType(Logon_Type);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Impersonation Level") == 0)
		{
			outputLog->isAvailableMyImpersonationLevel = true;
			i++;
			Impersonation_Level = arrayOfSplitted[i].c_str();
			i++;
			outputLog->myImpersonationLevel = new MyImpersonationLevel(Impersonation_Level);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account For Which Logon Failed") == 0)
		{
			outputLog->isAvailableMyAccountForWhichLogonFailed = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Security ID") == 0)
			{
				i++;
				AccountForWhichLogonFailed_Security_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				AccountForWhichLogonFailed_Security_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account Name") == 0)
			{
				i++;
				AccountForWhichLogonFailed_Account_Name = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				AccountForWhichLogonFailed_Account_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account Domain") == 0)
			{
				i++;
				AccountForWhichLogonFailed_Account_Domain = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				AccountForWhichLogonFailed_Account_Domain = L"";
			}
			outputLog->myAccountForWhichLogonFailed = new MyAccountForWhichLogonFailed(AccountForWhichLogonFailed_Security_ID, AccountForWhichLogonFailed_Account_Name, AccountForWhichLogonFailed_Account_Domain);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Failure Information") == 0)
		{
			outputLog->isAvailableMyFailureInformation = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Failure Reason") == 0)
			{
				i++;
				Failure_Reason = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				Failure_Reason = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Status") == 0)
			{
				i++;
				Status = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				Status = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Sub Status") == 0)
			{
				i++;
				Sub_Status = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Sub_Status = L"";
			}
			outputLog->myFailureInformation = new MyFailureInformation(Failure_Reason, Status, Sub_Status);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"New Logon") == 0)
		{
			outputLog->isAvailableMyNewLogon = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Security ID") == 0)
			{
				i++;
				NewLogon_Security_ID = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				NewLogon_Security_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account Name") == 0)
			{
				i++;
				NewLogon_Account_Name = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				NewLogon_Account_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Account Domain") == 0)
			{
				i++;
				NewLogon_Account_Domain = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				NewLogon_Account_Domain = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Logon ID") == 0)
			{
				i++;
				NewLogon_Logon_ID = arrayOfSplitted[i].c_str();
				i++;
			}
			else
			{
				NewLogon_Logon_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Logon GUID") == 0)
			{
				i++;
				NewLogon_Logon_GUID = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				NewLogon_Logon_GUID = L"";
			}
			outputLog->myNewLogon = new MyNewLogon(NewLogon_Security_ID, NewLogon_Account_Name, NewLogon_Account_Domain, NewLogon_Logon_ID, NewLogon_Logon_GUID);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Object") == 0)// There is Object type
		{
			outputLog->isAvailableMyObject = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Object Server") == 0)
			{
				i++;
				Object_Server = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Object_Server = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Object Type") == 0)
			{
				i++;
				Object_Type = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Object_Type = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Object Name") == 0)
			{
				i++;
				Object_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Object_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Handle ID") == 0)
			{
				i++;
				Handle_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Handle_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Resource Attributes") == 0)
			{
				i++;
				Resource_Attributes = arrayOfSplitted[i].c_str();
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Resource_Attributes = L"";
			}
			outputLog->myObject = new MyObject(Object_Server, Object_Type, Object_Name, Handle_ID, Resource_Attributes);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Process Information") == 0)// There is Process Information type
		{
			outputLog->isAvailableMyProcessInformation = true;
			i = i++;
			if (i < noOfSplitedStrings && (wcscmp(arrayOfSplitted[i].c_str(), L"Process ID") == 0 || wcscmp(arrayOfSplitted[i].c_str(), L"Caller Process ID") == 0) || wcscmp(arrayOfSplitted[i].c_str(), L"New Process ID") == 0)
			{
				i++;
				Process_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Process_ID = L"";
			}
			if (i < noOfSplitedStrings && (wcscmp(arrayOfSplitted[i].c_str(), L"Process Name") == 0 || wcscmp(arrayOfSplitted[i].c_str(), L"Caller Process Name") == 0 || wcscmp(arrayOfSplitted[i].c_str(), L"New Process Name") == 0))
			{
				i++;
				Process_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Process_Name = L"";
			}
			outputLog->myProcessInformation = new MyProcessInformation(Process_ID, Process_Name);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Application Information") == 0)// There is Application Information type
		{
			outputLog->isAvailableMyApplicationInformation = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Process ID") == 0)
			{
				i++;
				Application_Process_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Application_Process_ID: %ls\n", Application_Process_ID);
				i++;
			}
			else
			{
				Application_Process_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Application Name") == 0)
			{
				i++;
				Application_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Application_Name: %ls\n", Application_Name);
				i++;
			}
			else
			{
				Application_Name = L"";
			}
			outputLog->myApplicationInformation = new MyApplicationInformation(Application_Process_ID, Application_Name);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Network Information") == 0)// There is Network Information type
		{
			outputLog->isAvailableMyNetworkInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Workstation Name") == 0)
			{
				i++;
				Workstation_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Direction: %ls\n", Direction);
				i++;
			}
			else
			{
				Workstation_Name = L"";
			}

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Direction") == 0)
			{
				i++;
				Direction = arrayOfSplitted[i].c_str();
				//wprintf(L"Direction: %ls\n", Direction);
				i++;
			}
			else
			{
				Direction = L"";
			}
			if (i < noOfSplitedStrings && (wcscmp(arrayOfSplitted[i].c_str(), L"Source Address") == 0 || wcscmp(arrayOfSplitted[i].c_str(), L"Source Network Address") == 0))
			{
				i++;
				Source_Address = arrayOfSplitted[i].c_str();
				//wprintf(L"Source_Address: %ls\n", Source_Address);
				i++;
			}
			else
			{
				Source_Address = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Source Port") == 0)
			{
				i++;
				Source_Port = arrayOfSplitted[i].c_str();
				//wprintf(L"Source_Port: %ls\n", Source_Port);
				i++;
			}
			else
			{
				Source_Port = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Destination Address") == 0)
			{
				i++;
				Destination_Address = arrayOfSplitted[i].c_str();
				//wprintf(L"Destination_Address: %ls\n", Destination_Address);
				i++;
			}
			else
			{
				Destination_Address = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Destination Port") == 0)
			{
				i++;
				Destination_Port = arrayOfSplitted[i].c_str();
				//wprintf(L"Destination_Port: %ls\n", Destination_Port);
				i++;
			}
			else
			{
				Destination_Port = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Protocol") == 0)
			{
				i++;
				Protocol = arrayOfSplitted[i].c_str();
				//wprintf(L"Protocol: %ls\n", Protocol);
				i++;
			}
			else
			{
				Protocol = L"";
			}
			outputLog->myNetworkInformation = new MyNetworkInformation(Workstation_Name,Direction, Source_Address, Source_Port, Destination_Address, Destination_Port, Protocol);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Detailed Authentication Information") == 0)// There is Network Information type
		{
			outputLog->isAvailableMyDetailedAuthenticationInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Logon Process") == 0)
			{
				i++;
				Logon_Process = arrayOfSplitted[i].c_str();
				//wprintf(L"Direction: %ls\n", Direction);
				i++;
			}
			else
			{
				Logon_Process = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Authentication Package") == 0)
			{
				i++;
				Authentication_Package = arrayOfSplitted[i].c_str();
				//wprintf(L"Source_Address: %ls\n", Source_Address);
				i++;
			}
			else
			{
				Authentication_Package = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Transited Services") == 0)
			{
				i++;
				Transited_Services = arrayOfSplitted[i].c_str();
				//wprintf(L"Source_Port: %ls\n", Source_Port);
				i++;
			}
			else
			{
				Transited_Services = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Package Name (NTLM only)") == 0)
			{
				i++;
				Package_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Destination_Address: %ls\n", Destination_Address);
				i++;
			}
			else
			{
				Package_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Key Length") == 0)
			{
				i++;
				Key_Length = arrayOfSplitted[i].c_str();
				//wprintf(L"Destination_Port: %ls\n", Destination_Port);
				i++;
			}
			else
			{
				Key_Length = L"";
			}
			outputLog->myDetailedAuthenticationInformation = new MyDetailedAuthenticationInformation(Logon_Process, Authentication_Package, Transited_Services, Package_Name, Key_Length);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Provider Information") == 0)// There is Provider Information type
		{
			outputLog->isAvailableMyProviderInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"ID") == 0)
			{
				i++;
				Provider_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Provider_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Name") == 0)
			{
				i++;
				Provider_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Provider_Name = L"";
			}
			outputLog->myProviderInformation = new MyProviderInformation(Provider_ID, Provider_Name);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Change Information") == 0)// There is Change Information type
		{
			outputLog->isAvailableMyChangeInformation = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Change Type") == 0)
			{
				i++;
				Change_Type = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Change_Type = L"";
			}
			outputLog->myChangeInformation = new MyChangeInformation(Change_Type);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Access Request Information") == 0)// There is Access Request Information type
		{
			outputLog->isAvailableMyAccessRequestInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Transaction ID") == 0)
			{
				i++;
				Transaction_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Transaction_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Accesses") == 0)
			{
				i++;
				Accesses = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Accesses = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Access Reasons") == 0)
			{
				i++;
				Access_Reasons = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Access_Reasons = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Access Mask") == 0)
			{
				i++;
				Access_Mask = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Access_Mask = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Privileges Used for Access Check") == 0)
			{
				i++;
				Privileges_Used_For_Access_Check = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Privileges_Used_For_Access_Check = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Restricted SID Count") == 0)
			{
				i++;
				Restricted_SID_Count = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Restricted_SID_Count = L"";
			}
			outputLog->myAccessRequestInformation = new MyAccessRequestInformation(Transaction_ID, Accesses, Access_Reasons, Access_Mask,
				Privileges_Used_For_Access_Check, Restricted_SID_Count);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Filter Information") == 0)// There is Filter Information type
		{
			outputLog->isAvailableMyFilterInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"ID") == 0)
			{
				i++;
				Filter_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Filter_ID: %ls\n", Filter_ID);
				i++;
			}
			else
			{
				Filter_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Name") == 0)
			{
				i++;
				Filter_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Filter_Name: %ls\n", Filter_Name);
				i++;
			}
			else
			{
				Filter_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Type") == 0)
			{
				i++;
				Filter_Type = arrayOfSplitted[i].c_str();
				//wprintf(L"Filter_Type: %ls\n", Filter_Type);
				i++;
			}
			else
			{
				Filter_Type = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Run-Time ID") == 0)
			{
				i++;
				Runtime_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Runtime_ID: %ls\n", Runtime_ID);
				i++;
			}
			else
			{
				Runtime_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Filter Run-Time ID") == 0)
			{
				i++;
				Filter_Runtime_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Filter_Runtime_ID: %ls\n", Filter_Runtime_ID);
				i++;
			}
			else
			{
				Filter_Runtime_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Layer Name") == 0)
			{
				i++;
				Filter_Layer_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Filter_Layer_Name: %ls\n", Filter_Layer_Name);
				i++;
			}
			else
			{
				Filter_Layer_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Layer Run-Time ID") == 0)
			{
				i++;
				Filter_Layer_Runtime_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Filter_Layer_Runtime_ID: %ls\n", Filter_Layer_Runtime_ID);
				i++;
			}
			else
			{
				Filter_Layer_Runtime_ID = L"";
			}
			outputLog->myFilterInformation = new MyFilterInformation(Filter_ID, Filter_Name, Filter_Type, Runtime_ID, Filter_Runtime_ID, Filter_Layer_Name,
				Filter_Layer_Runtime_ID);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Layer Information") == 0)// There is Layer Information type
		{
			outputLog->isAvailableMyLayerInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"ID") == 0)
			{
				i++;
				Layer_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Layer_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Name") == 0)
			{
				i++;
				Layer_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Layer_Name = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Run-Time ID") == 0)
			{
				i++;
				Layer_Runtime_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Layer_Runtime_ID = L"";
			}
			outputLog->myLayerInformation = new MyLayerInformation(Layer_ID, Layer_Name, Layer_Runtime_ID);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Callout Information") == 0)// There is Callout Information type
		{
			outputLog->isAvailableMyCalloutInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"ID") == 0)
			{
				i++;
				Callout_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Callout_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Name") == 0)
			{
				i++;
				Callout_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Callout_Name = L"";
			}
			outputLog->myCalloutInformation = new MyCalloutInformation(Callout_ID, Callout_Name);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Rule Information") == 0)// There is Rule Information type
		{
			outputLog->isAvailableMyRuleInformation = true;
			i++;

			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"ID") == 0)
			{
				i++;
				Rule_ID = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Rule_ID = L"";
			}
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Name") == 0)
			{
				i++;
				Rule_Name = arrayOfSplitted[i].c_str();
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Rule_Name = L"";
			}
			outputLog->myRuleInformation = new MyRuleInformation(Rule_ID, Rule_Name);
		}
		if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Error Information") == 0)// There is Error Information type
		{
			outputLog->isAvailableMyErrorInformation = true;
			i++;
			if (i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i].c_str(), L"Reason") == 0)
			{
				i++;
				Error_Reason = arrayOfSplitted[i].c_str();
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Error_Reason = L"";
			}
			outputLog->myErrorInformation = new MyErrorInformation(Error_Reason);
		}
		i++;
	}
}