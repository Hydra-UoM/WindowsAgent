#include "stdafx.h"
#include "MyLogRetriever.h"

#define ARRAY_SIZE 1 // earlier was 10, handle by 10 of blocks
#define TIME_OUT INFINITE

MyLogRetriever::MyLogRetriever(std::string eventCategory1)
{
	eventCategory = eventCategory1;
}

DWORD MyLogRetriever::EnumerateResultsOnEventIDs(EVT_HANDLE hResults, vector<int>eventIDs)
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
			if (status == ERROR_SUCCESS){ // added later
				vector<int>::iterator it = eventIDs.begin();
				int count = 0;
				while (count < whiteListLength)
				{
					if (outputLogStructure->EventID == *it)
					{
						if (numberOfAvailableEvents < 800)
						{
							myLogStructures[numberOfAvailableEvents] = outputLogStructure;
							numberOfAvailableEvents++;
						}
					}
					else
					{
						// just leave the event
					}
					count++;
					++it;
				}
			} // added later
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
	totalNumberOfEvents = totalNumberOfEvents + numberOfAvailableEvents;
	return status;
}
/**
void MyLogRetriever::getEventsOnEventID(LPCWSTR pwsPath, LPCWSTR pwsQuery, list<int>eventIDs)
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
status = EnumerateResultsOnEventIDs(hSubscription, eventIDs);
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

if (aWaitHandles[1])
CloseHandle(aWaitHandles[1]);
}
*/

void MyLogRetriever::getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery, std::vector<DWORD>*process_id)
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hSubscription = NULL;
	HANDLE hEventLog = NULL;

	HANDLE aWaitHandles[2];
	DWORD dwWait = 0;
	DWORD dwLastRecordNumber = 0;

	//wprintf(L"++++++++++++++++++++: %s\n", pwsQuery);

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

void MyLogRetriever::getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery)
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hSubscription = NULL;
	HANDLE hEventLog = NULL;

	HANDLE aWaitHandles[2];
	DWORD dwWait = 0;
	DWORD dwLastRecordNumber = 0;

	//wprintf(L"++++++++++++++++++++: %s\n", pwsQuery);

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
			status = EnumerateResults(hSubscription);
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
DWORD MyLogRetriever::EnumerateResults(EVT_HANDLE hResults,std::vector<DWORD>*process_id)
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
			if (status == ERROR_SUCCESS)
			{ // added later
				for (int i = 0; i < process_id->size(); i++)
				{
					if (outputLogStructure->executionProcessID == (*process_id)[i])
					{
						if (numberOfAvailableEvents < 800)
						{
							myLogStructures[numberOfAvailableEvents] = outputLogStructure;
							numberOfAvailableEvents++;
						}
					}
					else
					{
						// just leave the event
					}
				}
			} // added later
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
	totalNumberOfEvents = totalNumberOfEvents + numberOfAvailableEvents;
	return status;
}

DWORD MyLogRetriever::EnumerateResults(EVT_HANDLE hResults)
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
			if (status == ERROR_SUCCESS)
			{ // added later
				//cout << numberOfAvailableEvents << endl; // need to check
				if (numberOfAvailableEvents < 800)
				{
					myLogStructures[numberOfAvailableEvents] = outputLogStructure;
					numberOfAvailableEvents++;
				}
			} // added later
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
	totalNumberOfEvents = totalNumberOfEvents + numberOfAvailableEvents;
	return status;
}

DWORD MyLogRetriever::EnumerateResults(EVT_HANDLE hResults, int logonType)
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

			if (status == ERROR_SUCCESS)
			{ // added later
				//cout << numberOfAvailableEvents << endl; // need to check
				string myLogonType = CW2A((outputLogStructure->myLogonType)->Logon_Type);
				std::string::size_type sz;   // alias of size_t
				int int_logonType = std::stoi(myLogonType, &sz);
				if (int_logonType == logonType)
				{
					if (numberOfAvailableEvents < 800)
					{
						myLogStructures[numberOfAvailableEvents] = outputLogStructure;
						numberOfAvailableEvents++;
					}
				}
			} // added later
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
	totalNumberOfEvents = totalNumberOfEvents + numberOfAvailableEvents;
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
		//free(myLogStructures[i]);
	}
}

bool MyLogRetriever::getSetOfProcessIDs(string process_name)
{
	process_id = new std::vector<DWORD>;
	processName = process_name;
	if (getSetOfProcessID(process_name, process_id))
	{
		/**
		for (int i = 0; i < (*process_id).size(); i++)
		{
			cout << (*process_id)[i] << endl;
		}
		*/
		return true;
	}
	else
		return false;
}

void MyLogRetriever::handleLogRetrivalInfo(std::string logType, std::string str_securityLevel, string process_name, int timePeriodInMilliSeconds1)
{
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
	isProcessIDConstraintAvailable = true;
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

vector<myStruct::myLogStructure> MyLogRetriever::getAllLogsForAProcess(std::string str_securityLevel, int timePeriodInMilliSeconds1, int summarizationLevel, std::string process_name)
{
	vector<myStruct::myLogStructure>logStructList;

	MyLogRetriever myLogRetriever1("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructSecurityList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever1.handleLogRetrivalInfo("Security", str_securityLevel, process_name, timePeriodInMilliSeconds1);
	myLogRetriever1.getSetOfProcessIDs(process_name);
	myLogRetriever1.getEvents(myLogRetriever1.lpcwstrLogType, myLogRetriever1.pwsQuery, myLogRetriever1.process_id);
	logStructSecurityList = myLogRetriever1.returnResultedEventWithStruct(myLogRetriever1.myLogStructures, myLogRetriever1.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever2("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructApplicationList;
	myLogRetriever2.handleLogRetrivalInfo("Application", str_securityLevel, process_name, timePeriodInMilliSeconds1);
	myLogRetriever1.getSetOfProcessIDs(process_name);
	myLogRetriever2.getEvents(myLogRetriever2.lpcwstrLogType, myLogRetriever2.pwsQuery, myLogRetriever2.process_id);
	logStructApplicationList = myLogRetriever2.returnResultedEventWithStruct(myLogRetriever2.myLogStructures, myLogRetriever2.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever3("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructSetupList;
	myLogRetriever3.handleLogRetrivalInfo("Setup", str_securityLevel, process_name, timePeriodInMilliSeconds1);
	myLogRetriever3.getSetOfProcessIDs(process_name);
	myLogRetriever3.getEvents(myLogRetriever3.lpcwstrLogType, myLogRetriever3.pwsQuery, myLogRetriever3.process_id);
	logStructSetupList = myLogRetriever3.returnResultedEventWithStruct(myLogRetriever3.myLogStructures, myLogRetriever3.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever4("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructSystemList;
	myLogRetriever4.handleLogRetrivalInfo("System", str_securityLevel, process_name, timePeriodInMilliSeconds1);
	myLogRetriever4.getSetOfProcessIDs(process_name);
	myLogRetriever4.getEvents(myLogRetriever4.lpcwstrLogType, myLogRetriever4.pwsQuery, myLogRetriever4.process_id);
	logStructSystemList = myLogRetriever4.returnResultedEventWithStruct(myLogRetriever4.myLogStructures, myLogRetriever4.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever5("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructOperationalList;
	myLogRetriever5.handleLogRetrivalInfo("Operational", str_securityLevel, process_name, timePeriodInMilliSeconds1);
	myLogRetriever5.getSetOfProcessIDs(process_name);
	myLogRetriever5.getEvents(myLogRetriever5.lpcwstrLogType, myLogRetriever5.pwsQuery, myLogRetriever5.process_id);
	logStructOperationalList = myLogRetriever5.returnResultedEventWithStruct(myLogRetriever5.myLogStructures, myLogRetriever5.numberOfAvailableEvents, summarizationLevel);
	
	logStructList.reserve(logStructSecurityList.size() + logStructApplicationList.size() + logStructSetupList.size() + logStructSystemList.size() + logStructOperationalList.size());
	logStructList.insert(logStructList.end(), logStructSecurityList.begin(), logStructSecurityList.end());
	logStructList.insert(logStructList.end(), logStructApplicationList.begin(), logStructApplicationList.end());
	logStructList.insert(logStructList.end(), logStructSetupList.begin(), logStructSetupList.end());
	logStructList.insert(logStructList.end(), logStructSystemList.begin(), logStructSystemList.end());
	logStructList.insert(logStructList.end(), logStructOperationalList.begin(), logStructOperationalList.end());
	/**
	int i = 0;
	for (iteratorStruct = logStructList.begin(); iteratorStruct != logStructList.end(); iteratorStruct++)
	{
		cout << i << ":" << (*iteratorStruct).channelMessageString << endl;
		i++;
	}
	*/
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogRetriever::getAllLogs(std::string str_securityLevel, int timePeriodInMilliSeconds1, int summarizationLevel)
{
	vector<myStruct::myLogStructure>logStructList;
	MyLogRetriever myLogRetriever1("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructSecurityList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever1.handleLogRetrivalInfo("Security",str_securityLevel, timePeriodInMilliSeconds1);
	myLogRetriever1.getEvents(myLogRetriever1.lpcwstrLogType, myLogRetriever1.pwsQuery);
	logStructSecurityList = myLogRetriever1.returnResultedEventWithStruct(myLogRetriever1.myLogStructures, myLogRetriever1.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever2("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructApplicationList;
	myLogRetriever2.handleLogRetrivalInfo("Application", str_securityLevel, timePeriodInMilliSeconds1);
	myLogRetriever2.getEvents(myLogRetriever2.lpcwstrLogType, myLogRetriever2.pwsQuery);
	logStructApplicationList = myLogRetriever2.returnResultedEventWithStruct(myLogRetriever2.myLogStructures, myLogRetriever2.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever3("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructSystemList;
	myLogRetriever3.handleLogRetrivalInfo("System", str_securityLevel, timePeriodInMilliSeconds1);
	myLogRetriever3.getEvents(myLogRetriever3.lpcwstrLogType, myLogRetriever3.pwsQuery);
	logStructSystemList = myLogRetriever3.returnResultedEventWithStruct(myLogRetriever3.myLogStructures, myLogRetriever3.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever4("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructSetupList;
	myLogRetriever4.handleLogRetrivalInfo("Setup", str_securityLevel, timePeriodInMilliSeconds1);
	myLogRetriever4.getEvents(myLogRetriever4.lpcwstrLogType, myLogRetriever4.pwsQuery);
	logStructSetupList = myLogRetriever4.returnResultedEventWithStruct(myLogRetriever4.myLogStructures, myLogRetriever4.numberOfAvailableEvents, summarizationLevel);

	MyLogRetriever myLogRetriever5("USER_DEFINED");
	vector<myStruct::myLogStructure>logStructOperationalList;
	myLogRetriever5.handleLogRetrivalInfo("Operational", str_securityLevel, timePeriodInMilliSeconds1);
	myLogRetriever5.getEvents(myLogRetriever5.lpcwstrLogType, myLogRetriever5.pwsQuery);
	logStructOperationalList = myLogRetriever5.returnResultedEventWithStruct(myLogRetriever5.myLogStructures, myLogRetriever5.numberOfAvailableEvents, summarizationLevel);

	logStructList.reserve(logStructSecurityList.size() + logStructApplicationList.size() + logStructSystemList.size() + logStructSetupList.size() + logStructOperationalList.size());
	logStructList.insert(logStructList.end(), logStructSecurityList.begin(), logStructSecurityList.end());
	logStructList.insert(logStructList.end(), logStructApplicationList.begin(), logStructApplicationList.end());
	logStructList.insert(logStructList.end(), logStructSystemList.begin(), logStructSystemList.end());
	logStructList.insert(logStructList.end(), logStructSetupList.begin(), logStructSetupList.end());
	logStructList.insert(logStructList.end(), logStructOperationalList.begin(), logStructOperationalList.end());
	/**
	int i = 0;
	for (iteratorStruct = logStructList.begin(); iteratorStruct != logStructList.end(); iteratorStruct++)
	{
		//cout << i << ":" << (*iteratorStruct).eventRecordID << endl;
		cout << i << ":" << (*iteratorStruct).channelMessageString << endl;
		i++;
	}
	*/
	return logStructList;
}

void MyLogRetriever::handleLogRetrivalInfo(std::string logType, std::string str_securityLevel, int timePeriodInMilliSeconds1)
{
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
/**
void MyLogRetriever::addWhiteList(int timePeriodInMilliSeconds1)
{
timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
whiteListLength = 0;
whiteListEntries = getFileContentsLineByLine("\WhiteList.txt");
lpcwstrLogType = L"Security";
//lpcwstrLogType = L"Application";
//lpcwstrLogType = L"System";
//lpcwstrLogType = L"Setup";
//lpcwstrLogType = L"Operational";
//handleEventsOnEventIDs(lpcwstrLogType, whiteListEntries);
}

list<int> MyLogRetriever::getFileContentsLineByLine(const char*fileName)
{
string line;
ifstream infile;
infile.open(fileName);
string previousLine = "";
list<int>whiteListEntries;
int noOfWhiteListEntries = 0;
while (!infile.eof())
{
getline(infile, line);
if (line != previousLine)
{
previousLine = line;
int temp;
try
{
temp = stoi(line);
whiteListEntries.push_back(temp);
noOfWhiteListEntries++;
}
catch (std::invalid_argument& e)
{
cout << "Invalid argument" << endl;
}
catch (std::out_of_range& e){
cout << "Out of range" << endl;
}
}
}
whiteListLength = noOfWhiteListEntries;
infile.close();
return whiteListEntries;
}
*/
vector<myLogStructure>MyLogRetriever::handleSuccessLoginEvents(int timePeriodInMilliSeconds1, int summarizationLevel)
{
	numberOfAvailableEvents = 0;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	wstring wsLogType = stringToWidestring("Security");
	lpcwstrLogType = wsLogType.c_str();

	string string_query = "";
	// success login: 4624
	string_query = string_query + "*[System[EventID= 4624]" + "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wstring wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();

	getEvents(lpcwstrLogType, pwsQuery);
	//printResultedEvent(myLogStructures, numberOfAvailableEvents);
	return returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	//numberOfAvailableEvents = 0;
}

vector<myLogStructure>MyLogRetriever::handleFailedLoginEvents(int timePeriodInMilliSeconds1, int summarizationLevel)
{
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	numberOfAvailableEvents = 0;
	wstring wsLogType = stringToWidestring("Security");
	lpcwstrLogType = wsLogType.c_str();

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
	string_query = string_query + "*[System[EventID= 4625 or EventID= 4648 or EventID= 537]" + "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	//cout << string_query << endl;
	wstring wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();

	getEvents(lpcwstrLogType, pwsQuery);
	//printResultedEvent(myLogStructures, numberOfAvailableEvents);
	return returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
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
			//cout << "Warning & Error & Critical is given" << endl;
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
			//cout << "Warning & Error is given" << endl;
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
			//cout << "Error & Critical is given" << endl;
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
			//cout << "Error & Critical is given" << endl;
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
		}
	}
	else if (logType.compare("Operational") == 0) // Operational
	{
		switch (int_securityLevel)
		{
		case 2:// error
			strSecurityLevelConstraint = "(Level = 2)";
			break;
			/*
			case 0:// information
			strSecurityLevelConstraint = "(Level = 4)"; // ?? // 0
			break;
			case 1:// warning
			strSecurityLevelConstraint = "(Level = 3)";
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
			//cout << "Error & Critical is given" << endl;
			strSecurityLevelConstraint = "(Level = 1 or Level = 2)"; // ??
		}
	}
	else
	{
		strSecurityLevelConstraint = "Error";
	}
	return strSecurityLevelConstraint;
}

void MyLogRetriever::handleFirewallEvents(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"Security";
	//string strSecurityLevelConstraint = "";
	string string_query = "";

	string_query = string_query + "*[System[EventID= 2004 or EventID= 2005 or EventID= 2006 or EventID= 2009 or EventID= 2033]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
}

void MyLogRetriever::handleAccountUsage(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"Security";
	//string strSecurityLevelConstraint = "";
	string string_query = "";

	string_query = string_query + "*[System[EventID= 4740 or EventID= 4728 or EventID= 4732 or EventID= 4756 or EventID= 4735 or EventID= 4624 or EventID= 4625 or EventID= 4648]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
}

void MyLogRetriever::groupPolicyEditorsEvents(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"System";
	//string strSecurityLevelConstraint = "";
	string string_query = "";

	string_query = string_query + "*[System[EventID= 1125 or EventID= 1127 or EventID= 1129]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery << endl;
}

void MyLogRetriever::windowsDefenderEvents(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"Operational";
	//string strSecurityLevelConstraint = "";
	string string_query = "";

	string_query = string_query + "*[System[EventID= 1005 or EventID= 1006 or EventID= 1008 or EventID= 1010 or EventID= 2001 or EventID= 2003 or EventID= 2004 or EventID= 3002 or EventID= 5008]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery << endl;
}

void MyLogRetriever::mobileDeviceEvents(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"Operational";
	//string strSecurityLevelConstraint = "";
	string string_query = "";

	string_query = string_query + "*[System[EventID= 10000 or EventID= 10001 or EventID= 8000 or EventID= 8011 or EventID= 8001 or EventID= 8003 or EventID= 11000"
		+ " or EventID= 11001 or EventID= 11002 or EventID= 11004 or EventID= 11005 or EventID= 11006 or EventID= 11010 or EventID= 8002 or EventID= 12011 or EventID= 12012 or EventID= 12013]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery << endl;
}

void MyLogRetriever::printingServicesEvents(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"Operational";
	//string strSecurityLevelConstraint = "";
	string string_query = "";

	string_query = string_query + "*[System[EventID= 307]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery << endl;
}

void MyLogRetriever::systemOrServiceFailures(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"System";
	//string strSecurityLevelConstraint = "";
	string string_query = "";
	string_query = string_query + "*[System[EventID= 7022 or EventID= 7023 or EventID= 7024 or EventID= 7026 or EventID= 7031 or EventID= 7032 or EventID= 7034]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery << endl;
}

/**
void MyLogRetriever::getLogs(int summarizationLevel1, int timePeriodInMilliSeconds1)
{
	summarizationLevel = summarizationLevel1; // 0 - send all, 1 - medium, 2 - only event message
}
*/

vector<myStruct::myLogStructure> MyLogRetriever::clearingEventLogs(int timePeriodInMilliSeconds1, int summarizationLevel, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"System";
	//string strSecurityLevelConstraint = "";
	string string_query = "";
	string_query = string_query + "*[System[EventID= 104]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();

	//wcout << pwsQuery << endl;

	getEvents(lpcwstrLogType, pwsQuery);
	vector<myStruct::myLogStructure>partial1;
	if (numberOfAvailableEvents != 0)
	{
		partial1 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	}
	int partialNumberOfEvents1 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>partial2;
	lpcwstrLogType = L"Security";
	//string strSecurityLevelConstraint = "";
	string_query = "";
	string_query = string_query + "*[System[EventID= 1102]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	//wcout << pwsQuery << endl;

	getEvents(lpcwstrLogType, pwsQuery);
	//localLast = myLogStructures[numberOfAvailableEvents - 1]->toLogString();
	partial2 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	int partialNumberOfEvents2 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>logsListToReturn;
	logsListToReturn.reserve(partial1.size() + partial2.size());
	logsListToReturn.insert(logsListToReturn.end(), partial1.begin(), partial1.end());
	logsListToReturn.insert(logsListToReturn.end(), partial2.begin(), partial2.end());

	numberOfAvailableEvents = numberOfAvailableEvents + partialNumberOfEvents1 + partialNumberOfEvents2;
	return logsListToReturn;
}

vector<myStruct::myLogStructure> MyLogRetriever::windowsUpdateErrors(int timePeriodInMilliSeconds1, int summarizationLevel, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"Operational";
	//string strSecurityLevelConstraint = "";
	string string_query = "";
	string_query = string_query + "*[System[EventID= 20 or EventID= 24 or EventID= 25 or EventID= 31 or EventID= 34 or EventID= 35]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);
	vector<myStruct::myLogStructure>partial1;
	if (numberOfAvailableEvents != 0)
	{
		//localFirst = myLogStructures[0]->toLogString();
		partial1 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	}
	int partialNumberOfEvents1 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>partial2;
	lpcwstrLogType = L"Setup";
	//string strSecurityLevelConstraint = "";
	string_query = "";
	string_query = string_query + "*[System[EventID= 1009]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);
	//localLast = myLogStructures[numberOfAvailableEvents - 1]->toLogString();
	partial2 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	int partialNumberOfEvents2 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>logsListToReturn;
	logsListToReturn.reserve(partial1.size() + partial2.size());
	logsListToReturn.insert(logsListToReturn.end(), partial1.begin(), partial1.end());
	logsListToReturn.insert(logsListToReturn.end(), partial2.begin(), partial2.end());

	numberOfAvailableEvents = numberOfAvailableEvents + partialNumberOfEvents1 + partialNumberOfEvents2;
	return logsListToReturn;
}

vector<myStruct::myLogStructure> MyLogRetriever::applicationCrashes(int timePeriodInMilliSeconds1, int summarizationLevel, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"System";
	//string strSecurityLevelConstraint = "";
	string string_query = "";
	string_query = string_query + "*[System[EventID= 1001]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);
	vector<myStruct::myLogStructure>partial1;
	if (numberOfAvailableEvents != 0)
	{
		partial1 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	}
	int partialNumberOfEvents1 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>partial2;
	lpcwstrLogType = L"Application";
	//string strSecurityLevelConstraint = "";
	string_query = "";
	string_query = string_query + "*[System[EventID= 1 or EventID= 2 or EventID= 1000 or EventID= 1001 or EventID= 1002]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);
	partial2 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	int partialNumberOfEvents2 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>logsListToReturn;
	logsListToReturn.reserve(partial1.size() + partial2.size());
	logsListToReturn.insert(logsListToReturn.end(), partial1.begin(), partial1.end());
	logsListToReturn.insert(logsListToReturn.end(), partial2.begin(), partial2.end());

	numberOfAvailableEvents = partialNumberOfEvents1 + partialNumberOfEvents2;
	return logsListToReturn;
}

vector<myStruct::myLogStructure> MyLogRetriever::softwareAndServicesInstallation(int timePeriodInMilliSeconds1, int summarizationLevel, int totalTimePeriodInMillisecond1)
{
	// valid for 23 events only
	numberOfAvailableEvents = 0;
	totalTimePeriodInMillisecond = totalTimePeriodInMillisecond1;
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	isLevelConstraintAvailable = false;
	isProcessIDConstraintAvailable = false;
	lpcwstrLogType = L"System";
	//string strSecurityLevelConstraint = "";
	string string_query = "";
	string_query = string_query + "*[System[EventID= 6 or EventID= 19 or EventID= 7045]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);
	vector<myStruct::myLogStructure>partial1;
	if (numberOfAvailableEvents != 0)
	{
		partial1 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	}
	int partialNumberOfEvents1 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myLogStructure>partial2;
	lpcwstrLogType = L"Application";
	//string strSecurityLevelConstraint = "";
	string_query = "";
	string_query = string_query + "*[System[EventID= 1022 or EventID= 1033 or EventID= 903 or EventID= 904 or EventID= 905 or EventID= 906 or EventID= 907 or EventID= 908 or EventID= 800]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);
	if (numberOfAvailableEvents != 0)
	{
		//localFirst = myLogStructures[0]->toLogString();
		partial2 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	}
	int partialNumberOfEvents2 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>partial3;
	lpcwstrLogType = L"Setup";
	//string strSecurityLevelConstraint = "";
	string_query = "";
	string_query = string_query + "*[System[EventID= 2]"
		+ "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();
	getEvents(lpcwstrLogType, pwsQuery);

	partial3 = returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	int partialNumberOfEvents3 = numberOfAvailableEvents;
	numberOfAvailableEvents = 0;

	vector<myStruct::myLogStructure>logsListToReturn;
	logsListToReturn.reserve(partial1.size() + partial2.size() + partial3.size());
	logsListToReturn.insert(logsListToReturn.end(), partial1.begin(), partial1.end());
	logsListToReturn.insert(logsListToReturn.end(), partial2.begin(), partial2.end());
	logsListToReturn.insert(logsListToReturn.end(), partial3.begin(), partial3.end());

	numberOfAvailableEvents = partialNumberOfEvents1 + partialNumberOfEvents2 + partialNumberOfEvents3;
	return logsListToReturn;
}

vector<myStruct::myLogStructure>MyLogRetriever::getRemoteLoginEvents(int timePeriodInMilliSeconds1, int summarizationLevel)
{
	timePeriodInMilliSeconds = timePeriodInMilliSeconds1;
	numberOfAvailableEvents = 0;
	wstring wsLogType = stringToWidestring("Security");
	lpcwstrLogType = wsLogType.c_str();

	string string_query = "";
	string_query = string_query + "*[System[EventID= 4624 or EventID= 4634]" + "/ TimeCreated[timediff(@SystemTime) <= " + to_string(timePeriodInMilliSeconds) + "]]";
	//cout << string_query << endl;
	// need to split and check against logon type 10
	wstring wsCons = stringToWidestring(string_query);
	pwsQuery = wsCons.c_str();

	getEvents(lpcwstrLogType, pwsQuery, 10);
	//printResultedEvent(myLogStructures, numberOfAvailableEvents);
	return returnResultedEventWithStruct(myLogStructures, numberOfAvailableEvents, summarizationLevel);
	//numberOfAvailableEvents = 0;
}

vector<myStruct::myLogStructure> MyLogRetriever::applicationWhitelisting(int timePeriodInMilliSeconds1, int totalTimePeriodInMillisecond1) // need to implement
{
	vector<myLogStructure>logsListPartial2;
	return logsListPartial2;
}

void MyLogRetriever::printResultedEvent(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents)
{
	for (int i = 0; i < numberOfFilteredEvents; i++)
	{
		myResultedLogStructures[i]->print();
		//std::cout << "*******************************************************************************\n";
		std::cout << "\n\n\n";
	}
}

/**
summarization MyLogRetriever::getSummarization()
{
	summarization summary;
	summary.count = numberOfAvailableEvents; //totalNumberOfEvents;
	summary.sum = noOfBytesToBeSent; //noOfTotalBytesToBeSent;
	summary.average = numberOfAvailableEvents * 1000 / (float)timePeriodInMilliSeconds;// event rate = totalNumberOfEvents * 1000 / (float)totalTimePeriodInMillisecond;
	return summary;
}
*/

void MyLogRetriever::getEvents(LPCWSTR pwsPath, LPCWSTR pwsQuery, int logonType)
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hSubscription = NULL;
	HANDLE hEventLog = NULL;

	HANDLE aWaitHandles[2];
	DWORD dwWait = 0;
	DWORD dwLastRecordNumber = 0;

	//wprintf(L"++++++++++++++++++++: %s\n", pwsQuery);

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
			status = EnumerateResults(hSubscription, logonType);
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

vector<myStruct::myLogStructure> MyLogRetriever::returnResultedEventWithStruct(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents, int summarizationLevel)
{
	vector<myLogStructure>returnedResult;
	myLogStructure local_returnedResult;
	for (int i = 0; i < numberOfFilteredEvents; i++)
	{
		local_returnedResult = myResultedLogStructures[i]->toStruct(summarizationLevel,processName,eventCategory);
		returnedResult.push_back(local_returnedResult);
	}
	return returnedResult;
}

vector<myStruct::myLogStructure> MyLogRetriever::returnResultedEventWithStruct(MyLogStructure*myResultedLogStructures[], int numberOfFilteredEvents)
{
	vector<myLogStructure>returnedResult;
	myLogStructure local_returnedResult;
	for (int i = 0; i < numberOfFilteredEvents; i++)
	{
		local_returnedResult = myResultedLogStructures[i]->toStruct(0, processName, eventCategory); 
		returnedResult.push_back(local_returnedResult);
	}
	return returnedResult;
}

MyLogRetriever::~MyLogRetriever(void)
{
	/**
	delete lpcwstrLogType;
	delete pwsQuery;
	delete process_id;
	delete myLogStructures;
	for (int i = 0; i < numberOfAvailableEvents; i++)
	{
		myLogStructures[0]->releaseMemory();
	}
	*/
}