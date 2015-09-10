#include "stdafx.h"
#include "MyLogManager.h"

MyLogManager::MyLogManager()
{
}

void MyLogManager::getTimePeriodToObserve()
{
	cout << "Enter a time period in milliseconds to observe(Default: 10000): " << endl;
	cin >> timePeriodInMilliSeconds;
}

void MyLogManager::getLogType()
{
	cout << "Choose one log type: \n\t\t1. Security\n\t\t2. Application\n\t\t3. System\n\t\t4. Setup\n\t\t5. Operational" << endl;
	int intLogType;
	cin >> intLogType;
	switch (intLogType)
	{
	case 1: logType = MY_SECURITY;
		break;
	case 2: logType = MY_APPLICATION;
		break;
	case 3: logType = MY_SYSTEM;
		break;
	case 4: logType = MY_SETUP;
		break;
	case 5: logType = MY_OPERATIONAL;
		break;
	default:
		cout << "Invalid Option\n!!";
		getLogType();
	}
}

void MyLogManager::getProcessID()
{
	cout << "Enter the process id: " << endl;
	cin >> process_id;
}

void MyLogManager::getSecurityConstraint()
{
	cout << "Select the constraint:\n\t0. Less than \n\t1. Less than or equal to \n\t2. Greater than " <<
		"\n\t3. Greater than or equal to\n\t4. Equal to\n\t" << endl;
	cin >> securityLevelConstraint;
	cout << "Enter the security level:" << endl;
	cin >> securityLevel;
}

void MyLogManager::getLogsForAllProcesses()
{
	getLogType();
	getTimePeriodToObserve();
	MyLogRetriever myLogRetriever;
	process_id = -1;
	securityLevelConstraint = ALL;
	securityLevel = 0;
	myLogRetriever.handleLogs(logType, securityLevelConstraint,securityLevel,process_id,timePeriodInMilliSeconds);
}

void MyLogManager::getLogsForAProcess()
{
	getLogType();
	getTimePeriodToObserve();
	getProcessID();
	MyLogRetriever myLogRetriever;
	securityLevelConstraint = ALL;
	securityLevel = 0;
	myLogRetriever.handleLogs(logType, securityLevelConstraint, securityLevel, process_id, timePeriodInMilliSeconds);
}

void MyLogManager::getLogsForAllProcessesWithSecurityConstraint()
{
	getLogType();
	getTimePeriodToObserve();
	getSecurityConstraint();
	MyLogRetriever myLogRetriever;
	process_id = -1;
	myLogRetriever.handleLogs(logType, securityLevelConstraint, securityLevel, process_id, timePeriodInMilliSeconds);
}

void MyLogManager::getLogsForAProcessWithSecurityConstraint()
{
	getLogType();
	getTimePeriodToObserve();
	getSecurityConstraint();
	MyLogRetriever myLogRetriever;
	getProcessID();
	myLogRetriever.handleLogs(logType, securityLevelConstraint, securityLevel, process_id, timePeriodInMilliSeconds);
}

void MyLogManager::getLogonFailures()
{
	MyLogRetriever myLogRetriever;
	LPCWSTR pwsQuery = L"*";
	DWORD eventIDs[] = { 4625, 4648, 537 };
	myLogRetriever.handleEventsOnEventIDs("Security", pwsQuery, eventIDs);
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
}

void MyLogManager::getCurrentLoggedInUser()
{
	MyUserAccountDetails details;
	details.getCurrentLoggedOnUserInformation();
}

void MyLogManager::getAllUserInformation()
{
	MyUserAccountDetails details;
	details.getAllUserInformation();
}

MyLogManager::~MyLogManager(void)
{

}