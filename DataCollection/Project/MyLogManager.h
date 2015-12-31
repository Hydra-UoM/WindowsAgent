#include "stdafx.h"
#ifndef MyLogManager_H
#define MyLogManager_H

#include <Iphlpapi.h>
#include <Assert.h>

#include "MyLogRetriever.h"
#include "MyUserAccountDetails.h"
#include <thread>
#include "MyLogStruct.h"
#include <stdint.h>
#include "RegisterDeviceService.h"
#include <fstream>
#include <wininet.h>
#include "MyDBHandler.h"

#pragma comment(lib, "iphlpapi.lib")

class MyLogManager
{
public:
	MyLogManager();
	~MyLogManager(void);
	static void stopExecution();
	static void getImportantLogEventsWithoutSummarization(int16_t timeInMinute);// bandwidth low && resource utilization high
	static void getImportantLogEvents(int16_t timeInMinute);// bandwidth low && resource utilization low
	static void getFullLogInformation(int16_t timeInMinute);// bandwidth high && resource utilization high
	static void getLogRelatedInformation(int16_t timeInMinute, int16_t summarizationLevel, std::vector<string> eventIndices, string logType, string process_name, string securityLevel);
	// bandwidth high && resource utilization low
	static bool isInternetConnectionAvailable();
	static string ConfigFile();
	static vector<HydraCN::myLogStructure> changeLogListFormatToBeSent(vector<myStruct::myLogStructure> logStructList, string eventCategory, string process_name);
	static HydraCN::myLogStructure changeLogFormatToBeSent(myStruct::myLogStructure logStruct, string eventCategory, string process_name);

	static vector<HydraCN::myUserAccountDetailsStruct> changeUserInfoListFormatToBeSent(vector<myStruct::myUserAccountDetailsStruct> userStructList);
	static HydraCN::myUserAccountDetailsStruct changeUserInfoFormatToBeSent(myStruct::myUserAccountDetailsStruct userStruct);
	static vector<myStruct::myLogStructure> getLogs(int timeGapInMilliSeconds, int64_t summarizationLevel, string logType, string process_name, string securityLevel);

	static vector<myStruct::myLogStructure> getLogsForAllProcesses(string logType, int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getLogsForAProcess(string logType, string process_name, int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getLogsForAllProcessesWithSecurityConstraint(string logType, string securityLevel, int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getLogsForAProcessWithSecurityConstraint(string logType, string securityLevel, string process_name, int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getLogonFailures(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getSuccessLoginInformation(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getFirewallEvents(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getAccountUsage(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getGroupPolicyEditors(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getWindowsDefenderEvents(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getMobileDeviceEvents(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getPrintingServicesEvents(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getSystemOrServiceFailures(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getClearingEventLogs(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getWindowsUpdateErrors(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getApplicationCrashes(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getSoftwareAndServicesInstallation(int timeGapInMilliSeconds, int summarizationLevel);
	static vector<myStruct::myLogStructure> getRemoteLoginEvents(int timeGapInMilliSeconds, int summarizationLevel);
	static myStruct::myUserAccountDetailsStruct getCurrentLoggedInUser(int summarizationLevel);
	static vector<myStruct::myUserAccountDetailsStruct> getAllUserInformation(int summarizationLevel);
	static void sendStoredData();
	static void storeImportantLogData();
	static string getMAC();
	static string getEventCategory(int eventIndex);
};
#endif