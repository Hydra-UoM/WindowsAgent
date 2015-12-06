#include "stdafx.h"
#include "MyProcessInformation.h"

MyProcessInformation::MyProcessInformation(const wchar_t* Process_ID1,const wchar_t* Process_Name1):
			Process_ID(Process_ID1),Process_Name(Process_Name1)
{
	    Process_ID = Process_ID1;
	    Process_Name = Process_Name1;
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