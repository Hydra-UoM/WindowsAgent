#include "stdafx.h"
#include "MyUserAccountDetailsStructure.h"

using namespace std;

MyUserAccountDetailsStructure::MyUserAccountDetailsStructure(LPWSTR computerName1, LPWSTR usri4_name1, DWORD usri4_password_age1,
	DWORD usri4_priv1,DWORD usri4_flags1, LPWSTR usri4_usr_comment1, LPWSTR usri4_parms1, DWORD usri4_last_logon1, 
	DWORD usri4_last_logoff1, DWORD usri4_acct_expires1, DWORD usri4_max_storage1, DWORD usri4_units_per_week1,
	PBYTE usri4_logon_hours1,DWORD usri4_bad_pw_count1, DWORD usri4_num_logons1, DWORD usri4_country_code1, DWORD usri4_code_page1,
	DWORD usri4_primary_group_id1, LPWSTR usri4_profile1, DWORD usri4_password_expired1, DWORD usri4_auth_flags1) :

	computerName(computerName1), usri4_name(usri4_name1), usri4_password_age(usri4_password_age1),
	usri4_priv(usri4_priv1), usri4_flags(usri4_flags1), usri4_usr_comment(usri4_usr_comment1),
	usri4_parms(usri4_parms1), usri4_last_logon(usri4_last_logon1), usri4_last_logoff(usri4_last_logoff1),
	usri4_acct_expires(usri4_acct_expires1), usri4_max_storage(usri4_max_storage1), usri4_units_per_week(usri4_units_per_week1),
	usri4_logon_hours(usri4_logon_hours1), usri4_bad_pw_count(usri4_bad_pw_count1), usri4_num_logons(usri4_num_logons1),
	usri4_country_code(usri4_country_code1), usri4_code_page(usri4_code_page1), usri4_primary_group_id(usri4_primary_group_id1),
	usri4_profile(usri4_profile1), usri4_password_expired(usri4_password_expired1), usri4_auth_flags(usri4_auth_flags1)
{
	computerName = computerName1;
	usri4_name = usri4_name1;
	usri4_password_age = usri4_password_age1;
	usri4_priv = usri4_priv1;
	usri4_flags = usri4_flags1;
	usri4_usr_comment = usri4_usr_comment1;
	usri4_parms = usri4_parms1;
	usri4_last_logon = usri4_last_logon1;
	usri4_last_logoff = usri4_last_logoff1;
	usri4_acct_expires = usri4_acct_expires1;
	usri4_max_storage = usri4_max_storage1;
	usri4_units_per_week = usri4_units_per_week1;
	usri4_logon_hours = usri4_logon_hours1;
	usri4_bad_pw_count = usri4_bad_pw_count1;
	usri4_num_logons = usri4_num_logons1;
	usri4_country_code = usri4_country_code1;
	usri4_code_page = usri4_code_page1;
	usri4_primary_group_id = usri4_primary_group_id1;
	usri4_profile = usri4_profile1;
	usri4_password_expired = usri4_password_expired1;
	usri4_auth_flags = usri4_auth_flags1;
}

void MyUserAccountDetailsStructure::print()
{
	//wprintf(L"\tComputer name: %s\n", computerName);
	wprintf(L"\tUser Name: %s\n", usri4_name);
	wprintf(L"\tPassword age (seconds): %d\n", usri4_password_age);
	wprintf(L"\tPrivilege level: %d\n", usri4_priv);
	wprintf(L"\tFlags (in decimal): %d\n", usri4_flags);
	wprintf(L"\tAuth flags (in hex): %x\n", usri4_auth_flags);
	wprintf(L"\tUser comment: %s\n", usri4_usr_comment);
	wprintf(L"\tParameters: %s\n", usri4_parms);
	wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n", usri4_last_logon);
	wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n", usri4_last_logoff);
	wprintf(L"\tAccount expires (seconds since January 1, 1970 GMT): %d\n", usri4_acct_expires);
	wprintf(L"\tMax storage: %d\n", usri4_max_storage);
	wprintf(L"\tUnits per week: %d\n", usri4_units_per_week);
	//wprintf(L"\tLogon hours: %d\n", usri4_logon_hours);
	wprintf(L"\tBad password count: %d\n", usri4_bad_pw_count);
	wprintf(L"\tNumber of logons: %d\n", usri4_num_logons);
	wprintf(L"\tCountry code: %d\n", usri4_country_code);
	wprintf(L"\tCode page: %d\n", usri4_code_page);
	wprintf(L"\tPrimary group ID: %d\n", usri4_primary_group_id);
	//wprintf(L"\tProfile: %s\n", usri4_profile);
	wprintf(L"\tPassword expired information: %d\n", usri4_password_expired);
	//zero if the password has not expired (and nonzero if it has).

	/**
	usri4_priv

	USER_PRIV_GUEST - 0
	USER_PRIV_USER - 1
	USER_PRIV_ADMIN - 2
	*/
	/**
	usri4_flags

	UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION - 16777216
	UF_SCRIPT - 1
	UF_ACCOUNTDISABLE - 2
	UF_HOMEDIR_REQUIRED - 8
	UF_PASSWD_NOTREQD - 32
	UF_PASSWD_CANT_CHANGE - 64
	UF_LOCKOUT - 16
	UF_DONT_EXPIRE_PASSWD - 65536
	UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED - 128
	UF_NOT_DELEGATED - 1048576
	UF_SMARTCARD_REQUIRED - 262144
	UF_USE_DES_KEY_ONLY - 2097152
	UF_DONT_REQUIRE_PREAUTH - 4194304
	UF_TRUSTED_FOR_DELEGATION - 524288
	UF_PASSWORD_EXPIRED - 8388608

	UF_INTERDOMAIN_TRUST_ACCOUNT - 2048
	UF_SERVER_TRUST_ACCOUNT - 8192
	UF_WORKSTATION_TRUST_ACCOUNT - 4096
	UF_TEMP_DUPLICATE_ACCOUNT - 256
	UF_NORMAL_ACCOUNT - 512
	*/

	/**
	usri4_auth_flags

	AF_OP_PRINT - 1
	AF_OP_COMM - 2
	AF_OP_SERVER - 4
	AF_OP_ACCOUNTS - 8
	*/

	/**
	usri4_acct_expires

	TIMEQ_FOREVER - -1
	*/

	/**
	usri4_max_storage

	USER_MAXSTORAGE_UNLIMITED - -1
	*/

	/**
	usri4_units_per_week

	UNITS_PER_WEEK - 168
	*/

	/**
	usri4_country_code English - 0
	*/

	/**
	usri4_primary_group_id

	DOMAIN_GROUP_RID_USERS - 513
	*/
}

myStruct::myUserAccountDetailsStruct MyUserAccountDetailsStructure::toUserDetailsStruct(int summarizationLevel)
{
	myStruct::myUserAccountDetailsStruct userString;
	if (summarizationLevel == 0)
	{
		if (computerName){ userString.computerName = CW2A(computerName); }
		if (usri4_name){ userString.usri4_name = CW2A(usri4_name); }
		userString.usri4_password_age = to_string(usri4_password_age);
		userString.usri4_priv = to_string(usri4_priv);
		userString.usri4_flags = to_string(usri4_flags);
		userString.usri4_auth_flags = to_string(usri4_auth_flags);
		if (usri4_usr_comment){ userString.usri4_usr_comment = CW2A(usri4_usr_comment); }
		if (usri4_parms){ userString.usri4_parms = CW2A(usri4_parms); }
		userString.usri4_last_logon = to_string(usri4_last_logon);
		userString.usri4_last_logoff = to_string(usri4_last_logoff);
		userString.usri4_acct_expires = to_string(usri4_acct_expires);
		userString.usri4_max_storage = to_string(usri4_max_storage);
		userString.usri4_units_per_week = to_string(usri4_units_per_week);
		userString.usri4_logon_hours = to_string(*((unsigned long*)usri4_logon_hours));
		userString.usri4_bad_pw_count = to_string(usri4_bad_pw_count);
		userString.usri4_num_logons = to_string(usri4_num_logons);
		userString.usri4_country_code = to_string(usri4_country_code);
		userString.usri4_code_page = to_string(usri4_code_page);
		userString.usri4_primary_group_id = to_string(usri4_primary_group_id);
		userString.usri4_password_expired = to_string(usri4_password_expired);
	}
	else if (summarizationLevel == 1)
	{
		if (computerName){ userString.computerName = CW2A(computerName); }
		if (usri4_name){ userString.usri4_name = CW2A(usri4_name); }
		userString.usri4_password_age = to_string(usri4_password_age);
		userString.usri4_priv = to_string(usri4_priv);
		//userString.usri4_flags = to_string(usri4_flags);
		//userString.usri4_auth_flags = to_string(usri4_auth_flags);
		//userString.usri4_usr_comment = CW2A(usri4_usr_comment);
		//userString.usri4_parms = CW2A(usri4_parms);
		userString.usri4_last_logon = to_string(usri4_last_logon);
		userString.usri4_last_logoff = to_string(usri4_last_logoff);
		userString.usri4_acct_expires = to_string(usri4_acct_expires);
		userString.usri4_max_storage = to_string(usri4_max_storage);
		//userString.usri4_units_per_week = to_string(usri4_units_per_week);
		userString.usri4_logon_hours = to_string(*((unsigned long*)usri4_logon_hours));
		userString.usri4_bad_pw_count = to_string(usri4_bad_pw_count);
		userString.usri4_num_logons = to_string(usri4_num_logons);
		//userString.usri4_country_code = to_string(usri4_country_code);
		//userString.usri4_code_page = to_string(usri4_code_page);
		//userString.usri4_primary_group_id = to_string(usri4_primary_group_id);
		userString.usri4_password_expired = to_string(usri4_password_expired);
	}
	else
	{
		if (computerName){ userString.computerName = CW2A(computerName); }
		if (usri4_name){ userString.usri4_name = CW2A(usri4_name); }
		userString.usri4_password_age = to_string(usri4_password_age);
		userString.usri4_priv = to_string(usri4_priv);
		//userString.usri4_flags = to_string(usri4_flags);
		//userString.usri4_auth_flags = to_string(usri4_auth_flags);
		//userString.usri4_usr_comment = CW2A(usri4_usr_comment);
		//userString.usri4_parms = CW2A(usri4_parms);
		userString.usri4_last_logon = to_string(usri4_last_logon);
		userString.usri4_last_logoff = to_string(usri4_last_logoff);
		userString.usri4_acct_expires = to_string(usri4_acct_expires);
		userString.usri4_max_storage = to_string(usri4_max_storage);
		//userString.usri4_units_per_week = to_string(usri4_units_per_week);
		//userString.usri4_logon_hours = to_string(*((unsigned long*)usri4_logon_hours));
		userString.usri4_bad_pw_count = to_string(usri4_bad_pw_count);
		//userString.usri4_num_logons = to_string(usri4_num_logons);
		//userString.usri4_country_code = to_string(usri4_country_code);
		//userString.usri4_code_page = to_string(usri4_code_page);
		//userString.usri4_primary_group_id = to_string(usri4_primary_group_id);
		userString.usri4_password_expired = to_string(usri4_password_expired);
	}
	return userString;
}

MyUserAccountDetailsStructure::~MyUserAccountDetailsStructure(void)
{

}