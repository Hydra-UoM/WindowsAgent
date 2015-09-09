/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyApplicationInformation.h"

MyApplicationInformation::MyApplicationInformation(const wchar_t* Application_Process_ID1,const wchar_t* Application_Name1):
			Application_Process_ID(Application_Process_ID1),Application_Name(Application_Name1)
{
	    Application_Process_ID = Application_Process_ID1;
	    Application_Name = Application_Name1;
}

void MyApplicationInformation::print()
{
	wprintf(L"Application_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Application_Process_ID: %s\n", Application_Process_ID);
	wprintf(L"Application_Name: %s\n", Application_Name);
}

MyApplicationInformation::~MyApplicationInformation(void)
{

}