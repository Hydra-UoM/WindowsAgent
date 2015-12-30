#include "stdafx.h"
#include "MyAccessRequestInformation.h"

MyAccessRequestInformation::MyAccessRequestInformation(const wchar_t* Transaction_ID1,const wchar_t* Accesses1,const wchar_t* Access_Reasons1,const wchar_t* Access_Mask1,
					const wchar_t* Privileges_Used_For_Access_Check1,const wchar_t* Restricted_SID_Count1):
			Transaction_ID(Transaction_ID1),Accesses(Accesses1),Access_Reasons(Access_Reasons1),Access_Mask(Access_Mask1),
			Privileges_Used_For_Access_Check(Privileges_Used_For_Access_Check1),Restricted_SID_Count(Restricted_SID_Count1)
{
	    Transaction_ID = Transaction_ID1;
	    Accesses = Accesses1;
		Access_Reasons = Access_Reasons1;
		Access_Mask = Access_Mask1;
		Privileges_Used_For_Access_Check = Privileges_Used_For_Access_Check1;
		Restricted_SID_Count = Restricted_SID_Count1;
}

myAccessRequestInformation MyAccessRequestInformation::toLogStruct(int summarizationLevel)
{
	myAccessRequestInformation m;
	if (summarizationLevel == 0)
	{
		m.Transaction_ID = CW2A(Transaction_ID);										
		m.Accesses = CW2A(Accesses);													
		m.Access_Reasons = CW2A(Access_Reasons);										
		m.Access_Mask = CW2A(Access_Mask);												
		m.Privileges_Used_For_Access_Check = CW2A(Privileges_Used_For_Access_Check);	
		m.Restricted_SID_Count = CW2A(Restricted_SID_Count);							
	}
	else
	{
		//m.Transaction_ID = CW2A(Transaction_ID);
		m.Accesses = CW2A(Accesses);
		m.Access_Reasons = CW2A(Access_Reasons);
		//m.Access_Mask = CW2A(Access_Mask);
		m.Privileges_Used_For_Access_Check = CW2A(Privileges_Used_For_Access_Check);
		//m.Restricted_SID_Count = CW2A(Restricted_SID_Count);
	}
	return m;
}

void MyAccessRequestInformation::print()
{
	wprintf(L"Access_Request_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Transaction_ID: %s\n", Transaction_ID);
	wprintf(L"Accesses: %s\n", Accesses);
	wprintf(L"Access_Reasons: %s\n", Access_Reasons);
	wprintf(L"Access_Mask: %s\n", Access_Mask);
	wprintf(L"Privileges_Used_For_Access_Check: %s\n", Privileges_Used_For_Access_Check);
	wprintf(L"Restricted_SID_Count: %s\n", Restricted_SID_Count);
}

MyAccessRequestInformation::~MyAccessRequestInformation(void)
{

}