#include "stdafx.h"
#include "MyProcessInformation.h"

MyProcessInformation::MyProcessInformation(const wchar_t* Process_ID1,const wchar_t* Process_Name1):
			Process_ID(Process_ID1),Process_Name(Process_Name1)
{
	    Process_ID = Process_ID1;
	    Process_Name = Process_Name1;
}

myProcessInformation MyProcessInformation::toLogStruct(int summarizationLevel)
{
	myProcessInformation m;
	if (summarizationLevel == 0)
	{
		m.Process_ID = CW2A(Process_ID);
		m.Process_Name = CW2A(Process_Name);
	}
	else// if(summarizationLevel == 1)
	{
		//m.Process_ID = CW2A(Process_ID);
		m.Process_Name = CW2A(Process_Name);
	}
	return m;
}

void MyProcessInformation::print()
{
	wprintf(L"Process_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Process_ID: %s\n", Process_ID);
	wprintf(L"Process_Name: %s\n", Process_Name);
}

MyProcessInformation::~MyProcessInformation(void)
{

}