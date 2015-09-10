#include "stdafx.h"
#ifndef MyLogManager_H
#define MyLogManager_H

#include "WindowsAgentConstants.h"
#include "MyLogRetriever.h"
#include "MyUserAccountDetails.h"

class MyLogManager
{
public:
	MyLogManager();

	~MyLogManager(void);

	void getLogsForAllProcesses();
	void getLogsForAProcess();
	void getLogsForAllProcessesWithSecurityConstraint();
	void getLogsForAProcessWithSecurityConstraint();
	void getLogType();
	void getProcessID();
	void getTimePeriodToObserve();
	void getSecurityConstraint();
	void getLogonFailures();
	void getCurrentLoggedInUser();
	void getAllUserInformation();


	std::string logType;
	int securityLevelConstraint;
	int securityLevel = 0;
	int timePeriodInMilliSeconds = 10000;
	int process_id = -1;
};
#endif