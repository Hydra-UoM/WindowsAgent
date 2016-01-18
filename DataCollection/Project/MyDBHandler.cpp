#include "stdafx.h"
#include "MyDBHandler.h"

sqlite3 *database;
char *myZErrMsg = 0;
bool  rc;
char*dbName_ = "logInfo.db";

bool MyDBHandler::isAvailableLogData = false;
bool MyDBHandler::isAvailableUsersData = false;
bool MyDBHandler::isAvailableCurrentUserData = false;

MyDBHandler::MyDBHandler()
{
}


MyDBHandler::~MyDBHandler()
{
}

// to create database connection
bool MyDBHandler::openDB(){
	return (sqlite3_open(dbName_, &database) == SQLITE_OK);
}

int MyDBHandler::query(char* qry)
{
	if (sqlite3_open(dbName_, &database) != SQLITE_OK) return 0;

	sqlite3_stmt *pointer;
	if (sqlite3_prepare_v2(database, qry, -1, &pointer, 0) == SQLITE_OK)
	{
		int result = 0;
		result = sqlite3_step(pointer);
		sqlite3_finalize(pointer);
		return result;
	}
	return 0;
	//sqlite3_close(database);

}

void MyDBHandler::close()
{
	sqlite3_close(database);
}

void MyDBHandler::createLogTable(){
	if (openDB())
	{
		std::string create_table_qry = "";
		create_table_qry = create_table_qry + "CREATE TABLE IF NOT EXISTS LogData(day INT,month INT, year INT, hour INT, minute INT, second INT, "
			+ "EventID TEXT, message TEXT, levelMessageString TEXT, taskMessageString TEXT, opCodeMessageString TEXT, channelMessageString TEXT, "
			+ "providerMessageString TEXT, keywords TEXT, executionProcessID TEXT, executionThreadID TEXT, computer TEXT, "
			+ "S_Security_ID TEXT, S_Account_Name TEXT, S_Account_Domain TEXT, S_Logon_ID TEXT, "
			+ "Provider_ID TEXT, Provider_Name TEXT, "
			+ "Object_Server TEXT, Object_Type TEXT, Object_Name TEXT, Handle_ID TEXT, Resource_Attributes TEXT, "
			+ "Direction TEXT, Source_Address TEXT, Source_Port TEXT, Destination_Address TEXT, Destination_Port TEXT, Protocol TEXT, Workstation_Name TEXT, "
			+ "Layer_ID TEXT, Layer_Name TEXT, Layer_Runtime_ID TEXT, "
			+ "Filter_ID TEXT, Filter_Name TEXT, Filter_Type TEXT, Runtime_ID TEXT, Filter_Runtime_ID TEXT, Filter_Layer_Name TEXT, Filter_Layer_Runtime_ID TEXT, "
			+ "Change_Type TEXT, "
			+ "Callout_ID TEXT, Callout_Name TEXT, "
			+ "Application_Process_ID TEXT, Application_Name TEXT, "
			+ "Transaction_ID TEXT, Accesses TEXT, Access_Reasons TEXT, Access_Mask TEXT, Privileges_Used_For_Access_Check TEXT, Restricted_SID_Count TEXT, "
			+ "ID TEXT, Name TEXT, "
			+ "Reason TEXT, "
			+ "Process_ID TEXT, Process_Name TEXT, "
			+ "Logon_Type TEXT, "
			+ "Impersonation_Level TEXT, "
			+ "AFL_Security_ID TEXT, AFL_Account_Name TEXT, AFL_Account_Domain TEXT, "
			+ "Failure_Reason TEXT, Status TEXT, Sub_Status TEXT, "
			+ "NL_Security_ID TEXT, NL_Account_Name TEXT, NL_Account_Domain TEXT, NL_Logon_ID TEXT, NL_Logon_GUID TEXT, "
			+ "Logon_Process TEXT, Authentication_Package TEXT, Transited_Services TEXT, Package_Name TEXT, Key_Length TEXT, "
			+ "eventCategory TEXT, processName TEXT)";

		rc = query((char*)create_table_qry.c_str());
		if (rc)
		{
			fprintf(stdout, "Table1 created successfully\n");
		}
		else
		{
			fprintf(stderr, "SQL error1: %s\n", myZErrMsg);
			sqlite3_free(myZErrMsg);
		}
		sqlite3_close(database);
	}
}

void MyDBHandler::createUsersTable(){
	if (openDB())
	{
		std::string create_table_qry = "";
		create_table_qry = create_table_qry + "CREATE TABLE IF NOT EXISTS UsersData(computerName TEXT NOT NULL,usri4_name TEXT,usri4_password_age TEXT,usri4_priv TEXT,"
			+ "usri4_flags TEXT, usri4_usr_comment TEXT, usri4_parms TEXT, usri4_last_logon TEXT, usri4_last_logoff TEXT, usri4_acct_expires TEXT, usri4_max_storage TEXT,"
			+ "usri4_units_per_week TEXT, usri4_logon_hours TEXT, usri4_bad_pw_count TEXT, usri4_num_logons TEXT, usri4_country_code TEXT, usri4_code_page TEXT,"
			+ "usri4_primary_group_id TEXT, usri4_profile TEXT, usri4_password_expired TEXT, usri4_auth_flags TEXT);";
		rc = query((char*)create_table_qry.c_str());
		if (rc)
		{
			fprintf(stdout, "Table2 created successfully\n");
		}
		else
		{
			fprintf(stderr, "SQL error2: %s\n", myZErrMsg);
			sqlite3_free(myZErrMsg);
		}
		sqlite3_close(database);
	}
}

void MyDBHandler::createCurrentUserTable(){
	if (openDB())
	{
		std::string create_table_qry = "";
		create_table_qry = create_table_qry + "CREATE TABLE IF NOT EXISTS CurrentUserData(computerName TEXT NOT NULL,usri4_name TEXT,usri4_password_age TEXT,usri4_priv TEXT,"
			+ "usri4_flags TEXT, usri4_usr_comment TEXT, usri4_parms TEXT, usri4_last_logon TEXT, usri4_last_logoff TEXT, usri4_acct_expires TEXT, usri4_max_storage TEXT,"
			+ "usri4_units_per_week TEXT, usri4_logon_hours TEXT, usri4_bad_pw_count TEXT, usri4_num_logons TEXT, usri4_country_code TEXT, usri4_code_page TEXT,"
			+ "usri4_primary_group_id TEXT, usri4_profile TEXT, usri4_password_expired TEXT, usri4_auth_flags TEXT);";
		rc = query((char*)create_table_qry.c_str());
		if (rc)
		{
			fprintf(stdout, "Table3 created successfully\n");
		}
		else
		{
			fprintf(stderr, "SQL error3: %s\n", myZErrMsg);
			sqlite3_free(myZErrMsg);
		}
		sqlite3_close(database);
	}
}

void MyDBHandler::storeLogData(std::vector<myStruct::myLogStructure>logList)
{
	std::vector<myStruct::myLogStructure>::iterator i;
	if (openDB())
	{
		std::string insert_qry = "";
		insert_qry = insert_qry + "INSERT INTO LogData VALUES(?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14,?15,?16,?17,?18,?19,?20,?21,?22,?23,?24,?25,"
			+ "?26,?27,?28,?29,?30,?31,?32,?33,?34,?35,?36,?37,?38,?39,?40,?41,?42,?43,?44,?45,?46,?47,?48,?49,?50,?51,?52,?53,?54,?55,?56,?57,?58,?59,?60,"
			+ "?61,?62,?63,?64,?65,?66,?67,?68,?69,?70,?71,?72,?73,?74,?75,?76,?77,?78,?79,?80,?81);";

		sqlite3_stmt *pointer;
		for (i = logList.begin(); i != logList.end(); ++i)
		{
			if (sqlite3_prepare_v2(database, insert_qry.c_str(), strlen(insert_qry.c_str()), &pointer, 0) == SQLITE_OK)
			{
				myStruct::myLogStructure tempLog = *i;
				sqlite3_bind_int(pointer, 1, tempLog.myTimeStamp1.day);
				sqlite3_bind_int(pointer, 2, tempLog.myTimeStamp1.month);
				sqlite3_bind_int(pointer, 3, tempLog.myTimeStamp1.year);
				sqlite3_bind_int(pointer, 4, tempLog.myTimeStamp1.hour);
				sqlite3_bind_int(pointer, 5, tempLog.myTimeStamp1.minute);
				sqlite3_bind_int(pointer, 6, tempLog.myTimeStamp1.second);

				sqlite3_bind_text(pointer, 7, tempLog.EventID.c_str(), strlen(tempLog.EventID.c_str()), 0);
				sqlite3_bind_text(pointer, 8, tempLog.message.c_str(), strlen(tempLog.message.c_str()), 0);
				sqlite3_bind_text(pointer, 9, tempLog.levelMessageString.c_str(), strlen(tempLog.levelMessageString.c_str()), 0);
				sqlite3_bind_text(pointer, 10, tempLog.taskMessageString.c_str(), strlen(tempLog.taskMessageString.c_str()), 0);
				sqlite3_bind_text(pointer, 11, tempLog.opCodeMessageString.c_str(), strlen(tempLog.opCodeMessageString.c_str()), 0);
				sqlite3_bind_text(pointer, 12, tempLog.channelMessageString.c_str(), strlen(tempLog.channelMessageString.c_str()), 0);
				sqlite3_bind_text(pointer, 13, tempLog.providerMessageString.c_str(), strlen(tempLog.providerMessageString.c_str()), 0);
				sqlite3_bind_text(pointer, 14, tempLog.keywords.c_str(), strlen(tempLog.keywords.c_str()), 0);
				sqlite3_bind_text(pointer, 15, tempLog.executionProcessID.c_str(), strlen(tempLog.executionProcessID.c_str()), 0);
				sqlite3_bind_text(pointer, 16, tempLog.executionThreadID.c_str(), strlen(tempLog.executionThreadID.c_str()), 0);
				sqlite3_bind_text(pointer, 17, tempLog.computer.c_str(), strlen(tempLog.computer.c_str()), 0);

				sqlite3_bind_text(pointer, 18, tempLog.mySubject1.Security_ID.c_str(), strlen(tempLog.mySubject1.Security_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 19, tempLog.mySubject1.Account_Name.c_str(), strlen(tempLog.mySubject1.Account_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 20, tempLog.mySubject1.Account_Domain.c_str(), strlen(tempLog.mySubject1.Account_Domain.c_str()), 0);
				sqlite3_bind_text(pointer, 21, tempLog.mySubject1.Logon_ID.c_str(), strlen(tempLog.mySubject1.Logon_ID.c_str()), 0);

				sqlite3_bind_text(pointer, 22, tempLog.myProviderInformation1.Provider_ID.c_str(), strlen(tempLog.myProviderInformation1.Provider_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 23, tempLog.myProviderInformation1.Provider_Name.c_str(), strlen(tempLog.myProviderInformation1.Provider_Name.c_str()), 0);

				sqlite3_bind_text(pointer, 24, tempLog.myObject1.Object_Server.c_str(), strlen(tempLog.myObject1.Object_Server.c_str()), 0);
				sqlite3_bind_text(pointer, 25, tempLog.myObject1.Object_Type.c_str(), strlen(tempLog.myObject1.Object_Type.c_str()), 0);
				sqlite3_bind_text(pointer, 26, tempLog.myObject1.Object_Name.c_str(), strlen(tempLog.myObject1.Object_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 27, tempLog.myObject1.Handle_ID.c_str(), strlen(tempLog.myObject1.Handle_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 28, tempLog.myObject1.Resource_Attributes.c_str(), strlen(tempLog.myObject1.Resource_Attributes.c_str()), 0);

				sqlite3_bind_text(pointer, 29, tempLog.myNetworkInformation1.Direction.c_str(), strlen(tempLog.myNetworkInformation1.Direction.c_str()), 0);
				sqlite3_bind_text(pointer, 30, tempLog.myNetworkInformation1.Source_Address.c_str(), strlen(tempLog.myNetworkInformation1.Source_Address.c_str()), 0);
				sqlite3_bind_text(pointer, 31, tempLog.myNetworkInformation1.Source_Port.c_str(), strlen(tempLog.myNetworkInformation1.Source_Port.c_str()), 0);
				sqlite3_bind_text(pointer, 32, tempLog.myNetworkInformation1.Destination_Address.c_str(), strlen(tempLog.myNetworkInformation1.Destination_Address.c_str()), 0);
				sqlite3_bind_text(pointer, 33, tempLog.myNetworkInformation1.Destination_Port.c_str(), strlen(tempLog.myNetworkInformation1.Destination_Port.c_str()), 0);
				sqlite3_bind_text(pointer, 34, tempLog.myNetworkInformation1.Protocol.c_str(), strlen(tempLog.myNetworkInformation1.Protocol.c_str()), 0);
				sqlite3_bind_text(pointer, 35, tempLog.myNetworkInformation1.Workstation_Name.c_str(), strlen(tempLog.myNetworkInformation1.Workstation_Name.c_str()), 0);

				sqlite3_bind_text(pointer, 36, tempLog.myLayerInformation1.Layer_ID.c_str(), strlen(tempLog.myLayerInformation1.Layer_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 37, tempLog.myLayerInformation1.Layer_Name.c_str(), strlen(tempLog.myLayerInformation1.Layer_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 38, tempLog.myLayerInformation1.Layer_Runtime_ID.c_str(), strlen(tempLog.myLayerInformation1.Layer_Runtime_ID.c_str()), 0);

				sqlite3_bind_text(pointer, 39, tempLog.myFilterInformation1.Filter_ID.c_str(), strlen(tempLog.myFilterInformation1.Filter_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 40, tempLog.myFilterInformation1.Filter_Name.c_str(), strlen(tempLog.myFilterInformation1.Filter_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 41, tempLog.myFilterInformation1.Filter_Type.c_str(), strlen(tempLog.myFilterInformation1.Filter_Type.c_str()), 0);
				sqlite3_bind_text(pointer, 42, tempLog.myFilterInformation1.Runtime_ID.c_str(), strlen(tempLog.myFilterInformation1.Runtime_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 43, tempLog.myFilterInformation1.Filter_Runtime_ID.c_str(), strlen(tempLog.myFilterInformation1.Filter_Runtime_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 44, tempLog.myFilterInformation1.Filter_Layer_Name.c_str(), strlen(tempLog.myFilterInformation1.Filter_Layer_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 45, tempLog.myFilterInformation1.Filter_Layer_Runtime_ID.c_str(), strlen(tempLog.myFilterInformation1.Filter_Layer_Runtime_ID.c_str()), 0);

				sqlite3_bind_text(pointer, 46, tempLog.myChangeInformation1.Change_Type.c_str(), strlen(tempLog.myChangeInformation1.Change_Type.c_str()), 0);

				sqlite3_bind_text(pointer, 47, tempLog.myCalloutInformation1.Callout_ID.c_str(), strlen(tempLog.myCalloutInformation1.Callout_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 48, tempLog.myCalloutInformation1.Callout_Name.c_str(), strlen(tempLog.myCalloutInformation1.Callout_Name.c_str()), 0);

				sqlite3_bind_text(pointer, 49, tempLog.myApplicationInformation1.Application_Process_ID.c_str(), strlen(tempLog.myApplicationInformation1.Application_Process_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 50, tempLog.myApplicationInformation1.Application_Name.c_str(), strlen(tempLog.myApplicationInformation1.Application_Name.c_str()), 0);

				sqlite3_bind_text(pointer, 51, tempLog.myAccessRequestInformation1.Transaction_ID.c_str(), strlen(tempLog.myAccessRequestInformation1.Transaction_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 52, tempLog.myAccessRequestInformation1.Accesses.c_str(), strlen(tempLog.myAccessRequestInformation1.Accesses.c_str()), 0);
				sqlite3_bind_text(pointer, 53, tempLog.myAccessRequestInformation1.Access_Reasons.c_str(), strlen(tempLog.myAccessRequestInformation1.Access_Reasons.c_str()), 0);
				sqlite3_bind_text(pointer, 54, tempLog.myAccessRequestInformation1.Access_Mask.c_str(), strlen(tempLog.myAccessRequestInformation1.Access_Mask.c_str()), 0);
				sqlite3_bind_text(pointer, 55, tempLog.myAccessRequestInformation1.Privileges_Used_For_Access_Check.c_str(), strlen(tempLog.myAccessRequestInformation1.Privileges_Used_For_Access_Check.c_str()), 0);
				sqlite3_bind_text(pointer, 56, tempLog.myAccessRequestInformation1.Restricted_SID_Count.c_str(), strlen(tempLog.myAccessRequestInformation1.Restricted_SID_Count.c_str()), 0);

				sqlite3_bind_text(pointer, 57, tempLog.myRuleInformation1.ID.c_str(), strlen(tempLog.myRuleInformation1.ID.c_str()), 0);
				sqlite3_bind_text(pointer, 58, tempLog.myRuleInformation1.Name.c_str(), strlen(tempLog.myRuleInformation1.Name.c_str()), 0);

				sqlite3_bind_text(pointer, 59, tempLog.myErrorInformation1.Reason.c_str(), strlen(tempLog.myErrorInformation1.Reason.c_str()), 0);

				sqlite3_bind_text(pointer, 60, tempLog.myProcessInformation1.Process_ID.c_str(), strlen(tempLog.myProcessInformation1.Process_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 61, tempLog.myProcessInformation1.Process_Name.c_str(), strlen(tempLog.myProcessInformation1.Process_Name.c_str()), 0);

				sqlite3_bind_text(pointer, 62, tempLog.myLogonType1.Logon_Type.c_str(), strlen(tempLog.myLogonType1.Logon_Type.c_str()), 0);

				sqlite3_bind_text(pointer, 63, tempLog.myImpersonationLevel1.Impersonation_Level.c_str(), strlen(tempLog.myImpersonationLevel1.Impersonation_Level.c_str()), 0);

				sqlite3_bind_text(pointer, 64, tempLog.myAccountForWhichLogonFailed1.Security_ID.c_str(), strlen(tempLog.myAccountForWhichLogonFailed1.Security_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 65, tempLog.myAccountForWhichLogonFailed1.Account_Name.c_str(), strlen(tempLog.myAccountForWhichLogonFailed1.Account_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 66, tempLog.myAccountForWhichLogonFailed1.Account_Domain.c_str(), strlen(tempLog.myAccountForWhichLogonFailed1.Account_Domain.c_str()), 0);

				sqlite3_bind_text(pointer, 67, tempLog.myFailureInformation1.Failure_Reason.c_str(), strlen(tempLog.myFailureInformation1.Failure_Reason.c_str()), 0);
				sqlite3_bind_text(pointer, 68, tempLog.myFailureInformation1.Status.c_str(), strlen(tempLog.myFailureInformation1.Status.c_str()), 0);
				sqlite3_bind_text(pointer, 69, tempLog.myFailureInformation1.Sub_Status.c_str(), strlen(tempLog.myFailureInformation1.Sub_Status.c_str()), 0);

				sqlite3_bind_text(pointer, 70, tempLog.myNewLogon1.Security_ID.c_str(), strlen(tempLog.myNewLogon1.Security_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 71, tempLog.myNewLogon1.Account_Name.c_str(), strlen(tempLog.myNewLogon1.Account_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 72, tempLog.myNewLogon1.Account_Domain.c_str(), strlen(tempLog.myNewLogon1.Account_Domain.c_str()), 0);
				sqlite3_bind_text(pointer, 73, tempLog.myNewLogon1.Logon_ID.c_str(), strlen(tempLog.myNewLogon1.Logon_ID.c_str()), 0);
				sqlite3_bind_text(pointer, 74, tempLog.myNewLogon1.Logon_GUID.c_str(), strlen(tempLog.myNewLogon1.Logon_GUID.c_str()), 0);

				sqlite3_bind_text(pointer, 75, tempLog.myDetailedAuthenticationInformation1.Logon_Process.c_str(), strlen(tempLog.myDetailedAuthenticationInformation1.Logon_Process.c_str()), 0);
				sqlite3_bind_text(pointer, 76, tempLog.myDetailedAuthenticationInformation1.Authentication_Package.c_str(), strlen(tempLog.myDetailedAuthenticationInformation1.Authentication_Package.c_str()), 0);
				sqlite3_bind_text(pointer, 77, tempLog.myDetailedAuthenticationInformation1.Transited_Services.c_str(), strlen(tempLog.myDetailedAuthenticationInformation1.Transited_Services.c_str()), 0);
				sqlite3_bind_text(pointer, 78, tempLog.myDetailedAuthenticationInformation1.Package_Name.c_str(), strlen(tempLog.myDetailedAuthenticationInformation1.Package_Name.c_str()), 0);
				sqlite3_bind_text(pointer, 79, tempLog.myDetailedAuthenticationInformation1.Key_Length.c_str(), strlen(tempLog.myDetailedAuthenticationInformation1.Key_Length.c_str()), 0);

				sqlite3_bind_text(pointer, 80, tempLog.eventCategory.c_str(), strlen(tempLog.eventCategory.c_str()), 0);
				sqlite3_bind_text(pointer, 81, tempLog.processName.c_str(), strlen(tempLog.processName.c_str()), 0);

				sqlite3_step(pointer);   // prepare statemnt Ready 
				sqlite3_reset(pointer);
				isAvailableLogData = true;
			}
			sqlite3_finalize(pointer);
		}
		sqlite3_close(database);
		printf("Data inserted successfully\n");
	}
}

void MyDBHandler::storeUsersData(std::vector<myStruct::myUserAccountDetailsStruct>usersList)
{
	std::vector<myStruct::myUserAccountDetailsStruct>::iterator i;
	if (openDB())
	{
		std::string insert_qry = "INSERT INTO UsersData VALUES(?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14,?15,?16,?17,?18,?19,?20,?21);";

		sqlite3_stmt *pointer;
		for (i = usersList.begin(); i != usersList.end(); ++i)
		{
			if (sqlite3_prepare_v2(database, insert_qry.c_str(), strlen(insert_qry.c_str()), &pointer, 0) == SQLITE_OK)
			{
				myStruct::myUserAccountDetailsStruct tempUser;
				tempUser = *i;
				sqlite3_bind_text(pointer, 1, tempUser.computerName.c_str(), strlen(tempUser.computerName.c_str()), 0);
				sqlite3_bind_text(pointer, 2, tempUser.usri4_name.c_str(), strlen(tempUser.usri4_name.c_str()), 0);
				sqlite3_bind_text(pointer, 3, tempUser.usri4_password_age.c_str(), strlen(tempUser.usri4_password_age.c_str()), 0);
				sqlite3_bind_text(pointer, 4, tempUser.usri4_priv.c_str(), strlen(tempUser.usri4_priv.c_str()), 0);
				sqlite3_bind_text(pointer, 5, tempUser.usri4_flags.c_str(), strlen(tempUser.usri4_flags.c_str()), 0);
				sqlite3_bind_text(pointer, 6, tempUser.usri4_usr_comment.c_str(), strlen(tempUser.usri4_usr_comment.c_str()), 0);
				sqlite3_bind_text(pointer, 7, tempUser.usri4_parms.c_str(), strlen(tempUser.usri4_parms.c_str()), 0);
				sqlite3_bind_text(pointer, 8, tempUser.usri4_last_logon.c_str(), strlen(tempUser.usri4_last_logon.c_str()), 0);
				sqlite3_bind_text(pointer, 9, tempUser.usri4_last_logoff.c_str(), strlen(tempUser.usri4_last_logoff.c_str()), 0);
				sqlite3_bind_text(pointer, 10, tempUser.usri4_acct_expires.c_str(), strlen(tempUser.usri4_acct_expires.c_str()), 0);
				sqlite3_bind_text(pointer, 11, tempUser.usri4_max_storage.c_str(), strlen(tempUser.usri4_max_storage.c_str()), 0);
				sqlite3_bind_text(pointer, 12, tempUser.usri4_units_per_week.c_str(), strlen(tempUser.usri4_units_per_week.c_str()), 0);
				sqlite3_bind_text(pointer, 13, tempUser.usri4_logon_hours.c_str(), strlen(tempUser.usri4_logon_hours.c_str()), 0);
				sqlite3_bind_text(pointer, 14, tempUser.usri4_bad_pw_count.c_str(), strlen(tempUser.usri4_bad_pw_count.c_str()), 0);
				sqlite3_bind_text(pointer, 15, tempUser.usri4_num_logons.c_str(), strlen(tempUser.usri4_num_logons.c_str()), 0);
				sqlite3_bind_text(pointer, 16, tempUser.usri4_country_code.c_str(), strlen(tempUser.usri4_country_code.c_str()), 0);
				sqlite3_bind_text(pointer, 17, tempUser.usri4_code_page.c_str(), strlen(tempUser.usri4_code_page.c_str()), 0);
				sqlite3_bind_text(pointer, 18, tempUser.usri4_primary_group_id.c_str(), strlen(tempUser.usri4_primary_group_id.c_str()), 0);
				sqlite3_bind_text(pointer, 19, tempUser.usri4_profile.c_str(), strlen(tempUser.usri4_profile.c_str()), 0);
				sqlite3_bind_text(pointer, 20, tempUser.usri4_password_expired.c_str(), strlen(tempUser.usri4_password_expired.c_str()), 0);
				sqlite3_bind_text(pointer, 21, tempUser.usri4_auth_flags.c_str(), strlen(tempUser.usri4_auth_flags.c_str()), 0);

				sqlite3_step(pointer);   // prepare statemnt Ready 
				sqlite3_reset(pointer);
				isAvailableUsersData = true;
			}
			sqlite3_finalize(pointer);
		}
		sqlite3_close(database);
		printf("Data inserted successfully\n");
	}
}

void MyDBHandler::storeCurrentUserData(myStruct::myUserAccountDetailsStruct currentUser)
{
	if (openDB())
	{
		std::string insert_qry = "INSERT INTO CurrentUserData VALUES(?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14,?15,?16,?17,?18,?19,?20,?21);";

		sqlite3_stmt *pointer;
		if (sqlite3_prepare_v2(database, insert_qry.c_str(), strlen(insert_qry.c_str()), &pointer, 0) == SQLITE_OK)
		{
			sqlite3_bind_text(pointer, 1, currentUser.computerName.c_str(), strlen(currentUser.computerName.c_str()), 0);
			sqlite3_bind_text(pointer, 2, currentUser.usri4_name.c_str(), strlen(currentUser.usri4_name.c_str()), 0);
			sqlite3_bind_text(pointer, 3, currentUser.usri4_password_age.c_str(), strlen(currentUser.usri4_password_age.c_str()), 0);
			sqlite3_bind_text(pointer, 4, currentUser.usri4_priv.c_str(), strlen(currentUser.usri4_priv.c_str()), 0);
			sqlite3_bind_text(pointer, 5, currentUser.usri4_flags.c_str(), strlen(currentUser.usri4_flags.c_str()), 0);
			sqlite3_bind_text(pointer, 6, currentUser.usri4_usr_comment.c_str(), strlen(currentUser.usri4_usr_comment.c_str()), 0);
			sqlite3_bind_text(pointer, 7, currentUser.usri4_parms.c_str(), strlen(currentUser.usri4_parms.c_str()), 0);
			sqlite3_bind_text(pointer, 8, currentUser.usri4_last_logon.c_str(), strlen(currentUser.usri4_last_logon.c_str()), 0);
			sqlite3_bind_text(pointer, 9, currentUser.usri4_last_logoff.c_str(), strlen(currentUser.usri4_last_logoff.c_str()), 0);
			sqlite3_bind_text(pointer, 10, currentUser.usri4_acct_expires.c_str(), strlen(currentUser.usri4_acct_expires.c_str()), 0);
			sqlite3_bind_text(pointer, 11, currentUser.usri4_max_storage.c_str(), strlen(currentUser.usri4_max_storage.c_str()), 0);
			sqlite3_bind_text(pointer, 12, currentUser.usri4_units_per_week.c_str(), strlen(currentUser.usri4_units_per_week.c_str()), 0);
			sqlite3_bind_text(pointer, 13, currentUser.usri4_logon_hours.c_str(), strlen(currentUser.usri4_logon_hours.c_str()), 0);
			sqlite3_bind_text(pointer, 14, currentUser.usri4_bad_pw_count.c_str(), strlen(currentUser.usri4_bad_pw_count.c_str()), 0);
			sqlite3_bind_text(pointer, 15, currentUser.usri4_num_logons.c_str(), strlen(currentUser.usri4_num_logons.c_str()), 0);
			sqlite3_bind_text(pointer, 16, currentUser.usri4_country_code.c_str(), strlen(currentUser.usri4_country_code.c_str()), 0);
			sqlite3_bind_text(pointer, 17, currentUser.usri4_code_page.c_str(), strlen(currentUser.usri4_code_page.c_str()), 0);
			sqlite3_bind_text(pointer, 18, currentUser.usri4_primary_group_id.c_str(), strlen(currentUser.usri4_primary_group_id.c_str()), 0);
			sqlite3_bind_text(pointer, 19, currentUser.usri4_profile.c_str(), strlen(currentUser.usri4_profile.c_str()), 0);
			sqlite3_bind_text(pointer, 20, currentUser.usri4_password_expired.c_str(), strlen(currentUser.usri4_password_expired.c_str()), 0);
			sqlite3_bind_text(pointer, 21, currentUser.usri4_auth_flags.c_str(), strlen(currentUser.usri4_auth_flags.c_str()), 0);

			sqlite3_step(pointer);   // prepare statemnt Ready 
			sqlite3_reset(pointer);
			isAvailableCurrentUserData = true;
		}
		printf("Data inserted successfully\n");
		sqlite3_finalize(pointer);
		sqlite3_close(database);
	}
}

myStruct::myUserAccountDetailsStruct MyDBHandler::retrieveCurrentUserData(){
	if (openDB())
	{
		std::string get_data_qry = "SELECT * FROM CurrentUserData";
		myStruct::myUserAccountDetailsStruct currentUser;
		sqlite3_stmt *pointer;

		if (sqlite3_prepare_v2(database, ((char*)get_data_qry.c_str()), -1, &pointer, 0) == SQLITE_OK)
		{
			int result = sqlite3_step(pointer);
			if(result == SQLITE_ROW)
			{
				currentUser.computerName = (char*)sqlite3_column_text(pointer, 0);
				currentUser.usri4_name = (char*)sqlite3_column_text(pointer, 1);
				currentUser.usri4_password_age = (char*)sqlite3_column_text(pointer, 2);
				currentUser.usri4_priv = (char*)sqlite3_column_text(pointer, 3);
				currentUser.usri4_flags = (char*)sqlite3_column_text(pointer, 4);
				currentUser.usri4_usr_comment = (char*)sqlite3_column_text(pointer, 5);
				currentUser.usri4_parms = (char*)sqlite3_column_text(pointer, 6);
				currentUser.usri4_last_logon = (char*)sqlite3_column_text(pointer, 7);
				currentUser.usri4_last_logoff = (char*)sqlite3_column_text(pointer, 8);
				currentUser.usri4_acct_expires = (char*)sqlite3_column_text(pointer, 9);
				currentUser.usri4_max_storage = (char*)sqlite3_column_text(pointer, 10);
				currentUser.usri4_units_per_week = (char*)sqlite3_column_text(pointer, 11);
				currentUser.usri4_logon_hours = (char*)sqlite3_column_text(pointer, 12);
				currentUser.usri4_bad_pw_count = (char*)sqlite3_column_text(pointer, 13);
				currentUser.usri4_num_logons = (char*)sqlite3_column_text(pointer, 14);
				currentUser.usri4_country_code = (char*)sqlite3_column_text(pointer, 15);
				currentUser.usri4_code_page = (char*)sqlite3_column_text(pointer, 16);
				currentUser.usri4_primary_group_id = (char*)sqlite3_column_text(pointer, 17);
				currentUser.usri4_profile = (char*)sqlite3_column_text(pointer, 18);
				currentUser.usri4_password_expired = (char*)sqlite3_column_text(pointer, 19);
				currentUser.usri4_auth_flags = (char*)sqlite3_column_text(pointer, 20);
				result = sqlite3_step(pointer);
			}
		}
		sqlite3_close(database);
		return currentUser;
	}
}

std::vector<myStruct::myUserAccountDetailsStruct>  MyDBHandler::retrieveUsersData(){
	if (openDB())
	{
		std::string get_data_qry = "SELECT * FROM UsersData";
		std::vector<myStruct::myUserAccountDetailsStruct>usersList;
		sqlite3_stmt *pointer;
		if (sqlite3_prepare_v2(database, ((char*)get_data_qry.c_str()), -1, &pointer, 0) == SQLITE_OK)
		{
			int result = sqlite3_step(pointer);
			while (result == SQLITE_ROW)
			{
				myStruct::myUserAccountDetailsStruct tempUser;
				tempUser.computerName = (char*)sqlite3_column_text(pointer, 0);
				tempUser.usri4_name = (char*)sqlite3_column_text(pointer, 1);
				tempUser.usri4_password_age = (char*)sqlite3_column_text(pointer, 2);
				tempUser.usri4_priv = (char*)sqlite3_column_text(pointer, 3);
				tempUser.usri4_flags = (char*)sqlite3_column_text(pointer, 4);
				tempUser.usri4_usr_comment = (char*)sqlite3_column_text(pointer, 5);
				tempUser.usri4_parms = (char*)sqlite3_column_text(pointer, 6);
				tempUser.usri4_last_logon = (char*)sqlite3_column_text(pointer, 7);
				tempUser.usri4_last_logoff = (char*)sqlite3_column_text(pointer, 8);
				tempUser.usri4_acct_expires = (char*)sqlite3_column_text(pointer, 9);
				tempUser.usri4_max_storage = (char*)sqlite3_column_text(pointer, 10);
				tempUser.usri4_units_per_week = (char*)sqlite3_column_text(pointer, 11);
				tempUser.usri4_logon_hours = (char*)sqlite3_column_text(pointer, 12);
				tempUser.usri4_bad_pw_count = (char*)sqlite3_column_text(pointer, 13);
				tempUser.usri4_num_logons = (char*)sqlite3_column_text(pointer, 14);
				tempUser.usri4_country_code = (char*)sqlite3_column_text(pointer, 15);
				tempUser.usri4_code_page = (char*)sqlite3_column_text(pointer, 16);
				tempUser.usri4_primary_group_id = (char*)sqlite3_column_text(pointer, 17);
				tempUser.usri4_profile = (char*)sqlite3_column_text(pointer, 18);
				tempUser.usri4_password_expired = (char*)sqlite3_column_text(pointer, 19);
				tempUser.usri4_auth_flags = (char*)sqlite3_column_text(pointer, 20);
				result = sqlite3_step(pointer);
				usersList.push_back(tempUser);
			}
		}
		sqlite3_close(database);
		return usersList;
	}
}

std::vector<myStruct::myLogStructure> MyDBHandler::retrieveLogData(){
	if (openDB())
	{
		std::string get_data_qry = "SELECT * FROM LogData";
		std::vector<myStruct::myLogStructure>logList;
		sqlite3_stmt *pointer;
		if (sqlite3_prepare_v2(database, ((char*)get_data_qry.c_str()), -1, &pointer, 0) == SQLITE_OK)
		{
			int result = sqlite3_step(pointer);
			while (result == SQLITE_ROW)
			{
				myStruct::myLogStructure tempLog;
				tempLog.myTimeStamp1.day = (unsigned short)sqlite3_column_int(pointer, 0);
				tempLog.myTimeStamp1.month = (unsigned short)sqlite3_column_int(pointer, 1);
				tempLog.myTimeStamp1.year = (unsigned short)sqlite3_column_int(pointer, 2);
				tempLog.myTimeStamp1.hour = (unsigned short)sqlite3_column_int(pointer, 3);
				tempLog.myTimeStamp1.minute = (unsigned short)sqlite3_column_int(pointer, 4);
				tempLog.myTimeStamp1.second = (unsigned short)sqlite3_column_int(pointer, 5);

				tempLog.EventID = (char*)sqlite3_column_text(pointer, 6);
				tempLog.message = (char*)sqlite3_column_text(pointer, 7);
				tempLog.levelMessageString = (char*)sqlite3_column_text(pointer, 8);
				tempLog.taskMessageString = (char*)sqlite3_column_text(pointer, 9);
				tempLog.opCodeMessageString = (char*)sqlite3_column_text(pointer, 10);
				tempLog.channelMessageString = (char*)sqlite3_column_text(pointer, 11);
				tempLog.providerMessageString = (char*)sqlite3_column_text(pointer, 12);
				tempLog.keywords = (char*)sqlite3_column_text(pointer, 13);
				tempLog.executionProcessID = (char*)sqlite3_column_text(pointer, 14);
				tempLog.executionThreadID = (char*)sqlite3_column_text(pointer, 15);
				tempLog.computer = (char*)sqlite3_column_text(pointer, 16);

				tempLog.mySubject1.Security_ID = (char*)sqlite3_column_text(pointer, 17);
				tempLog.mySubject1.Account_Name = (char*)sqlite3_column_text(pointer, 18);
				tempLog.mySubject1.Account_Domain = (char*)sqlite3_column_text(pointer, 19);
				tempLog.mySubject1.Logon_ID = (char*)sqlite3_column_text(pointer, 20);

				tempLog.myProviderInformation1.Provider_ID = (char*)sqlite3_column_text(pointer, 21);
				tempLog.myProviderInformation1.Provider_Name = (char*)sqlite3_column_text(pointer, 22);

				tempLog.myObject1.Object_Server = (char*)sqlite3_column_text(pointer, 23);
				tempLog.myObject1.Object_Type = (char*)sqlite3_column_text(pointer, 24);
				tempLog.myObject1.Object_Name = (char*)sqlite3_column_text(pointer, 25);
				tempLog.myObject1.Handle_ID = (char*)sqlite3_column_text(pointer, 26);
				tempLog.myObject1.Resource_Attributes = (char*)sqlite3_column_text(pointer, 27);

				tempLog.myNetworkInformation1.Direction = (char*)sqlite3_column_text(pointer, 28);
				tempLog.myNetworkInformation1.Source_Address = (char*)sqlite3_column_text(pointer, 29);
				tempLog.myNetworkInformation1.Source_Port = (char*)sqlite3_column_text(pointer, 30);
				tempLog.myNetworkInformation1.Destination_Address = (char*)sqlite3_column_text(pointer, 31);
				tempLog.myNetworkInformation1.Destination_Port = (char*)sqlite3_column_text(pointer, 32);
				tempLog.myNetworkInformation1.Protocol = (char*)sqlite3_column_text(pointer, 33);
				tempLog.myNetworkInformation1.Workstation_Name = (char*)sqlite3_column_text(pointer, 34);

				tempLog.myLayerInformation1.Layer_ID = (char*)sqlite3_column_text(pointer, 35);
				tempLog.myLayerInformation1.Layer_Name = (char*)sqlite3_column_text(pointer, 36);
				tempLog.myLayerInformation1.Layer_Runtime_ID = (char*)sqlite3_column_text(pointer, 37);

				tempLog.myFilterInformation1.Filter_ID = (char*)sqlite3_column_text(pointer, 38);
				tempLog.myFilterInformation1.Filter_Name = (char*)sqlite3_column_text(pointer, 39);
				tempLog.myFilterInformation1.Filter_Type = (char*)sqlite3_column_text(pointer, 40);
				tempLog.myFilterInformation1.Runtime_ID = (char*)sqlite3_column_text(pointer, 41);
				tempLog.myFilterInformation1.Filter_Runtime_ID = (char*)sqlite3_column_text(pointer, 42);
				tempLog.myFilterInformation1.Filter_Layer_Name = (char*)sqlite3_column_text(pointer, 43);
				tempLog.myFilterInformation1.Filter_Layer_Runtime_ID = (char*)sqlite3_column_text(pointer, 44);

				tempLog.myChangeInformation1.Change_Type = (char*)sqlite3_column_text(pointer, 45);

				tempLog.myCalloutInformation1.Callout_ID = (char*)sqlite3_column_text(pointer, 46);
				tempLog.myCalloutInformation1.Callout_Name = (char*)sqlite3_column_text(pointer, 47);

				tempLog.myApplicationInformation1.Application_Process_ID = (char*)sqlite3_column_text(pointer, 48);
				tempLog.myApplicationInformation1.Application_Name = (char*)sqlite3_column_text(pointer, 49);

				tempLog.myAccessRequestInformation1.Transaction_ID = (char*)sqlite3_column_text(pointer, 50);
				tempLog.myAccessRequestInformation1.Accesses = (char*)sqlite3_column_text(pointer, 51);
				tempLog.myAccessRequestInformation1.Access_Reasons = (char*)sqlite3_column_text(pointer, 52);
				tempLog.myAccessRequestInformation1.Access_Mask = (char*)sqlite3_column_text(pointer, 53);
				tempLog.myAccessRequestInformation1.Privileges_Used_For_Access_Check = (char*)sqlite3_column_text(pointer, 54);
				tempLog.myAccessRequestInformation1.Restricted_SID_Count = (char*)sqlite3_column_text(pointer, 55);

				tempLog.myRuleInformation1.ID = (char*)sqlite3_column_text(pointer, 56);
				tempLog.myRuleInformation1.Name = (char*)sqlite3_column_text(pointer, 57);

				tempLog.myErrorInformation1.Reason = (char*)sqlite3_column_text(pointer, 58);

				tempLog.myProcessInformation1.Process_ID = (char*)sqlite3_column_text(pointer, 59);
				tempLog.myProcessInformation1.Process_Name = (char*)sqlite3_column_text(pointer, 60);

				tempLog.myLogonType1.Logon_Type = (char*)sqlite3_column_text(pointer, 61);

				tempLog.myImpersonationLevel1.Impersonation_Level = (char*)sqlite3_column_text(pointer, 62);

				tempLog.myAccountForWhichLogonFailed1.Security_ID = (char*)sqlite3_column_text(pointer, 63);
				tempLog.myAccountForWhichLogonFailed1.Account_Name = (char*)sqlite3_column_text(pointer, 64);
				tempLog.myAccountForWhichLogonFailed1.Account_Domain = (char*)sqlite3_column_text(pointer, 65);

				tempLog.myFailureInformation1.Failure_Reason = (char*)sqlite3_column_text(pointer, 66);
				tempLog.myFailureInformation1.Status = (char*)sqlite3_column_text(pointer, 67);
				tempLog.myFailureInformation1.Sub_Status = (char*)sqlite3_column_text(pointer, 68);

				tempLog.myNewLogon1.Security_ID = (char*)sqlite3_column_text(pointer, 69);
				tempLog.myNewLogon1.Account_Name = (char*)sqlite3_column_text(pointer, 70);
				tempLog.myNewLogon1.Account_Domain = (char*)sqlite3_column_text(pointer, 71);
				tempLog.myNewLogon1.Logon_ID = (char*)sqlite3_column_text(pointer, 72);
				tempLog.myNewLogon1.Logon_GUID = (char*)sqlite3_column_text(pointer, 73);

				tempLog.myDetailedAuthenticationInformation1.Logon_Process = (char*)sqlite3_column_text(pointer, 74);
				tempLog.myDetailedAuthenticationInformation1.Authentication_Package = (char*)sqlite3_column_text(pointer, 75);
				tempLog.myDetailedAuthenticationInformation1.Transited_Services = (char*)sqlite3_column_text(pointer, 76);
				tempLog.myDetailedAuthenticationInformation1.Package_Name = (char*)sqlite3_column_text(pointer, 77);
				tempLog.myDetailedAuthenticationInformation1.Key_Length = (char*)sqlite3_column_text(pointer, 78);

				tempLog.eventCategory = (char*)sqlite3_column_text(pointer, 79);
				tempLog.processName = (char*)sqlite3_column_text(pointer, 80);

				result = sqlite3_step(pointer);
				logList.push_back(tempLog);
			}
		}
		sqlite3_close(database);
		return logList;
	}
}

void MyDBHandler::deleteLogData(){
	if (openDB())
	{
		std::string delete_qry = "DELETE FROM LogData; SELECT * from LogData";
		rc = query((char*)delete_qry.c_str());
		sqlite3_close(database);
		if (rc){
			fprintf(stdout, "Data deleted successfully\n");
		}
		else{
			fprintf(stderr, "SQL error: %s\n", myZErrMsg);
			sqlite3_free(myZErrMsg);
		}
		sqlite3_close(database);
		isAvailableLogData = false;
	}
}

void MyDBHandler::deleteCurrentUserData(){
	if (openDB())
	{
		std::string delete_qry = "DELETE FROM CurrentUserData; SELECT * from CurrentUserData";
		rc = query((char*)delete_qry.c_str());
		sqlite3_close(database);
		if (rc){
			fprintf(stdout, "Data deleted successfully\n");
		}
		else{
			fprintf(stderr, "SQL error: %s\n", myZErrMsg);
			sqlite3_free(myZErrMsg);
		}
		sqlite3_close(database);
		isAvailableCurrentUserData = false;
	}
}

void MyDBHandler::deleteUsersData(){
	if (openDB())
	{
		std::string delete_qry = "DELETE FROM UsersData; SELECT * from UsersData";
		rc = query((char*)delete_qry.c_str());
		sqlite3_close(database);
		if (rc){
			fprintf(stdout, "Data deleted successfully\n");
		}
		else{
			fprintf(stderr, "SQL error: %s\n", myZErrMsg);
			sqlite3_free(myZErrMsg);
		}
		sqlite3_close(database);
		isAvailableUsersData = false;
	}
}