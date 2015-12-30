#include "stdafx.h"
#include "MyFilterInformation.h"

MyFilterInformation::MyFilterInformation(const wchar_t* Filter_ID1,const wchar_t* Filter_Name1,const wchar_t* Filter_Type1,
										 const wchar_t* Runtime_ID1,const wchar_t* Filter_Runtime_ID1,const wchar_t* Filter_Layer_Name1,
										 const wchar_t* Filter_Layer_Runtime_ID1):
			Filter_ID(Filter_ID1),Filter_Name(Filter_Name1),Filter_Type(Filter_Type1),Runtime_ID(Runtime_ID1),
			Filter_Runtime_ID(Filter_Runtime_ID1), Filter_Layer_Name(Filter_Layer_Name1),Filter_Layer_Runtime_ID(Filter_Layer_Runtime_ID1)
{
	    Filter_ID = Filter_ID1;
	    Filter_Name = Filter_Name1;
		Filter_Type = Filter_Type1;
		Runtime_ID = Runtime_ID1;
		Filter_Runtime_ID = Filter_Runtime_ID1;
		Filter_Layer_Name = Filter_Layer_Name1;
		Filter_Layer_Runtime_ID = Filter_Layer_Runtime_ID1;
}

myFilterInformation MyFilterInformation::toLogStruct(int summarizationLevel)
{
	myFilterInformation m;
	if (summarizationLevel == 0)
	{
		m.Filter_ID = CW2A(Filter_ID);								
		m.Filter_Name = CW2A(Filter_Name);							
		m.Filter_Type = CW2A(Filter_Type);							
		m.Runtime_ID = CW2A(Runtime_ID);							
		m.Filter_Runtime_ID = CW2A(Filter_Runtime_ID);				
		m.Filter_Layer_Name = CW2A(Filter_Layer_Name);				
		m.Filter_Layer_Runtime_ID = CW2A(Filter_Layer_Runtime_ID);	
	}
	else
	{
		//m.Filter_ID = CW2A(Filter_ID);
		m.Filter_Name = CW2A(Filter_Name);
		m.Filter_Type = CW2A(Filter_Type);
		//m.Runtime_ID = CW2A(Runtime_ID);
		//m.Filter_Runtime_ID = CW2A(Filter_Runtime_ID);
		m.Filter_Layer_Name = CW2A(Filter_Layer_Name);
		//m.Filter_Layer_Runtime_ID = CW2A(Filter_Layer_Runtime_ID);
	}
	return m;
}

void MyFilterInformation::print()
{
	wprintf(L"Filter_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Filter_ID: %s\n", Filter_ID);
	wprintf(L"Filter_Name: %s\n", Filter_Name);
	wprintf(L"Filter_Type: %s\n", Filter_Type);
	wprintf(L"Runtime_ID: %s\n", Runtime_ID);
	wprintf(L"Filter_Runtime_ID: %s\n", Filter_Runtime_ID);
	wprintf(L"Filter_Layer_Name: %s\n", Filter_Layer_Name);
	wprintf(L"Filter_Layer_Runtime_ID: %s\n", Filter_Layer_Runtime_ID);
}

MyFilterInformation::~MyFilterInformation(void)
{

}