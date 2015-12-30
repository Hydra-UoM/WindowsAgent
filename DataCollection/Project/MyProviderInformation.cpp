#include "stdafx.h"
#include "MyProviderInformation.h"

MyProviderInformation::MyProviderInformation(const wchar_t* Provider_ID1,const wchar_t* Provider_Name1):
			Provider_ID(Provider_ID1),Provider_Name(Provider_Name1)
{
	    Provider_ID = Provider_ID1;
	    Provider_Name = Provider_Name1;
}

myProviderInformation MyProviderInformation::toLogStruct(int summarizationLevel)
{
	myProviderInformation m;
	if (summarizationLevel == 0)
	{
		m.Provider_ID = CW2A(Provider_ID);						
		m.Provider_Name = CW2A(Provider_Name);					
	}
	else
	{
		//m.Provider_ID = CW2A(Provider_ID);
		m.Provider_Name = CW2A(Provider_Name);
	}
	return m;
}

void MyProviderInformation::print()
{
	wprintf(L"MyProviderInformation:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Provider_ID: %s\n", Provider_ID);
	wprintf(L"Provider_Name: %s\n", Provider_Name);
}

MyProviderInformation::~MyProviderInformation(void)
{

}