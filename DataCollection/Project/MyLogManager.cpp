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
	cout << "Select the constraint:\n\t0. Information \n\t1. Warning \n\t2. Error \n\t3. Critical " <<
		"\n\t4. Warning & Error & Critical\n\t5. Error & Critical \n\t" << endl;
	// 0 for demonstration
	cin >> securityLevelUser;
}

void MyLogManager::assignSecurityConstraint()
{
	/**cout << "Select the constraint:\n\t0. Less than \n\t1. Less than or equal to \n\t2. Greater than " <<
		"\n\t3. Greater than or equal to\n\t4. Equal to\n\t" << endl;*/
	//strSecurityLevelConstraint = "(Level = 0 or Level = 1 or Level = 2)";// 4 and 1 or 3 // ??
	switch(intLogType)
	{
		case 1: // Security log: 0 -Information, 1 -, 2 - Error, 3 –, 4 – Information
			switch (securityLevelUser)
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
			break;
		case 2: // 	Application Log: 0 - Information, 1 - Information, 2 - Error, 3 – Warning, 4 – Information
			switch (securityLevelUser)
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
			break;
		case 3: // 	System : 0 - , 1 - Critical, 2 - Error, 3 – Warning, 4 – Information
			switch (securityLevelUser)
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
			break;
		case 4: // setup
			switch (securityLevelUser)
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
			break;
		case 5: // operational
			switch (securityLevelUser)
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
			break;
	}
}

void MyLogManager::getLogsForAllProcessesWithSecurityConstraint()
{
	getLogType();
	getTimePeriodToObserve();
	getSecurityConstraint();
	assignSecurityConstraint();
	process_id = -1;
	/**
	MyLogRetriever myLogRetriever;
	myLogRetriever.handleLogRetrivalInfo(logType, strSecurityLevelConstraint, process_id, timePeriodInMilliSeconds);
	myLogRetriever.getLogs();
	*/
}

void MyLogManager::getLogsForAProcessWithSecurityConstraint()
{
	getLogType();
	getTimePeriodToObserve();
	getSecurityConstraint();
	assignSecurityConstraint();
	getProcessID();
	/**
	MyLogRetriever myLogRetriever;
	myLogRetriever.handleLogRetrivalInfo(logType, strSecurityLevelConstraint,process_id, timePeriodInMilliSeconds);
	myLogRetriever.getLogs();
	*/
}

void MyLogManager::getLogsForAllProcesses()
{
	getLogType();
	getTimePeriodToObserve();
	process_id = -1;
	strSecurityLevelConstraint = "ALL";
	securityLevel = 0;
	/*
	MyLogRetriever myLogRetriever;
	myLogRetriever.handleLogRetrivalInfo(logType, strSecurityLevelConstraint, process_id, timePeriodInMilliSeconds);
	myLogRetriever.getLogs();
	*/
}

void MyLogManager::getLogsForAProcess()
{
	getLogType();
	getTimePeriodToObserve();
	getProcessID();
	strSecurityLevelConstraint = "ALL";
	securityLevel = 0;
	/**
	MyLogRetriever myLogRetriever;
	myLogRetriever.handleLogRetrivalInfo(logType, strSecurityLevelConstraint, process_id, timePeriodInMilliSeconds);
	myLogRetriever.getLogs();
	*/
}

list<string>MyLogManager::getSuccessLoginInformation()
{
	MyLogRetriever myLogRetriever;
	// success login: 4624
	return myLogRetriever.handleSuccessLoginEvents();
}

list<string>MyLogManager::getLogonFailures()
{
	MyLogRetriever myLogRetriever;
	//Failed login: 4625, 4648, 537
	return myLogRetriever.handleFailedLoginEvents();
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

list<string>MyLogManager::getCurrentLoggedInUser()
{
	MyUserAccountDetails details;
	return details.getCurrentLoggedOnUserInformation();
}

list<string>MyLogManager::getAllUserInformation()
{
	MyUserAccountDetails details;
	return details.getAllUserInformation();
}

MyLogManager::~MyLogManager(void)
{

}