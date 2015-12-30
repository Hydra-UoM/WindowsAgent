/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyAccountForWhichLogonFailed.h"

MyAccountForWhichLogonFailed::MyAccountForWhichLogonFailed(const wchar_t* Security_ID1, const wchar_t* Account_Name1, const wchar_t* Account_Domain1) :
Security_ID(Security_ID1), Account_Name(Account_Name1), Account_Domain(Account_Domain1)
{
	Security_ID = Security_ID1;
	Account_Name = Account_Name1;
	Account_Domain = Account_Domain1;
}

myAccountForWhichLogonFailed MyAccountForWhichLogonFailed::toLogStruct(int summarizationLevel)
{
	myAccountForWhichLogonFailed m;
	if (summarizationLevel == 0)
	{
		m.Security_ID = CW2A(Security_ID);	
		m.Account_Name = CW2A(Account_Name);					
		m.Account_Domain = CW2A(Account_Domain);			
	}
	else// if(summarizationLevel == 1)
	{
		//m.Security_ID = CW2A(Security_ID);
		m.Account_Name = CW2A(Account_Name);
		m.Account_Domain = CW2A(Account_Domain);
	}
	return m;
}

void MyAccountForWhichLogonFailed::print()
{
	wprintf(L"Account For Which Logon Failed:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Security_ID: %s\n", Security_ID);
	wprintf(L"Account_Name: %s\n", Account_Name);
	wprintf(L"Account_Domain: %s\n", Account_Domain);
}

MyAccountForWhichLogonFailed::~MyAccountForWhichLogonFailed(void)
{

}