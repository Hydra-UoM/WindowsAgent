#include "stdafx.h"
#include "MyUserAccountDetails.h"

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <lm.h>
#include <sddl.h>               /* for ConvertSidToStringSid function */
#include <Lmcons.h>
#include <tchar.h>

using namespace myStruct;
using namespace std;

#define INFO_BUFFER_SIZE 32767

#define ARRAY_SIZE 1 // earlier was 10, handle by 10 of blocks
#define TIME_OUT INFINITE

MyUserAccountDetailsStructure*myCurrentUserAccountStructure;
//int numberOfAvailableEvents = 0;

MyUserAccountDetails::MyUserAccountDetails()
{
}

myStruct::myUserAccountDetailsStruct MyUserAccountDetails::getCurrentLoggedOnUserInformation(int summarizationLevel)
{
	LPWSTR  computerName;
	LPWSTR usri4_name;
	DWORD usri4_password_age;
	DWORD usri4_priv;
	DWORD usri4_flags;
	LPWSTR usri4_usr_comment;
	LPWSTR usri4_parms;
	DWORD usri4_last_logon;
	DWORD usri4_last_logoff;
	DWORD usri4_acct_expires;
	DWORD usri4_max_storage;
	DWORD usri4_units_per_week;
	PBYTE usri4_logon_hours;
	DWORD usri4_bad_pw_count;
	DWORD usri4_num_logons;
	DWORD usri4_country_code;
	DWORD usri4_code_page;
	DWORD usri4_primary_group_id;
	LPWSTR usri4_profile;
	DWORD usri4_password_expired;
	DWORD usri4_auth_flags;

	DWORD dwLevel = 4;

	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_4 pBuf4 = NULL;

	NET_API_STATUS nStatus;

	LPTSTR sStringSid = NULL;
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	// Get and display the name of the computer. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetComputerName(infoBuf, &bufCharCount))
		printError(TEXT("GetComputerName"));
	computerName = infoBuf;
	//_tprintf(TEXT("\tComputer name:      %s\n"), infoBuf);

	// Get and display the user name. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetUserName(infoBuf, &bufCharCount))
		printError(TEXT("GetUserName"));
	usri4_name = infoBuf;
	//_tprintf(TEXT("\nUser name:          %s\n"), infoBuf);
	// Call the NetUserGetInfo function.
	LPCWSTR servername = NULL;
	LPCWSTR username = infoBuf;//L"Jeyatharshini";
	nStatus = NetUserGetInfo(servername, username, dwLevel, (LPBYTE *)& pBuf);
	
	// This is not succeed in service:???
	if (nStatus == NERR_Success)
	{
		if (pBuf != NULL)
		{
			pBuf4 = (LPUSER_INFO_4)pBuf;
			usri4_name = pBuf4->usri4_name;
			//wprintf(L"\tPassword: %s\n", pBuf4->usri4_password);
			usri4_password_age = pBuf4->usri4_password_age;

			usri4_priv = pBuf4->usri4_priv;
			
			//wprintf(L"\tHome directory: %s\n", pBuf4->usri4_home_dir);
			//wprintf(L"\tComment: %s\n", pBuf4->usri4_comment);
			usri4_flags = pBuf4->usri4_flags;
		
			//wprintf(L"\tScript path: %s\n", pBuf4->usri4_script_path);
			
			//wprintf(L"\tFull name: %s\n", pBuf4->usri4_full_name);
			usri4_usr_comment = pBuf4->usri4_usr_comment;
			
			usri4_parms = pBuf4->usri4_parms;
			
			//wprintf(L"\tWorkstations: %s\n", pBuf4->usri4_workstations);
			usri4_last_logon = pBuf4->usri4_last_logon;
			
			usri4_last_logoff = pBuf4->usri4_last_logoff;
			
			usri4_acct_expires = pBuf4->usri4_acct_expires;
			
			usri4_max_storage = pBuf4->usri4_max_storage;
			
			usri4_units_per_week = pBuf4->usri4_units_per_week;

			usri4_logon_hours = pBuf4->usri4_logon_hours;
			
			/**
			for (j = 0; j < 21; j++)
			{
			printf(" %x", (BYTE)pBuf4->usri4_logon_hours[j]);
			}
			*/
			usri4_bad_pw_count = pBuf4->usri4_bad_pw_count;
			
			usri4_num_logons = pBuf4->usri4_num_logons;
			
			//(L"\tLogon server: %s\n", pBuf4->usri4_logon_server);
			usri4_country_code = pBuf4->usri4_country_code;
			
			usri4_code_page = pBuf4->usri4_code_page;
			
			/*
			if (ConvertSidToStringSid(pBuf4->usri4_user_sid, &sStringSid))
			{
				wprintf(L"\tUser SID: %s\n", sStringSid);
				LocalFree(sStringSid);
			}
			else
				wprintf(L"ConvertSidToSTringSid failed with error %d\n",GetLastError());
			*/
			usri4_primary_group_id = pBuf4->usri4_primary_group_id;
			usri4_auth_flags = pBuf4->usri4_auth_flags;
			
			usri4_profile = pBuf4->usri4_profile;
			
			//wprintf(L"\tHome directory drive letter: %s\n",pBuf4->usri4_home_dir_drive);
			usri4_password_expired = pBuf4->usri4_password_expired;
			myCurrentUserAccountStructure = new MyUserAccountDetailsStructure(computerName, usri4_name, usri4_password_age, usri4_priv,
				usri4_flags, usri4_usr_comment, usri4_parms, usri4_last_logon, usri4_last_logoff,
				usri4_acct_expires, usri4_max_storage, usri4_units_per_week, usri4_logon_hours,
				usri4_bad_pw_count, usri4_num_logons, usri4_country_code, usri4_code_page,
				usri4_primary_group_id, usri4_profile, usri4_password_expired, usri4_auth_flags);
		}
		// Otherwise, print the system error.
		//

		else
		{
			fprintf(stderr, "NetUserGetinfo failed with error: %d\n", nStatus);
		}
		//
		// Free the allocated memory.
		//
	}
	myUserAccountDetailsStruct userString;
	if (myCurrentUserAccountStructure != NULL)
	{
		userString = myCurrentUserAccountStructure->toUserDetailsStruct(summarizationLevel);
	}
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	//wprintf(L"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	return userString;
}

void MyUserAccountDetails::printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n\t%s failed with error %d (%s)"), msg, eNum, sysMsg);
}

vector<myStruct::myUserAccountDetailsStruct> MyUserAccountDetails::getAllUserInformation(int summarizationLevel)
{
	vector<myStruct::myUserAccountDetailsStruct>returnedResult;

	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	LPWSTR  computerName;

	// Get and display the name of the computer. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetComputerName(infoBuf, &bufCharCount))
		printError(TEXT("GetComputerName"));
	computerName = infoBuf;
	//_tprintf(TEXT("\nComputer name:      %s"), infoBuf);

	LPUSER_INFO_2 pBuf = NULL;
	LPUSER_INFO_2 pTmpBuf;
	DWORD dwLevel = 2;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	// The server is the default local computer.
	pszServerName = NULL;
	//wprintf(L"\nUser account on %s: \n", pszServerName);
	//
	// Call the NetUserEnum function, specifying level 0; 
	//   enumerate global user account types only.
	//
	do // begin do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		//
		// If the call succeeds,
		//
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				//
				// Loop through the entries.
				//
				for (i = 0; (i < dwEntriesRead); i++)
				{
					assert(pTmpBuf != NULL);

					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					LPWSTR usri2_name;
					DWORD usri2_password_age;
					DWORD usri2_priv;
					DWORD usri2_flags;
					LPWSTR usri2_usr_comment;
					LPWSTR usri2_parms;
					DWORD usri2_last_logon;
					DWORD usri2_last_logoff;
					DWORD usri2_acct_expires;
					DWORD usri2_max_storage;
					DWORD usri2_units_per_week;
					PBYTE usri2_logon_hours;
					DWORD usri2_bad_pw_count;
					DWORD usri2_num_logons;
					DWORD usri2_country_code;
					DWORD usri2_code_page;
					DWORD usri2_auth_flags;

					// not available in LPUSER_INFO_2; I assigned some default values
					DWORD usri2_primary_group_id = -10000;
					LPWSTR usri2_profile = L"Not Set";
					DWORD usri2_password_expired = -10000;

					usri2_name = pTmpBuf->usri2_name;
					DWORD pwAgeSeconds = pTmpBuf->usri2_password_age;
					usri2_password_age = pwAgeSeconds;
					usri2_priv = pTmpBuf->usri2_priv;
					usri2_flags = pTmpBuf->usri2_flags;
					usri2_usr_comment = pTmpBuf->usri2_usr_comment;
					usri2_parms = pTmpBuf->usri2_parms;
					usri2_last_logon = pTmpBuf->usri2_last_logon;
					usri2_last_logoff = pTmpBuf->usri2_last_logoff;
					usri2_acct_expires = pTmpBuf->usri2_acct_expires;
					usri2_max_storage = pTmpBuf->usri2_max_storage;
					usri2_units_per_week = pTmpBuf->usri2_units_per_week;
					usri2_logon_hours = pTmpBuf->usri2_logon_hours;
					usri2_bad_pw_count = pTmpBuf->usri2_bad_pw_count;
					usri2_num_logons = pTmpBuf->usri2_num_logons;
					usri2_country_code = pTmpBuf->usri2_country_code;/** The country/region code for the user's language of choice.*/
					usri2_code_page = pTmpBuf->usri2_code_page;/** The code page for the user's language of choice.*/
					usri2_auth_flags = pTmpBuf->usri2_auth_flags;
					/*
					DWORD pwAgeMinute = pwAgeSeconds / 60;
					DWORD pwAgeHour = pwAgeMinute / 60;
					DWORD pwAgeDay = pwAgeHour / 24;
					*/

					myCurrentUserAccountStructure = new MyUserAccountDetailsStructure(computerName, usri2_name, usri2_password_age, usri2_priv,
						usri2_flags, usri2_usr_comment, usri2_parms, usri2_last_logon, usri2_last_logoff,
						usri2_acct_expires, usri2_max_storage, usri2_units_per_week, usri2_logon_hours,
						usri2_bad_pw_count, usri2_num_logons, usri2_country_code, usri2_code_page,
						usri2_primary_group_id, usri2_profile, usri2_password_expired, usri2_auth_flags);

					//myCurrentUserAccountStructure->print();
					//wprintf(L"\n\n");

					myUserAccountDetailsStruct local_returnedResult;
					local_returnedResult = myCurrentUserAccountStructure->toUserDetailsStruct(summarizationLevel);
					returnedResult.push_back(local_returnedResult);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		//
		// Otherwise, print the system error.
		//
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		//
		// Free the allocated buffer.
		//
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	// Continue to call NetUserEnum while 
	//  there are more entries. 
	// 
	while (nStatus == ERROR_MORE_DATA); // end do
	//
	// Check again for allocated memory.
	//
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	//
	// Print the final count of users enumerated.
	//
	//fprintf(stderr, "\nTotal of %d entries enumerated\n", dwTotalCount);
	//wprintf(L"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	return returnedResult;
}

MyUserAccountDetails::~MyUserAccountDetails(void)
{

}