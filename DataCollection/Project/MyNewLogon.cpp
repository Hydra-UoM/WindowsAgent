/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyNewLogon.h"

MyNewLogon::MyNewLogon(const wchar_t* Security_ID1, const wchar_t* Account_Name1, const wchar_t* Account_Domain1, const wchar_t* Logon_ID1, const wchar_t* Logon_GUID1) :
Security_ID(Security_ID1), Account_Name(Account_Name1), Account_Domain(Account_Domain1), Logon_ID(Logon_ID1), Logon_GUID(Logon_GUID1)
{
	Security_ID = Security_ID1;
	Account_Name = Account_Name1;
	Account_Domain = Account_Domain1;
	Logon_ID = Logon_ID1;
	Logon_GUID = Logon_GUID1;
}

list<string>MyNewLogon::toLogString(list<string>prepart, int summarizationLevel)
{
	prepart.push_back("New Logon");
	if (summarizationLevel == 0)
	{
		string str_Security_ID = CW2A(Security_ID);						prepart.push_back(str_Security_ID);
		string str_Account_Name = CW2A(Account_Name);					prepart.push_back(str_Account_Name);
		string str_Account_Domain = CW2A(Account_Domain);				prepart.push_back(str_Account_Domain);
		string str_Logon_ID = CW2A(Logon_ID);							prepart.push_back(str_Logon_ID);
		string str_Logon_GUID = CW2A(Logon_GUID);						prepart.push_back(str_Logon_GUID);
	}
	else// if(summarizationLevel == 1)
	{
		//string str_Security_ID = CW2A(Security_ID);						prepart.push_back(str_Security_ID);
		string str_Account_Name = CW2A(Account_Name);					prepart.push_back(str_Account_Name);
		string str_Account_Domain = CW2A(Account_Domain);				prepart.push_back(str_Account_Domain);
		string str_Logon_ID = CW2A(Logon_ID);							prepart.push_back(str_Logon_ID);
		//string str_Logon_GUID = CW2A(Logon_GUID);						prepart.push_back(str_Logon_GUID);
	}
	return prepart;
}

void MyNewLogon::print()
{
	wprintf(L"New Logon:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Security_ID: %s\n", Security_ID);
	wprintf(L"Account_Name: %s\n", Account_Name);
	wprintf(L"Account_Domain: %s\n", Account_Domain);
	wprintf(L"Logon_ID: %s\n", Logon_ID);
	wprintf(L"Logon_GUID: %s\n", Logon_GUID);
}

MyNewLogon::~MyNewLogon(void)
{

}