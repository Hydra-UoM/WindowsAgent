/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyErrorInformation.h"

MyErrorInformation::MyErrorInformation(const wchar_t* Reason1):
			Reason(Reason1)
{
	    Reason = Reason1;
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