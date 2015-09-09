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

MyLogManager::~MyLogManager(void)
{

}