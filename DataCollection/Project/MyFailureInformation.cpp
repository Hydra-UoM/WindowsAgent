/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyFailureInformation.h"

MyFailureInformation::MyFailureInformation(const wchar_t* Failure_Reason1, const wchar_t* Status1, const wchar_t* Sub_Status1) :
Failure_Reason(Failure_Reason1), Status(Status1), Sub_Status(Sub_Status1)
{
	Failure_Reason = Failure_Reason1;
	Status = Status1;
	Sub_Status = Sub_Status1;
}

myFailureInformation MyFailureInformation::toLogStruct(int summarizationLevel)
{
	myFailureInformation m;
	if (summarizationLevel == 0)
	{
		m.Failure_Reason = CW2A(Failure_Reason);
		m.Status = CW2A(Status);
		m.Sub_Status = CW2A(Sub_Status);			
	}
	else// if(summarizationLevel == 1)
	{
		m.Failure_Reason = CW2A(Failure_Reason);
		m.Status = CW2A(Status);
		//m.Sub_Status = CW2A(Sub_Status);
	}
	return m;
}

void MyFailureInformation::print()
{
	wprintf(L"Failure Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Security_ID: %s\n", Failure_Reason);
	wprintf(L"Account_Name: %s\n", Status);
	wprintf(L"Account_Domain: %s\n", Sub_Status);
}

MyFailureInformation::~MyFailureInformation(void)
{

}