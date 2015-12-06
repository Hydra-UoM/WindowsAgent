#include "stdafx.h"
#include "MyUserAccountDetailsStructure.h"

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
	wprintf(L"\tUser account name: %s\n", usri4_name);
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
	wprintf(L"\tLogon hours: %d\n", usri4_logon_hours);
	wprintf(L"\tBad password count: %d\n", usri4_bad_pw_count);
	wprintf(L"\tNumber of logons: %d\n", usri4_num_logons);
	wprintf(L"\tCountry code: %d\n", usri4_country_code);
	wprintf(L"\tCode page: %d\n", usri4_code_page);
	wprintf(L"\tPrimary group ID: %d\n", usri4_primary_group_id);
	//wprintf(L"\tProfile: %s\n", usri4_profile);
	wprintf(L"\tPassword expired information: %d\n", usri4_password_expired);
}

list<string> MyUserAccountDetailsStructure::toUserDetailsString()
{
	list<string> userString;
	string str_usri4_name = CW2A(usri4_name);											userString.push_back(str_usri4_name);
	string str_usri4_password_age = to_string(usri4_password_age);						userString.push_back(str_usri4_password_age);
	string str_usri4_priv = to_string(usri4_priv);										userString.push_back(str_usri4_priv);
	string str_usri4_flags = to_string(usri4_flags);									userString.push_back(str_usri4_flags);
	string str_usri4_auth_flags = to_string(usri4_auth_flags);							userString.push_back(str_usri4_auth_flags);
	string str_usri4_usr_comment = CW2A(usri4_usr_comment);								userString.push_back(str_usri4_usr_comment);
	string str_usri4_parms = CW2A(usri4_parms);											userString.push_back(str_usri4_parms);
	string str_usri4_last_logon = to_string(usri4_last_logon);							userString.push_back(str_usri4_last_logon);
	string str_usri4_last_logoff = to_string(usri4_last_logoff);						userString.push_back(str_usri4_last_logoff);
	string str_usri4_acct_expires = to_string(usri4_acct_expires);						userString.push_back(str_usri4_acct_expires);
	string str_usri4_max_storage = to_string(usri4_max_storage);						userString.push_back(str_usri4_max_storage);
	string str_usri4_units_per_week = to_string(usri4_units_per_week);					userString.push_back(str_usri4_units_per_week);
	//string str_usri4_logon_hours(reinterpret_cast< char const* >(usri4_logon_hours));	userString.push_back(str_usri4_logon_hours);
	string str_usri4_bad_pw_count = to_string(usri4_bad_pw_count);						userString.push_back(str_usri4_bad_pw_count);
	string str_usri4_num_logons = to_string(usri4_num_logons);							userString.push_back(str_usri4_num_logons);
	string str_usri4_country_code = to_string(usri4_country_code);						userString.push_back(str_usri4_country_code);
	string str_usri4_code_page = to_string(usri4_code_page);							userString.push_back(str_usri4_code_page);
	string str_usri4_primary_group_id = to_string(usri4_primary_group_id);				userString.push_back(str_usri4_primary_group_id);
	string str_usri4_password_expired = to_string(usri4_password_expired);				userString.push_back(str_usri4_password_expired);
	return userString;
}

MyUserAccountDetailsStructure::~MyUserAccountDetailsStructure(void)
{

}