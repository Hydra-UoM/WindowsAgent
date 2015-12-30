#include "stdafx.h"
#include "RegisterDeviceService.h"
#include "MyLogManager.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using boost::shared_ptr;
using namespace HydraCN;

bool isAllowed;
bool isNotCompleted = true;

MyLogManager::MyLogManager()
{

}

MyLogManager::~MyLogManager(void)
{

}

void MyLogManager::getImportantLogEventsWithoutSummarization(int16_t timeInMinute)
{
	std::vector<string> eventIndices;
	eventIndices.push_back("17");// getLogonFailures
	eventIndices.push_back("18");// all logs
	//eventIndices.push_back("22");// getCurrentLoggedInUser
	//eventIndices.push_back("23");// getAllUserInformation
	//eventIndices.push_back("25");// getSuccessLoginInformation
	eventIndices.push_back("27");// getFirewallEvents
	eventIndices.push_back("28");// getAccountUsage
	eventIndices.push_back("29");// getGroupPolicyEditors
	eventIndices.push_back("30");// getWindowsDefenderEvents
	eventIndices.push_back("31");// getMobileDeviceEvents
	//eventIndices.push_back("32");// getPrintingServicesEvents
	eventIndices.push_back("33");// getSystemOrServiceFailures
	eventIndices.push_back("34");// getClearingEventLogs
	eventIndices.push_back("35");// getWindowsUpdateErrors
	eventIndices.push_back("36");// getApplicationCrashes
	eventIndices.push_back("37");// getSoftwareAndServicesInstallation
	eventIndices.push_back("38");// getRemoteLoginEvents
	getLogRelatedInformation(timeInMinute, 0, eventIndices, "", "", "Error & Critical");
}

void MyLogManager::getImportantLogEvents(int16_t timeInMinute)
{
	std::vector<string> eventIndices;
	eventIndices.push_back("17");// getLogonFailures
	eventIndices.push_back("18");// all logs
	//eventIndices.push_back("22");// getCurrentLoggedInUser
	//eventIndices.push_back("23");// getAllUserInformation
	//eventIndices.push_back("25");// getSuccessLoginInformation
	eventIndices.push_back("27");// getFirewallEvents
	eventIndices.push_back("28");// getAccountUsage
	eventIndices.push_back("29");// getGroupPolicyEditors
	eventIndices.push_back("30");// getWindowsDefenderEvents
	eventIndices.push_back("31");// getMobileDeviceEvents
	//eventIndices.push_back("32");// getPrintingServicesEvents
	eventIndices.push_back("33");// getSystemOrServiceFailures
	eventIndices.push_back("34");// getClearingEventLogs
	eventIndices.push_back("35");// getWindowsUpdateErrors
	eventIndices.push_back("36");// getApplicationCrashes
	eventIndices.push_back("37");// getSoftwareAndServicesInstallation
	eventIndices.push_back("38");// getRemoteLoginEvents
	getLogRelatedInformation(timeInMinute, 2, eventIndices, "", "", "Error & Critical");
}

void MyLogManager::getFullLogInformation(int16_t timeInMinute)
{
	std::vector<string> eventIndices;
	eventIndices.push_back("17");
	eventIndices.push_back("18");
	eventIndices.push_back("22");
	eventIndices.push_back("23");
	eventIndices.push_back("25");
	eventIndices.push_back("27");
	eventIndices.push_back("28");
	eventIndices.push_back("29");
	eventIndices.push_back("30");
	eventIndices.push_back("31");
	eventIndices.push_back("32");
	eventIndices.push_back("33");
	eventIndices.push_back("34");
	eventIndices.push_back("35");
	eventIndices.push_back("36");
	eventIndices.push_back("37");
	eventIndices.push_back("38");
	getLogRelatedInformation(timeInMinute, 0, eventIndices, "", "", "");
}

void MyLogManager::getLogRelatedInformation(int16_t timeInMinute, int16_t summarizationLevel, std::vector<string> eventIndices, string logType, string process_name, string securityLevel)
{
	string line;
	vector<string> fileRead;
	line = MyLogManager::ConfigFile();
	istringstream ss(line);
	string token;

	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}
	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));// 9091
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);

	vector<string>::iterator intIterator;

	myStruct::myUserAccountDetailsStruct getCurrentLoggedInUserInfo;						HydraCN::myUserAccountDetailsStruct getCurrentLoggedInUserInfoToBeSent;
	vector<myStruct::myUserAccountDetailsStruct> getAllUserInformationList;					vector<HydraCN::myUserAccountDetailsStruct> getAllUserInformationListToBeSent;
	vector<myStruct::myLogStructure> logList;												vector<HydraCN::myLogStructure> logListToBeSent;
	myStruct::myLogStructure tempLogStructure;												HydraCN::myLogStructure tempLogStructureToBeSent;

	myStruct::myUserAccountDetailsStruct getCurrentLoggedInUserInfoFromDB;						HydraCN::myUserAccountDetailsStruct getCurrentLoggedInUserInfoFromDBToBeSent;
	vector<myStruct::myUserAccountDetailsStruct> getAllUserInformationFromDBList;					vector<HydraCN::myUserAccountDetailsStruct> getAllUserInformationListFromDBToBeSent;
	vector<myStruct::myLogStructure> logFromDBList;												vector<HydraCN::myLogStructure> logFromDBListToBeSent;

	isAllowed = true;
	while (isAllowed)
	{
		try{
			//if (isInternetConnectionAvailable)
			//{
			transport->open();
			for (intIterator = eventIndices.begin(); intIterator != eventIndices.end(); ++intIterator)
			{
				if (isInternetConnectionAvailable && MyDBHandler::isAvailableLogData)
				{
					logFromDBList = MyDBHandler::retrieveLogData();
					logFromDBListToBeSent = changeLogListFormatToBeSent(logFromDBList);
					client.pushLogInfo(logFromDBListToBeSent);
					MyDBHandler::deleteLogData();
					cout << "PushedLogData" << endl;
				}
				if (isInternetConnectionAvailable && MyDBHandler::isAvailableUsersData)
				{
					getAllUserInformationFromDBList = MyDBHandler::retrieveUsersData();
					getAllUserInformationListFromDBToBeSent = changeUserInfoListFormatToBeSent(getAllUserInformationFromDBList);
					client.pushUsersInfo(getAllUserInformationListFromDBToBeSent);
					MyDBHandler::deleteUsersData();
					cout << "PushedUsersData" << endl;

					myStruct::myUserAccountDetailsStruct getCurrentLoggedInUserInfoFromDB;						HydraCN::myUserAccountDetailsStruct getCurrentLoggedInUserInfoFromDBToBeSent;
				}
				if (isInternetConnectionAvailable && MyDBHandler::isAvailableCurrentUserData)
				{
					//getCurrentLoggedInUserInfoFromDB = MyDBHandler::retrieveCurrentUserData();
					getCurrentLoggedInUserInfoFromDBToBeSent = changeUserInfoFormatToBeSent(getCurrentLoggedInUserInfoFromDB);
					client.pushCurrentUserInfo(getCurrentLoggedInUserInfoFromDBToBeSent);
					MyDBHandler::deleteCurrentUserData();
					cout << "PushedCurrentUserData" << endl;
				}
				switch (stoi(*intIterator))
				{
				case 18:
					logList = MyLogManager::getLogs(timeInMinute * 60000, summarizationLevel, logType, process_name, securityLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed18" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 17:
					logList = MyLogManager::getLogonFailures(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed17" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 25:
					logList = MyLogManager::getSuccessLoginInformation(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed25" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 27:
					logList = MyLogManager::getFirewallEvents(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed27" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 28:
					logList = MyLogManager::getAccountUsage(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed28" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 29:
					logList = MyLogManager::getGroupPolicyEditors(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed29" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 30:
					logList = MyLogManager::getWindowsDefenderEvents(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed30" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 31:
					logList = MyLogManager::getMobileDeviceEvents(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed31" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 32:
					logList = MyLogManager::getPrintingServicesEvents(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed32" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 33:
					logList = MyLogManager::getSystemOrServiceFailures(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed33" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 34:
					logList = MyLogManager::getClearingEventLogs(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed34" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 35:
					logList = MyLogManager::getWindowsUpdateErrors(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed35" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 36:
					logList = MyLogManager::getApplicationCrashes(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed36" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 37:
					logList = MyLogManager::getSoftwareAndServicesInstallation(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed37" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 38:
					logList = MyLogManager::getRemoteLoginEvents(timeInMinute * 60000, summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						logListToBeSent = changeLogListFormatToBeSent(logList);
						client.pushLogInfo(logListToBeSent);
						cout << "Pushed38" << endl;
					}
					else
					{
						MyDBHandler::createLogTable();
						MyDBHandler::storeLogData(logList);
					}
					break;
				case 22:
					getCurrentLoggedInUserInfo = MyLogManager::getCurrentLoggedInUser(summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						getCurrentLoggedInUserInfoToBeSent = changeUserInfoFormatToBeSent(getCurrentLoggedInUserInfo);
						client.pushCurrentUserInfo(getCurrentLoggedInUserInfoToBeSent);
						cout << "pushed22" << endl;
					}
					else
					{
						MyDBHandler::createCurrentUserTable();
						MyDBHandler::storeCurrentUserData(getCurrentLoggedInUserInfo);
					}
					break;
				case 23:
					getAllUserInformationList = MyLogManager::getAllUserInformation(summarizationLevel);
					if (isInternetConnectionAvailable)
					{
						getAllUserInformationListToBeSent = changeUserInfoListFormatToBeSent(getAllUserInformationList);
						client.pushUsersInfo(getAllUserInformationListToBeSent);
						cout << "pushed23" << endl;
					}
					else
					{
						MyDBHandler::createUsersTable();
						MyDBHandler::storeUsersData(getAllUserInformationList);
					}
					break;
				}
			}
			transport->close();
			//}
		}
		catch (TException& tx)
		{
			cout << "ERROR: " << tx.what() << endl;
		}
		Sleep(timeInMinute * 60000);
	}
	isNotCompleted = false;
}

void MyLogManager::stopExecution()
{
	isAllowed = false;
	while (isNotCompleted)
	{

	}
	isNotCompleted = true;
}

vector<myStruct::myLogStructure> MyLogManager::getLogs(int timeGapInMilliSeconds, int64_t summarizationLevel, string logType, string process_name, string securityLevel)
{
	MyLogRetriever myLogRetriever;
	if (logType.compare("") == 0) // need to send all logs
	{
		if (process_name.compare("") == 0) // need to send logs for all process
		{
			if (securityLevel.compare("") == 0) // without constraint
			{
				return myLogRetriever.getAllLogs("ALL", timeGapInMilliSeconds, summarizationLevel);
			}
			else // with constraint
			{
				return myLogRetriever.getAllLogs(securityLevel, timeGapInMilliSeconds, summarizationLevel);
			}
		}
		else // need to send all logs for a process
		{
			if (securityLevel.compare("") == 0) // without constraint
			{
				return myLogRetriever.getAllLogsForAProcess("ALL", timeGapInMilliSeconds, summarizationLevel, process_name);
			}
			else // with constraint
			{
				return myLogRetriever.getAllLogsForAProcess(securityLevel, timeGapInMilliSeconds, summarizationLevel, process_name);
			}
		}
	}
	else
	{
		if (process_name.compare("") == 0) // need to send logs for all process
		{
			if (securityLevel.compare("") == 0) // without constraint
			{
				return getLogsForAllProcesses(logType, timeGapInMilliSeconds, summarizationLevel);
			}
			else // with constraint
			{
				return getLogsForAllProcessesWithSecurityConstraint(logType, securityLevel, timeGapInMilliSeconds, summarizationLevel);
			}
		}
		else // need to send logs for a process
		{
			if (securityLevel.compare("") == 0) // without constraint
			{
				return getLogsForAProcess(logType, process_name, timeGapInMilliSeconds, summarizationLevel);
			}
			else // with constraint
			{
				return getLogsForAProcessWithSecurityConstraint(logType, securityLevel, process_name, timeGapInMilliSeconds, summarizationLevel);
			}
		}
	}
}

vector<myStruct::myLogStructure> MyLogManager::getLogsForAllProcesses(string logType, int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.handleLogRetrivalInfo(logType, "ALL", timeGapInMilliSeconds);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	/**
	int i = 0;
	for (iteratorStruct = logStructList.begin(); iteratorStruct != logStructList.end(); iteratorStruct++)
	{
	cout << i << ":" << (*iteratorStruct).EventID << endl;
	i++;
	}
	*/
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getLogsForAProcess(string logType, string process_name, int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.handleLogRetrivalInfo(logType, "ALL", process_name, timeGapInMilliSeconds);
	myLogRetriever.getSetOfProcessIDs(process_name);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery, myLogRetriever.process_id);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getLogsForAllProcessesWithSecurityConstraint(string logType, string securityLevel, int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.handleLogRetrivalInfo(logType, securityLevel, timeGapInMilliSeconds);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getLogsForAProcessWithSecurityConstraint(string logType, string securityLevel, string process_name, int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.handleLogRetrivalInfo(logType, securityLevel, timeGapInMilliSeconds);
	myLogRetriever.getSetOfProcessIDs(process_name);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery, myLogRetriever.process_id);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

myStruct::myUserAccountDetailsStruct MyLogManager::getCurrentLoggedInUser(int summarizationLevel) // need to release memory
{
	MyUserAccountDetails details;
	myStruct::myUserAccountDetailsStruct user;
	user = details.getCurrentLoggedOnUserInformation(summarizationLevel);
	//cout << user.computerName << endl;
	return user;
}

vector<myStruct::myUserAccountDetailsStruct> MyLogManager::getAllUserInformation(int summarizationLevel) // need to release memory
{
	MyUserAccountDetails details;
	//vector<myStruct::myUserAccountDetailsStruct>::iterator iteratorUStruct;
	vector<myStruct::myUserAccountDetailsStruct>userList;
	userList = details.getAllUserInformation(summarizationLevel);
	/**
	for (iteratorUStruct = userList.begin(); iteratorUStruct != userList.end(); ++iteratorUStruct)
	{
	cout << (*iteratorUStruct).computerName << endl;
	cout << (*iteratorUStruct).usri4_name << endl;
	}
	*/
	return userList;
}

vector<myStruct::myLogStructure> MyLogManager::getLogonFailures(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.handleFailedLoginEvents(timeGapInMilliSeconds, summarizationLevel);
	/**
	int i = 0;
	for (iteratorStruct = logStructList.begin(); iteratorStruct != logStructList.end(); iteratorStruct++)
	{
	cout << i << ":" << (*iteratorStruct).myProcessInformation1.Process_Name << endl;
	i++;
	}
	*/
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getSuccessLoginInformation(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	//vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.handleSuccessLoginEvents(timeGapInMilliSeconds, summarizationLevel);
	/**
	for (iteratorStruct = logStructList.begin(); iteratorStruct != logStructList.end(); ++iteratorStruct)
	{
	cout << (*iteratorStruct).mySubject1.Account_Domain << endl;
	cout << (*iteratorStruct).channelMessageString << endl;
	cout << (*iteratorStruct).message << endl;
	}
	*/
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getFirewallEvents(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.handleFirewallEvents(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getAccountUsage(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.handleAccountUsage(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getGroupPolicyEditors(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.groupPolicyEditorsEvents(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getWindowsDefenderEvents(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.windowsDefenderEvents(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getMobileDeviceEvents(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.mobileDeviceEvents(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getPrintingServicesEvents(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.printingServicesEvents(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getSystemOrServiceFailures(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	myLogRetriever.systemOrServiceFailures(timeGapInMilliSeconds, 50000);
	myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery);
	logStructList = myLogRetriever.returnResultedEventWithStruct(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getClearingEventLogs(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.clearingEventLogs(timeGapInMilliSeconds, summarizationLevel, 50000);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getWindowsUpdateErrors(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.windowsUpdateErrors(timeGapInMilliSeconds, summarizationLevel, 50000);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getApplicationCrashes(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.applicationCrashes(timeGapInMilliSeconds, summarizationLevel, 50000);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getSoftwareAndServicesInstallation(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.softwareAndServicesInstallation(timeGapInMilliSeconds, summarizationLevel, 50000);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<myStruct::myLogStructure> MyLogManager::getRemoteLoginEvents(int timeGapInMilliSeconds, int summarizationLevel)
{
	MyLogRetriever myLogRetriever;
	vector<myStruct::myLogStructure>logStructList;
	vector<myStruct::myLogStructure>::iterator iteratorStruct;
	logStructList = myLogRetriever.getRemoteLoginEvents(timeGapInMilliSeconds, summarizationLevel);
	myLogRetriever.releaseMemory();
	return logStructList;
}

vector<HydraCN::myUserAccountDetailsStruct> MyLogManager::changeUserInfoListFormatToBeSent(vector<myStruct::myUserAccountDetailsStruct> userStructList)
{
	vector<myStruct::myUserAccountDetailsStruct>::iterator userDetailsIterator;
	myStruct::myUserAccountDetailsStruct userStruct;		HydraCN::myUserAccountDetailsStruct UserInfoToBeSent;
	vector<HydraCN::myUserAccountDetailsStruct>userStructListToBeSent;
	for (userDetailsIterator = userStructList.begin(); userDetailsIterator != userStructList.end(); userDetailsIterator++)
	{
		userStruct = (*userDetailsIterator);
		UserInfoToBeSent = changeUserInfoFormatToBeSent(userStruct);
		userStructListToBeSent.push_back(UserInfoToBeSent);
	}
	return userStructListToBeSent;
}
vector<HydraCN::myLogStructure> MyLogManager::changeLogListFormatToBeSent(vector<myStruct::myLogStructure> logStructList)
{
	vector<myStruct::myLogStructure>::iterator logIterator;
	myStruct::myLogStructure tempLogStructure;												HydraCN::myLogStructure tempLogStructureToBeSent;
	vector<HydraCN::myLogStructure> logListToBeSent;
	for (logIterator = logStructList.begin(); logIterator != logStructList.end(); logIterator++)
	{
		tempLogStructure = (*logIterator);
		tempLogStructureToBeSent = changeLogFormatToBeSent(tempLogStructure);
		logListToBeSent.push_back(tempLogStructureToBeSent);
	}
	return logListToBeSent;
}

HydraCN::myUserAccountDetailsStruct MyLogManager::changeUserInfoFormatToBeSent(myStruct::myUserAccountDetailsStruct userStruct)
{
	HydraCN::myUserAccountDetailsStruct UserInfoToBeSent;
	UserInfoToBeSent.computerName = userStruct.computerName;
	UserInfoToBeSent.usri4_name = userStruct.usri4_name;
	UserInfoToBeSent.usri4_password_age = userStruct.usri4_password_age;
	UserInfoToBeSent.usri4_priv = userStruct.usri4_priv;
	UserInfoToBeSent.usri4_last_logoff = userStruct.usri4_last_logoff;
	UserInfoToBeSent.usri4_last_logon = userStruct.usri4_last_logon;
	UserInfoToBeSent.usri4_acct_expires = userStruct.usri4_acct_expires;
	UserInfoToBeSent.usri4_max_storage = userStruct.usri4_max_storage;
	UserInfoToBeSent.usri4_units_per_week = userStruct.usri4_units_per_week;
	UserInfoToBeSent.usri4_logon_hours = userStruct.usri4_logon_hours;
	UserInfoToBeSent.usri4_bad_pw_count = userStruct.usri4_bad_pw_count;
	UserInfoToBeSent.usri4_num_logons = userStruct.usri4_num_logons;
	UserInfoToBeSent.usri4_country_code = userStruct.usri4_country_code;
	UserInfoToBeSent.usri4_code_page = userStruct.usri4_code_page;
	UserInfoToBeSent.usri4_primary_group_id = userStruct.usri4_primary_group_id;
	UserInfoToBeSent.usri4_profile = userStruct.usri4_profile;
	UserInfoToBeSent.usri4_password_expired = userStruct.usri4_password_expired;
	UserInfoToBeSent.usri4_auth_flags = userStruct.usri4_auth_flags;
	return UserInfoToBeSent;
}

HydraCN::myLogStructure MyLogManager::changeLogFormatToBeSent(myStruct::myLogStructure logStruct)
{
	HydraCN::myLogStructure tempLogStructureToBeSent;
	tempLogStructureToBeSent.message = logStruct.message;
	tempLogStructureToBeSent.levelMessageString = logStruct.levelMessageString;
	tempLogStructureToBeSent.taskMessageString = logStruct.taskMessageString;
	tempLogStructureToBeSent.opCodeMessageString = logStruct.opCodeMessageString;
	tempLogStructureToBeSent.channelMessageString = logStruct.channelMessageString;
	tempLogStructureToBeSent.providerMessageString = logStruct.providerMessageString;
	//tempLogStructureToBeSent.version = logStruct.version;
	//tempLogStructureToBeSent.level = logStruct.level;
	//tempLogStructureToBeSent.task = logStruct.task;
	//tempLogStructureToBeSent.opCode = logStruct.opCode;
	//tempLogStructureToBeSent.keywords = logStruct.keywords;
	//tempLogStructureToBeSent.eventRecordID = logStruct.eventRecordID;
	//tempLogStructureToBeSent.executionProcessID = logStruct.executionProcessID;
	//tempLogStructureToBeSent.executionThreadID = logStruct.executionThreadID;
	//tempLogStructureToBeSent.channel = logStruct.channel;
	tempLogStructureToBeSent.computer = logStruct.computer;
	tempLogStructureToBeSent.EventID = logStruct.EventID;

	myStruct::mySubject tempMySubject;														HydraCN::mySubject tempMySubjectToBeSent;
	myStruct::myProviderInformation tempMyProviderInformation;								HydraCN::myProviderInformation tempMyProviderInformationToBeSent;
	myStruct::myObject tempMyObject;														HydraCN::myObject tempMyObjectToBeSent;
	myStruct::myNetworkInformation tempMyNetworkInformation;								HydraCN::myNetworkInformation tempMyNetworkInformationToBeSent;
	myStruct::myLayerInformation tempMyLayerInformation;									HydraCN::myLayerInformation tempMyLayerInformationToBeSent;
	myStruct::myFilterInformation tempMyFilterInformation;									HydraCN::myFilterInformation tempMyFilterInformationToBeSent;
	myStruct::myChangeInformation tempMyChangeInformation;									HydraCN::myChangeInformation tempMyChangeInformationToBeSent;
	myStruct::myCalloutInformation tempMyCalloutInformation;								HydraCN::myCalloutInformation tempMyCalloutInformationToBeSent;
	myStruct::myApplicationInformation tempMyApplicationInformation;						HydraCN::myApplicationInformation tempMyApplicationInformationToBeSent;
	myStruct::myAccessRequestInformation tempMyAccessRequestInformation;					HydraCN::myAccessRequestInformation tempMyAccessRequestInformationToBeSent;
	myStruct::myRuleInformation tempMyRuleInformation;										HydraCN::myRuleInformation tempMyRuleInformationToBeSent;
	myStruct::myErrorInformation tempMyErrorInformation;									HydraCN::myErrorInformation tempMyErrorInformationToBeSent;
	myStruct::myProcessInformation tempMyProcessInformation;								HydraCN::myProcessInformation tempMyProcessInformationToBeSent;
	myStruct::myLogonType tempMyLogonType;													HydraCN::myLogonType tempMyLogonTypeToBeSent;
	myStruct::myImpersonationLevel tempMyImpersonationLevel;								HydraCN::myImpersonationLevel tempMyImpersonationLevelToBeSent;
	myStruct::myAccountForWhichLogonFailed tempMyAccountForWhichLogonFailed;				HydraCN::myAccountForWhichLogonFailed tempMyAccountForWhichLogonFailedToBeSent;
	myStruct::myFailureInformation tempMyFailureInformation;								HydraCN::myFailureInformation tempMyFailureInformationToBeSent;
	myStruct::myNewLogon tempMyNewLogon;													HydraCN::myNewLogon tempMyNewLogonToBeSent;
	myStruct::myDetailedAuthenticationInformation tempMyDetailedAuthenticationInformation;	HydraCN::myDetailedAuthenticationInformation tempMyDetailedAuthenticationInformationToBeSent;
	myStruct::myTimeStamp tempMyTimeStamp;													HydraCN::myTimeStamp tempMyTimeStampToBeSent;

	tempMySubject = logStruct.mySubject1;
	tempMySubjectToBeSent.Security_ID = tempMySubject.Security_ID;
	tempMySubjectToBeSent.Account_Name = tempMySubject.Account_Name;
	tempMySubjectToBeSent.Account_Domain = tempMySubject.Account_Domain;
	tempMySubjectToBeSent.Logon_ID = tempMySubject.Logon_ID;

	tempMyProviderInformation = logStruct.myProviderInformation1;
	tempMyProviderInformationToBeSent.Provider_ID = tempMyProviderInformation.Provider_ID;
	tempMyProviderInformationToBeSent.Provider_Name = tempMyProviderInformation.Provider_Name;

	tempMyObject = logStruct.myObject1;
	tempMyObjectToBeSent.Object_Server = tempMyObject.Object_Server;
	tempMyObjectToBeSent.Object_Type = tempMyObject.Object_Type;
	tempMyObjectToBeSent.Object_Name = tempMyObject.Object_Name;
	tempMyObjectToBeSent.Handle_ID = tempMyObject.Handle_ID;
	tempMyObjectToBeSent.Resource_Attributes = tempMyObject.Resource_Attributes;

	tempMyNetworkInformation = logStruct.myNetworkInformation1;
	tempMyNetworkInformationToBeSent.Direction = tempMyNetworkInformation.Direction;
	tempMyNetworkInformationToBeSent.Source_Address = tempMyNetworkInformation.Source_Address;
	tempMyNetworkInformationToBeSent.Source_Port = tempMyNetworkInformation.Source_Port;
	tempMyNetworkInformationToBeSent.Destination_Address = tempMyNetworkInformation.Destination_Address;
	tempMyNetworkInformationToBeSent.Destination_Port = tempMyNetworkInformation.Destination_Port;
	tempMyNetworkInformationToBeSent.Protocol = tempMyNetworkInformation.Protocol;
	tempMyNetworkInformationToBeSent.Workstation_Name = tempMyNetworkInformation.Workstation_Name;

	tempMyLayerInformation = logStruct.myLayerInformation1;
	tempMyLayerInformationToBeSent.Layer_ID = tempMyLayerInformation.Layer_ID;
	tempMyLayerInformationToBeSent.Layer_Name = tempMyLayerInformation.Layer_Name;
	tempMyLayerInformationToBeSent.Layer_Runtime_ID = tempMyLayerInformation.Layer_Runtime_ID;

	tempMyFilterInformation = logStruct.myFilterInformation1;
	tempMyFilterInformationToBeSent.Filter_ID = tempMyFilterInformation.Filter_ID;
	tempMyFilterInformationToBeSent.Filter_Name = tempMyFilterInformation.Filter_Name;
	tempMyFilterInformationToBeSent.Filter_Type = tempMyFilterInformation.Filter_Type;
	tempMyFilterInformationToBeSent.Runtime_ID = tempMyFilterInformation.Runtime_ID;
	tempMyFilterInformationToBeSent.Filter_Runtime_ID = tempMyFilterInformation.Filter_Runtime_ID;
	tempMyFilterInformationToBeSent.Filter_Layer_Name = tempMyFilterInformation.Filter_Layer_Name;
	tempMyFilterInformationToBeSent.Filter_Layer_Runtime_ID = tempMyFilterInformation.Filter_Layer_Runtime_ID;

	tempMyChangeInformation = logStruct.myChangeInformation1;
	tempMyChangeInformationToBeSent.Change_Type = tempMyChangeInformation.Change_Type;

	tempMyCalloutInformation = logStruct.myCalloutInformation1;
	tempMyCalloutInformationToBeSent.Callout_ID = tempMyCalloutInformation.Callout_ID;
	tempMyCalloutInformationToBeSent.Callout_Name = tempMyCalloutInformation.Callout_Name;

	tempMyApplicationInformation = logStruct.myApplicationInformation1;
	tempMyApplicationInformationToBeSent.Application_Process_ID = tempMyApplicationInformation.Application_Process_ID;
	tempMyApplicationInformationToBeSent.Application_Name = tempMyApplicationInformation.Application_Name;

	tempMyAccessRequestInformation = logStruct.myAccessRequestInformation1;
	tempMyAccessRequestInformationToBeSent.Transaction_ID = tempMyAccessRequestInformation.Transaction_ID;
	tempMyAccessRequestInformationToBeSent.Accesses = tempMyAccessRequestInformation.Accesses;
	tempMyAccessRequestInformationToBeSent.Access_Reasons = tempMyAccessRequestInformation.Access_Reasons;
	tempMyAccessRequestInformationToBeSent.Access_Mask = tempMyAccessRequestInformation.Access_Mask;
	tempMyAccessRequestInformationToBeSent.Privileges_Used_For_Access_Check = tempMyAccessRequestInformation.Privileges_Used_For_Access_Check;
	tempMyAccessRequestInformationToBeSent.Restricted_SID_Count = tempMyAccessRequestInformation.Restricted_SID_Count;

	tempMyRuleInformation = logStruct.myRuleInformation1;
	tempMyRuleInformationToBeSent.ID = tempMyRuleInformation.ID;
	tempMyRuleInformationToBeSent.Name = tempMyRuleInformation.Name;

	tempMyErrorInformation = logStruct.myErrorInformation1;
	tempMyErrorInformationToBeSent.Reason = tempMyErrorInformation.Reason;

	tempMyProcessInformation = logStruct.myProcessInformation1;
	tempMyProcessInformationToBeSent.Process_ID = tempMyProcessInformation.Process_ID;
	tempMyProcessInformationToBeSent.Process_Name = tempMyProcessInformation.Process_Name;

	tempMyLogonType = logStruct.myLogonType1;
	tempMyLogonTypeToBeSent.Logon_Type = tempMyLogonType.Logon_Type;

	tempMyImpersonationLevel = logStruct.myImpersonationLevel1;
	tempMyImpersonationLevelToBeSent.Impersonation_Level = tempMyImpersonationLevel.Impersonation_Level;

	tempMyAccountForWhichLogonFailed = logStruct.myAccountForWhichLogonFailed1;
	tempMyAccountForWhichLogonFailedToBeSent.Security_ID = tempMyAccountForWhichLogonFailed.Security_ID;
	tempMyAccountForWhichLogonFailedToBeSent.Account_Name = tempMyAccountForWhichLogonFailed.Account_Name;
	tempMyAccountForWhichLogonFailedToBeSent.Account_Domain = tempMyAccountForWhichLogonFailed.Account_Domain;

	tempMyFailureInformation = logStruct.myFailureInformation1;
	tempMyFailureInformationToBeSent.Failure_Reason = tempMyFailureInformation.Failure_Reason;
	tempMyFailureInformationToBeSent.Status = tempMyFailureInformation.Status;
	tempMyFailureInformationToBeSent.Sub_Status = tempMyFailureInformation.Sub_Status;

	tempMyNewLogon = logStruct.myNewLogon1;
	tempMyNewLogonToBeSent.Security_ID = tempMyNewLogon.Security_ID;
	tempMyNewLogonToBeSent.Account_Name = tempMyNewLogon.Account_Name;
	tempMyNewLogonToBeSent.Account_Domain = tempMyNewLogon.Account_Domain;
	tempMyNewLogonToBeSent.Logon_ID = tempMyNewLogon.Logon_ID;
	tempMyNewLogonToBeSent.Logon_GUID = tempMyNewLogon.Logon_GUID;

	tempMyDetailedAuthenticationInformation = logStruct.myDetailedAuthenticationInformation1;
	tempMyDetailedAuthenticationInformationToBeSent.Logon_Process = tempMyDetailedAuthenticationInformation.Logon_Process;
	tempMyDetailedAuthenticationInformationToBeSent.Authentication_Package = tempMyDetailedAuthenticationInformation.Authentication_Package;
	tempMyDetailedAuthenticationInformationToBeSent.Transited_Services = tempMyDetailedAuthenticationInformation.Transited_Services;
	tempMyDetailedAuthenticationInformationToBeSent.Package_Name = tempMyDetailedAuthenticationInformation.Package_Name;
	tempMyDetailedAuthenticationInformationToBeSent.Key_Length = tempMyDetailedAuthenticationInformation.Key_Length;

	tempMyTimeStamp = logStruct.myTimeStamp1;
	tempMyTimeStampToBeSent.day = tempMyTimeStamp.day;
	tempMyTimeStampToBeSent.month = tempMyTimeStamp.month;
	tempMyTimeStampToBeSent.year = tempMyTimeStamp.year;
	tempMyTimeStampToBeSent.hour = tempMyTimeStamp.hour;
	tempMyTimeStampToBeSent.minute = tempMyTimeStamp.minute;
	tempMyTimeStampToBeSent.second = tempMyTimeStamp.second;

	tempLogStructureToBeSent.myTimeStamp1 = tempMyTimeStampToBeSent;
	tempLogStructureToBeSent.mySubject1 = tempMySubjectToBeSent;
	tempLogStructureToBeSent.myProviderInformation1 = tempMyProviderInformationToBeSent;
	tempLogStructureToBeSent.myObject1 = tempMyObjectToBeSent;
	tempLogStructureToBeSent.myNetworkInformation1 = tempMyNetworkInformationToBeSent;
	tempLogStructureToBeSent.myLayerInformation1 = tempMyLayerInformationToBeSent;
	tempLogStructureToBeSent.myFilterInformation1 = tempMyFilterInformationToBeSent;
	tempLogStructureToBeSent.myChangeInformation1 = tempMyChangeInformationToBeSent;
	tempLogStructureToBeSent.myCalloutInformation1 = tempMyCalloutInformationToBeSent;
	tempLogStructureToBeSent.myApplicationInformation1 = tempMyApplicationInformationToBeSent;
	tempLogStructureToBeSent.myAccessRequestInformation1 = tempMyAccessRequestInformationToBeSent;
	tempLogStructureToBeSent.myRuleInformation1 = tempMyRuleInformationToBeSent;
	tempLogStructureToBeSent.myErrorInformation1 = tempMyErrorInformationToBeSent;
	tempLogStructureToBeSent.myProcessInformation1 = tempMyProcessInformationToBeSent;
	tempLogStructureToBeSent.myLogonType1 = tempMyLogonTypeToBeSent;
	tempLogStructureToBeSent.myImpersonationLevel1 = tempMyImpersonationLevelToBeSent;
	tempLogStructureToBeSent.myAccountForWhichLogonFailed1 = tempMyAccountForWhichLogonFailedToBeSent;
	tempLogStructureToBeSent.myFailureInformation1 = tempMyFailureInformationToBeSent;
	tempLogStructureToBeSent.myNewLogon1 = tempMyNewLogonToBeSent;
	tempLogStructureToBeSent.myDetailedAuthenticationInformation1 = tempMyDetailedAuthenticationInformationToBeSent;
	return tempLogStructureToBeSent;
}

string MyLogManager::ConfigFile(){

	ifstream myfile("IP.txt");
	string ip;
	string group;
	string fileRead;
	int num1, num2;
	if (myfile.is_open())
	{
		getline(myfile, ip);
		ip += ",";
		while (myfile)
		{
			group += ip;
			getline(myfile, ip);

		}
		myfile.close();
		return group;

	}

	else cout << "Unable to open file";
}

bool MyLogManager::isInternetConnectionAvailable()
{
	if (InternetCheckConnection(L"http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}