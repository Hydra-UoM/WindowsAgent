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
	void getSuccessLoginInformation();
	void assignSecurityConstraint();

	std::string logType;
	int intLogType;
	int securityLevelConstraint;
	std::string strSecurityLevelConstraint;
	int securityLevelUser;
	int securityLevel = 0;
	int timePeriodInMilliSeconds = 10000;
	int process_id = -1;
};
#endif