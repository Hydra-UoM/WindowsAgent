#include "stdafx.h"
#include "MyLogRetriever.h"

#define ARRAY_SIZE 1 // earlier was 10, handle by 10 of blocks
#define TIME_OUT INFINITE

MyLogRetriever::MyLogRetriever()
{

}

// Enumerate the events in the result set.
DWORD MyLogRetriever::EnumerateResultsOnEventIDs(EVT_HANDLE hResults, DWORD eventIDs[])
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hEvents[ARRAY_SIZE];
	DWORD dwReturned = 0;
	while (true) //GetAsyncKeyState(VK_ESCAPE) != true
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

			MyLogStructure*outputLogStructure;// = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			//myLogStructures[numberOfAvailableEvents] = (MyLogStructure*)malloc(sizeof(MyLogStructure));
			status = l.getStatus(outputLogStructure);

			for (int k = 0; k < (sizeof(eventIDs) / sizeof(*eventIDs)); k++)
			{
				if (outputLogStructure->EventID == eventIDs[k])
				{
					myLogStructures[numberOfAvailableEvents] = outputLogStructure;
					numberOfAvailableEvents++;
				}
				else
				{
					// just leave the event
				}
			}
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

void MyLogRetriever::getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery, int process_id)
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
	while (true)//GetAsyncKeyState(VK_ESCAPE) != true
	{
		dwWait = WaitForMultipleObjects(sizeof(aWaitHandles) / sizeof(HANDLE), aWaitHandles, FALSE, INFINITE);

		if (0 == dwWait - WAIT_OBJECT_0)  // Console input. If press key go out
		{
			if (IsKeyEvent(aWaitHandles[0]))
				break;
		}
		else if (1 == dwWait - WAIT_OBJECT_0) // Query results
		{
			//std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
			status = EnumerateResults(hSubscription, process_id);
			if (ERROR_NO_MORE_ITEMS != status)
			{
				//std::cout << "***********End Before Break******************\n";
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

// Enumerate the events in the result set.
DWORD MyLogRetriever::EnumerateResults(EVT_HANDLE hResults, int process_id)
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hEvents[ARRAY_SIZE];
	DWORD dwReturned = 0;
	while (true)//GetAsyncKeyState(VK_ESCAPE) != true
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
			if (process_id != -1)
			{
				if (outputLogStructure->executionProcessID == process_id)
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
BOOL MyLogRetriever::IsKeyEvent(HANDLE hStdIn)
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

std::wstring MyLogRetriever::stringToWidestring(const std::string& s)
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

void MyLogRetriever::releaseMemory()
{
	for (int i = 0; i < numberOfAvailableEvents; i++)
	{
		free(myLogStructures[i]);
	}
}

void MyLogRetriever::handleLogRetrivalInfo(std::string logType, std::string str_securityLevel, int process_id1, int timePeriodInMilliSeconds1)
{
	process_id = process_id1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;

	wsLogType = stringToWidestring(logType);
	lpcwstrLogType = wsLogType.c_str();
	//wcout << lpcwstrLogType;

	string_query = "";
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	string strSecurityLevelConstraint = "";

	if (str_securityLevel.compare("ALL") != 0)
	{
		isLevelConstraintAvailable = true;
		strSecurityLevelConstraint = assignSecurityConstraint(logType, str_securityLevel);
	}
	if (process_id != -1)
	{
		isProcessIDConstraintAvailable = true;
	}

	if (!isLevelConstraintAvailable)
	{
		string_query = string_query + "*[System/TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	}
	else
	{
		string_query = string_query + "*[System[";
		//string sec_lev_cons = "";
		string_query = string_query + strSecurityLevelConstraint + " and TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]]";
	}

	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery;
}

list<string>MyLogRetriever::handleSuccessLoginEvents()
{
	numberOfAvailableEvents = 0;
	wstring wsLogType = stringToWidestring("Security");
	LPCWSTR lpcwstrLogType = wsLogType.c_str();

	string string_query = "";
	// success login: 4624
	string_query = string_query + "*[System/EventID= 4624]";
	wstring wsCons = stringToWidestring(string_query);
	LPCWSTR pwsQuery = wsCons.c_str();

	getEvents(lpcwstrLogType, pwsQuery, -1);
	//printResultedEvent(myLogStructures, numberOfAvailableEvents);
	return returnResultedEvent(myLogStructures, numberOfAvailableEvents);
	//numberOfAvailableEvents = 0;
}

list<string>MyLogRetriever::handleFailedLoginEvents()
{
	numberOfAvailableEvents = 0;
	wstring wsLogType = stringToWidestring("Security");
	LPCWSTR lpcwstrLogType = wsLogType.c_str();

	string string_query = "";
	//Failed login: 4625, 4648, 537
	/**
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
	string_query = string_query + "*[System/EventID= 4625 or EventID= 4648 or EventID= 537]";
	//cout << string_query << endl;
	wstring wsCons = stringToWidestring(string_query);
	LPCWSTR pwsQuery = wsCons.c_str();

	getEvents(lpcwstrLogType, pwsQuery, -1);
	//printResultedEvent(myLogStructures, numberOfAvailableEvents);
	return returnResultedEvent(myLogStructures, numberOfAvailableEvents);
	//numberOfAvailableEvents = 0;
}

string  MyLogRetriever::assignSecurityConstraint(string logType, string str_securityLevel)
{
	string strSecurityLevelConstraint;
	int int_securityLevel;
	/*0. Information 1. Warning 2. Error 3. Critical " 4. Warning & Error & Critical5. Error & Critical*/
	if (str_securityLevel.compare("Information") == 0)
	{
		int_securityLevel = 0;
	}
	else if (str_securityLevel.compare("Warning") == 0)
	{
		int_securityLevel = 1;
	}
	else if (str_securityLevel.compare("Error") == 0)
	{
		int_securityLevel = 2;
	}
	else if (str_securityLevel.compare("Critical") == 0)
	{
		int_securityLevel = 3;
	}
	else if (str_securityLevel.compare("Warning & Error & Critical") == 0)
	{
		int_securityLevel = 4;
	}
	else if (str_securityLevel.compare("Error & Critical") == 0)
	{
		int_securityLevel = 5;
	}
	else
	{
		int_securityLevel = 5; // anything else
	}
	if (logType.compare("Security") == 0)// Security log: 0 -Information, 1 -, 2 - Error, 3 –, 4 – Information
	{
		switch (int_securityLevel)
		{
		case 0:// information
			strSecurityLevelConstraint = "(Level = 0 or Level = 4)";// 4 and 1 or 3 // ??
			break;
		case 1:// warning
			strSecurityLevelConstraint = "(Level = 1)";// 1 or 3 // ??
			break;
		case 2:// error
			strSecurityLevelConstraint = "(Level = 2)";// ??
			break;
		case 3:// Critical
			strSecurityLevelConstraint = "(Level = 3)";// 1 or 3 // ??
			break;
		case 4:// Warning & Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2 or Level = 3)"; // ??
			break;
		case 5:// Error & Critical
			strSecurityLevelConstraint = "(Level = 2 or Level = 3)"; // ??
			break;
		default:// Warning & Error & Critical is given
			cout << "Warning & Error & Critical is given" << endl;
			strSecurityLevelConstraint = "(Level = 1 or Level = 2 or Level = 3)"; // ??
		}
	}
	else if (logType.compare("Application") == 0) // 	Application Log: 0 - Information, 1 - Information, 2 - Error, 3 – Warning, 4 – Information
	{
		switch (int_securityLevel)
		{
		case 0:// information
			strSecurityLevelConstraint = "(Level = 0 or Level = 1 or Level = 4)";
			break;
		case 1:// warning
			strSecurityLevelConstraint = "(Level = 3)";
			break;
		case 2:// error
			strSecurityLevelConstraint = "(Level = 2)";
			break;
			/*
			case 3:// Critical
			strSecurityLevelConstraint = "(Level = 3)";// 1 or 3 // ??
			break;
			case 4:// Warning & Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2 or Level = 3)"; // ??
			break;
			case 5:// Error & Critical
			strSecurityLevelConstraint = "(Level = 2 or Level = 3)"; // ??
			break;
			*/
		default:// Warning & Error is given
			cout << "Warning & Error is given" << endl;
			strSecurityLevelConstraint = "(Level = 2 or Level = 3)"; // ??
		}
	}
	else if (logType.compare("System") == 0)  // 	System : 0 - , 1 - Critical, 2 - Error, 3 – Warning, 4 – Information
	{
		switch (int_securityLevel)
		{
		case 0:// information
			strSecurityLevelConstraint = "(Level = 4)"; // ?? // 0
			break;
		case 1:// warning
			strSecurityLevelConstraint = "(Level = 3)";
			break;
		case 2:// error
			strSecurityLevelConstraint = "(Level = 2)";
			break;
		case 3:// Critical
			strSecurityLevelConstraint = "(Level = 1)";
			break;
		case 4:// Warning & Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2 or Level = 3)"; // ??
			break;
		case 5:// Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
			break;
		default:// Error & Critical is given
			cout << "Error & Critical is given" << endl;
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
		}
	}
	else if (logType.compare("Setup") == 0) // setup
	{
		switch (int_securityLevel)
		{
			/*
			case 0:// information
			strSecurityLevelConstraint = "(Level = 4)"; // ?? // 0
			break;
			case 1:// warning
			strSecurityLevelConstraint = "(Level = 3)";
			break;
			case 2:// error
			strSecurityLevelConstraint = "(Level = 2)";
			break;
			case 3:// Critical
			strSecurityLevelConstraint = "(Level = 1)";
			break;
			case 4:// Warning & Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2 or Level = 3)"; // ??
			break;
			case 5:// Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
			break;
			*/
		default:// Error & Critical is given
			cout << "Error & Critical is given" << endl;
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
		}
	}
	else if (logType.compare("Operational") == 0) // operational
	{
		switch (int_securityLevel)
		{
			/*
			case 0:// information
			strSecurityLevelConstraint = "(Level = 4)"; // ?? // 0
			break;
			case 1:// warning
			strSecurityLevelConstraint = "(Level = 3)";
			break;
			case 2:// error
			strSecurityLevelConstraint = "(Level = 2)";
			break;
			case 3:// Critical
			strSecurityLevelConstraint = "(Level = 1)";
			break;
			case 4:// Warning & Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2 or Level = 3)"; // ??
			break;
			case 5:// Error & Critical
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
			break;
			*/
		default:// Error & Critical is given
			cout << "Error & Critical is given" << endl;
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
		}
	}
	else
	{
		strSecurityLevelConstraint = "Error";
	}
	return strSecurityLevelConstraint;
}

/**
void MyLogRetriever::handleEventsOnEventIDs(std::string logType, LPCWSTR pwsQuery, DWORD eventIDs[])
{
while (true) //GetAsyncKeyState(VK_ESCAPE) != true
{
wstring wsLogType = stringToWidestring(logType);
LPCWSTR lpcwstrLogType = wsLogType.c_str();
//getEventsOnEventIDs(lpcwstrLogType, pwsQuery, eventIDs);
string string_query = "*[System/";
for (int i = 0; i < (sizeof(eventIDs) / sizeof(*eventIDs)) - 1; i++)
{
string_query = string_query + "EventID= " + to_string(eventIDs[i]) + " or ";//EventID= 4648 or EventID= 537]";
}
string_query = string_query + "EventID = " + to_string(eventIDs[(sizeof(eventIDs) / sizeof(*eventIDs)) - 1]) + "]";
wstring wsCons = stringToWidestring(string_query);
LPCWSTR pwsQuery = wsCons.c_str();
getEvents(lpcwstrLogType, pwsQuery, -1);

printResultedEvent(myLogStructures, numberOfAvailableEvents);
list<string> MyLogRetriever::returnResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents);
numberOfAvailableEvents = 0;
//releaseMemory();
}
}

void MyLogRetriever::handleEventsOnEventID(std::string logType, DWORD eventID)
{
while (true) //GetAsyncKeyState(VK_ESCAPE) != true
{
wstring wsLogType = stringToWidestring(logType);
LPCWSTR lpcwstrLogType = wsLogType.c_str();

string string_query = "";
string_query = string_query + "*[System/EventID= " + to_string(eventID) + "]";
wstring wsCons = stringToWidestring(string_query);
LPCWSTR pwsQuery = wsCons.c_str();

getEvents(lpcwstrLogType, pwsQuery, -1);
//printResultedEvent(myLogStructures, numberOfAvailableEvents);
list<string> MyLogRetriever::returnResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents);
numberOfAvailableEvents = 0;
}
}
*/

void MyLogRetriever::printResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents)
{
	for (int i = 0; i < numberOfFilteredEvents; i++)
	{
		myResultedLogStructures[i]->print();
		std::cout << "*******************************************************************************\n";
	}
}

list<string> MyLogRetriever::returnResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents)
{
	list<string>returnedResult;
	list<string>local_returnedResult;
	if (numberOfFilteredEvents == 0)
	{
		returnedResult.push_back("None");
		return returnedResult;
	}
	for (int i = 0; i < numberOfFilteredEvents; i++)
	{
		returnedResult.push_back("start"); // start is inserted to indicate start of a log's details
		local_returnedResult = myResultedLogStructures[i]->toLogString();
		returnedResult.splice(returnedResult.end(), local_returnedResult);
		returnedResult.push_back("end"); // end is inserted to indicate end of a log's details
	}
	return returnedResult;
}


MyLogRetriever::~MyLogRetriever(void)
{

}