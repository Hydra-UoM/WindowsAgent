/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyLogonType.h"

MyLogonType::MyLogonType(const wchar_t* Logon_Type1) :
Logon_Type(Logon_Type1)
{
	Logon_Type = Logon_Type1;
}

myLogonType MyLogonType::toLogStruct(int summarizationLevel)
{
	myLogonType m;
	if (summarizationLevel == 0)
	{
		m.Logon_Type = CW2A(Logon_Type);
	}
	else// if(summarizationLevel == 1)
	{
		m.Logon_Type = CW2A(Logon_Type);
	}
	return m;
}

void MyLogonType::print()
{
	wprintf(L"Logon Type:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Logon_Type: %s\n", Logon_Type);
}

MyLogonType::~MyLogonType(void)
{

}