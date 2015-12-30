/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyErrorInformation.h"

MyErrorInformation::MyErrorInformation(const wchar_t* Reason1):
			Reason(Reason1)
{
	    Reason = Reason1;
}

myErrorInformation MyErrorInformation::toLogStruct(int summarizationLevel)
{
	myErrorInformation m;
	if (summarizationLevel == 0)
	{
		m.Reason = CW2A(Reason);				
	}
	else
	{
		m.Reason = CW2A(Reason);
	}
	return m;
}

void MyErrorInformation::print()
{
	wprintf(L"MyErrorInformation:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Reason: %s\n", Reason);
}

MyErrorInformation::~MyErrorInformation(void)
{

}