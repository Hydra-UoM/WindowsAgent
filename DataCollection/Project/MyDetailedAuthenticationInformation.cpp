/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyDetailedAuthenticationInformation.h"

MyDetailedAuthenticationInformation::MyDetailedAuthenticationInformation(const wchar_t* Logon_Process1, const wchar_t* Authentication_Package1, const wchar_t* Transited_Services1, const wchar_t* Package_Name1, const wchar_t* Key_Length1) :
Logon_Process(Logon_Process1), Authentication_Package(Authentication_Package1), Transited_Services(Transited_Services1), Package_Name(Package_Name1), Key_Length(Key_Length1)
{
	Logon_Process = Logon_Process1;
	Authentication_Package = Authentication_Package1;
	Transited_Services = Transited_Services1;
	Package_Name = Package_Name1;
	Key_Length = Key_Length1;
}

myDetailedAuthenticationInformation MyDetailedAuthenticationInformation::toLogStruct(int summarizationLevel)
{
	myDetailedAuthenticationInformation m;
	if (summarizationLevel == 0)
	{
		m.Logon_Process = CW2A(Logon_Process);						
		m.Authentication_Package = CW2A(Authentication_Package);	
		m.Transited_Services = CW2A(Transited_Services);			
		m.Package_Name = CW2A(Package_Name);						
		m.Key_Length = CW2A(Key_Length);							
	}
	else// if(summarizationLevel == 1)
	{
		m.Logon_Process = CW2A(Logon_Process);
		m.Authentication_Package = CW2A(Authentication_Package);
		m.Transited_Services = CW2A(Transited_Services);
		//m.Package_Name = CW2A(Package_Name);
		//m.Key_Length = CW2A(Key_Length);
	}
	return m;
}

void MyDetailedAuthenticationInformation::print()
{
	wprintf(L"Detailed Authentication Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Logon_Process: %s\n", Logon_Process);
	wprintf(L"Authentication_Package: %s\n", Authentication_Package);
	wprintf(L"Transited_Services: %s\n", Transited_Services);
	wprintf(L"Package_Name: %s\n", Package_Name);
	wprintf(L"Key_Length: %s\n", Key_Length);
}

MyDetailedAuthenticationInformation::~MyDetailedAuthenticationInformation(void)
{

}