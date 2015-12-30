/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyImpersonationLevel.h"

MyImpersonationLevel::MyImpersonationLevel(const wchar_t* Impersonation_Level1) :
Impersonation_Level(Impersonation_Level1)
{
	Impersonation_Level = Impersonation_Level1;
}

list<string>MyImpersonationLevel::toLogString(list<string>prepart, int summarizationLevel)
{
	prepart.push_back("Logon Type");
	if (summarizationLevel == 0)
	{
		string str_Impersonation_Level = CW2A(Impersonation_Level);						prepart.push_back(str_Impersonation_Level);
	}
	else// if(summarizationLevel == 1)
	{
		string str_Impersonation_Level = CW2A(Impersonation_Level);						prepart.push_back(str_Impersonation_Level);
	}
	return prepart;
}

void MyImpersonationLevel::print()
{
	wprintf(L"Impersonation Level:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Impersonation_Level: %s\n", Impersonation_Level);
}

MyImpersonationLevel::~MyImpersonationLevel(void)
{

}