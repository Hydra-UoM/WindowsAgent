#include "stdafx.h"
#include "MyProviderInformation.h"

MyProviderInformation::MyProviderInformation(const wchar_t* Provider_ID1,const wchar_t* Provider_Name1):
			Provider_ID(Provider_ID1),Provider_Name(Provider_Name1)
{
	    Provider_ID = Provider_ID1;
	    Provider_Name = Provider_Name1;
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