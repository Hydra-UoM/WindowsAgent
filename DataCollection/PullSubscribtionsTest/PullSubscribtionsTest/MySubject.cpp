/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MySubject.h"

MySubject::MySubject(const wchar_t* Security_ID1,const wchar_t* Account_Name1,const wchar_t* Account_Domain1,const wchar_t* Logon_ID1):
			Security_ID(Security_ID1),Account_Name(Account_Name1),Account_Domain(Account_Domain1),Logon_ID(Logon_ID1)
{
	    Security_ID = Security_ID1;
	    Account_Name = Account_Name1;
		Account_Domain = Account_Domain1;
		Logon_ID = Logon_ID1;
}

void MySubject::print()
{
	wprintf(L"Subject:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Security_ID: %s\n", Security_ID);
	wprintf(L"Account_Name: %s\n", Account_Name);
	wprintf(L"Account_Domain: %s\n", Account_Domain);
	wprintf(L"Logon_ID: %s\n", Logon_ID);
}

MySubject::~MySubject(void)
{

}