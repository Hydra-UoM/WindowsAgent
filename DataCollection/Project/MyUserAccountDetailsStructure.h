#include "stdafx.h"
#ifndef MyUserAccountDetailsStructure_H
#define MyUserAccountDetailsStructure_H

#include <windows.h>
#include <string>
#include <vector>
#include <atlstr.h>
#include <Lmaccess.h>
#include <iostream>

#include "MyLogStruct.h"

//using namespace std;

class MyUserAccountDetailsStructure
{
public:
	MyUserAccountDetailsStructure(LPWSTR  computerName1, LPWSTR usri4_name1, DWORD usri4_password_age1, DWORD usri4_priv1,
		DWORD usri4_flags1, LPWSTR usri4_usr_comment1, LPWSTR usri4_parms1, DWORD usri4_last_logon1, DWORD usri4_last_logoff1,
		DWORD usri4_acct_expires1, DWORD usri4_max_storage1, DWORD usri4_units_per_week1, PBYTE usri4_logon_hours1,
		DWORD usri4_bad_pw_count1, DWORD usri4_num_logons1, DWORD usri4_country_code1, DWORD usri4_code_page1,
		DWORD usri4_primary_group_id1, LPWSTR usri4_profile1, DWORD usri4_password_expired1, DWORD usri4_auth_flags1);

	void print();
	myStruct::myUserAccountDetailsStruct toUserDetailsStruct(int summarizationLevel);

	~MyUserAccountDetailsStructure(void);

	LPWSTR  computerName;
	LPWSTR usri4_name;
	DWORD usri4_password_age; // in second
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
	DWORD usri4_primary_group_id = -10000;
	LPWSTR usri4_profile = L"";
	DWORD usri4_password_expired = -10000;
	DWORD usri4_auth_flags = 0;

	/** not available in LPUSER_INFO_2; Following are the default values
	DWORD usri2_primary_group_id = -10000;
	LPWSTR usri2_profile = L"Not Set";
	DWORD usri2_password_expired = -10000;
	*/
};
#endif