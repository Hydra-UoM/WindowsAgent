#include "stdafx.h"
#include "MyChangeInformation.h"

MyChangeInformation::MyChangeInformation(const wchar_t* Change_Type1):
			Change_Type(Change_Type1)
{
	    Change_Type = Change_Type1;
}

void MyChangeInformation::print()
{
	wprintf(L"Change_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Change_Type: %s\n", Change_Type);
}

MyChangeInformation::~MyChangeInformation(void)
{

}