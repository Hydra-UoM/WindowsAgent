#include "stdafx.h"
#ifndef MyLogStruct_H
#define MyLogStruct_H

#include <string>

namespace myStruct
{
	struct myUserAccountDetailsStruct
	{
		std::string computerName = "";
		std::string usri4_name = "";
		std::string usri4_password_age = "";
		std::string usri4_priv = "";
		std::string usri4_flags = "";
		std::string usri4_usr_comment = "";
		std::string usri4_parms = "";
		std::string usri4_last_logon = "";
		std::string usri4_last_logoff = "";
		std::string usri4_acct_expires = "";
		std::string usri4_max_storage = "";
		std::string usri4_units_per_week = "";
		std::string usri4_logon_hours = "";
		std::string usri4_bad_pw_count = "";
		std::string usri4_num_logons = "";
		std::string usri4_country_code = "";
		std::string usri4_code_page = "";
		std::string usri4_primary_group_id = "";
		std::string usri4_profile = "";
		std::string usri4_password_expired = "";
		std::string usri4_auth_flags = "";
	};

	struct myRuleInformation
	{
		std::string ID = "";
		std::string Name = "";
	};

	struct mySubject
	{
		std::string Security_ID = "";
		std::string Account_Name = "";
		std::string Account_Domain = "";
		std::string Logon_ID = "";
	};

	struct myProviderInformation
	{
		std::string Provider_ID = "";
		std::string Provider_Name = "";
	};

	struct myObject
	{
		std::string Object_Server = "";
		std::string Object_Type = "";
		std::string Object_Name = "";
		std::string Handle_ID = "";
		std::string Resource_Attributes = "";
	};

	struct myProcessInformation
	{
		std::string Process_ID = "";
		std::string Process_Name = "";
	};

	struct myNewLogon
	{
		std::string Security_ID = "";
		std::string Account_Name = "";
		std::string Account_Domain = "";
		std::string Logon_ID = "";
		std::string Logon_GUID = "";
	};

	struct myNetworkInformation
	{
		std::string Direction = "";
		std::string Source_Address = "";
		std::string Source_Port = "";
		std::string Destination_Address = "";
		std::string Destination_Port = "";
		std::string Protocol = "";
		std::string Workstation_Name = "";
	};

	struct myLogonType
	{
		std::string Logon_Type = "";
	};

	struct myLayerInformation
	{
		std::string Layer_ID = "";
		std::string Layer_Name = "";
		std::string Layer_Runtime_ID = "";
	};

	struct myImpersonationLevel
	{
		std::string Impersonation_Level = "";
	};

	struct myFilterInformation
	{
		std::string Filter_ID = "";
		std::string Filter_Name = "";
		std::string Filter_Type = "";
		std::string Runtime_ID = "";
		std::string Filter_Runtime_ID = "";
		std::string Filter_Layer_Name = "";
		std::string Filter_Layer_Runtime_ID = "";
	};

	struct myFailureInformation
	{
		std::string Failure_Reason = "";
		std::string Status = "";
		std::string Sub_Status = "";
	};

	struct myErrorInformation
	{
		std::string Reason = "";
	};

	struct myDetailedAuthenticationInformation
	{
		std::string Logon_Process = "";
		std::string Authentication_Package = "";
		std::string Transited_Services = "";
		std::string Package_Name = "";
		std::string Key_Length = "";
	};

	struct myChangeInformation
	{
		std::string Change_Type = "";
	};

	struct myCalloutInformation
	{
		std::string Callout_ID = "";
		std::string Callout_Name = "";
	};

	struct myApplicationInformation
	{
		std::string Application_Process_ID = "";
		std::string Application_Name = "";
	};

	struct myAdditionalInformation
	{
		std::string Weight = "";
		std::string Conditions = "";
		std::string Condition_ID1 = "";
		std::string Match_Value1 = "";
		std::string Condition_Value1 = "";
		std::string Condition_ID2 = "";
		std::string Match_Value2 = "";
		std::string Condition_Value2 = "";
		std::string Condition_ID3 = "";
		std::string Match_Value3 = "";
		std::string Condition_Value3 = "";
		std::string Filter_Action = "";
	};

	struct myAccountForWhichLogonFailed
	{
		std::string Security_ID = "";
		std::string Account_Name = "";
		std::string Account_Domain = "";
	};

	struct myAccessRequestInformation
	{
		std::string Transaction_ID = "";
		std::string Accesses = "";
		std::string Access_Reasons = "";
		std::string Access_Mask = "";
		std::string Privileges_Used_For_Access_Check = "";
		std::string Restricted_SID_Count = "";
	};

	struct myTimeStamp
	{
		unsigned short month;
		unsigned short day;
		unsigned short year;
		unsigned short hour;
		unsigned short minute;
		unsigned short second;
	};

	struct myLogStructure
	{
		std::string message = "";
		std::string levelMessageString = "";
		std::string taskMessageString = "";
		std::string opCodeMessageString = "";
		std::string channelMessageString = "";
		std::string providerMessageString = "";

		//std::string version = "";
		//std::string level = "";
		//std::string task = "";
		//std::string opCode = "";
		std::string keywords = "";
		//std::string eventRecordID = "";
		std::string executionProcessID = "";
		std::string executionThreadID = "";
		//std::string channel = "";
		std::string computer = "";
		std::string EventID = "";
		/**
		bool isAvailableMySubject;
		bool isAvailableMyProviderInformation;
		bool isAvailableMyProcessInformation;
		bool isAvailableMyObject;
		bool isAvailableMyNetworkInformation;
		bool isAvailableMyLayerInformation;
		bool isAvailableMyFilterInformation;
		bool isAvailableMyChangeInformation;
		bool isAvailableMyCalloutInformation;
		bool isAvailableMyApplicationInformation;
		bool isAvailableMyAdditionalInformation;
		bool isAvailableMyAccessRequestInformation;
		bool isAvailableMyRuleInformation;
		bool isAvailableMyErrorInformation;

		bool isAvailableMyLogonType;
		bool isAvailableMyImpersonationLevel;
		bool isAvailableMyAccountForWhichLogonFailed;
		bool isAvailableMyFailureInformation;
		bool isAvailableMyNewLogon;
		bool isAvailableMyDetailedAuthenticationInformation;
		*/
		myTimeStamp myTimeStamp1;
		mySubject mySubject1;
		myProviderInformation myProviderInformation1;
		myObject myObject1;
		myNetworkInformation myNetworkInformation1;
		myLayerInformation myLayerInformation1;
		myFilterInformation myFilterInformation1;
		myChangeInformation myChangeInformation1;
		myCalloutInformation myCalloutInformation1;
		myApplicationInformation myApplicationInformation1;
		//myAdditionalInformation myAdditionalInformation1;
		myAccessRequestInformation myAccessRequestInformation1;
		myRuleInformation myRuleInformation1;
		myErrorInformation myErrorInformation1;
		myProcessInformation myProcessInformation1;

		myLogonType myLogonType1;
		myImpersonationLevel myImpersonationLevel1;
		myAccountForWhichLogonFailed myAccountForWhichLogonFailed1;
		myFailureInformation myFailureInformation1;
		myNewLogon myNewLogon1;
		myDetailedAuthenticationInformation myDetailedAuthenticationInformation1;

		std::string eventCategory = "";
		std::string processName = "";
	};
}
#endif