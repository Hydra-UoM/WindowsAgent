/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyRuleInformation.h"

MyRuleInformation::MyRuleInformation(const wchar_t* ID1,const wchar_t* Name1):
			ID(ID1),Name(Name1)
{
	    ID = ID1;
	    Name = Name1;
}

myRuleInformation MyRuleInformation::toLogStruct(int summarizationLevel)
{
	myRuleInformation m;
	if (summarizationLevel == 0)
	{
		m.ID = CW2A(ID);						
		m.Name = CW2A(Name);					
	}
	else
	{
		//m.ID = CW2A(ID);
		m.Name = CW2A(Name);
	}
	return m;
}

void MyRuleInformation::print()
{
	wprintf(L"MyRuleInformation:\n");
	wprintf(L"*********************************\n");
	wprintf(L"ID: %s\n", ID);
	wprintf(L"Name: %s\n", Name);
}

MyRuleInformation::~MyRuleInformation(void)
{

}