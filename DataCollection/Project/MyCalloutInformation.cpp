#include "stdafx.h"
#include "MyCalloutInformation.h"

MyCalloutInformation::MyCalloutInformation(const wchar_t* Callout_ID1,const wchar_t* Callout_Name1):
			Callout_ID(Callout_ID1),Callout_Name(Callout_Name1)
{
	    Callout_ID = Callout_ID1;
	    Callout_Name = Callout_Name1;
}

myCalloutInformation MyCalloutInformation::toLogStruct(int summarizationLevel)
{
	myCalloutInformation m;
	if(summarizationLevel == 0)
	{
		m.Callout_ID = CW2A(Callout_ID);						
		m.Callout_Name = CW2A(Callout_Name);					
	}
	else
	{
		//m.Callout_ID = CW2A(Callout_ID);
		m.Callout_Name = CW2A(Callout_Name);
	}
	return m;
}

void MyCalloutInformation::print()
{
	wprintf(L"Callout_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Callout_ID: %s\n", Callout_ID);
	wprintf(L"Callout_Name: %s\n", Callout_Name);
}

MyCalloutInformation::~MyCalloutInformation(void)
{

}