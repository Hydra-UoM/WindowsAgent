#include "stdafx.h"
#include "MyObject.h"

MyObject::MyObject(const wchar_t* Object_Server1,const wchar_t* Object_Type1,const wchar_t* Object_Name1,const wchar_t* Handle_ID1,
			const wchar_t* Resource_Attributes1):
			Object_Server(Object_Server1),Object_Type(Object_Type1),Object_Name(Object_Name1),Handle_ID(Handle_ID1),
			Resource_Attributes(Resource_Attributes1)
{
	    Object_Server = Object_Server1;
	    Object_Type = Object_Type1;
		Object_Name = Object_Name1;
		Handle_ID = Handle_ID1;
		Resource_Attributes = Resource_Attributes1;
}

void MyObject::print()
{
	wprintf(L"Object:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Object_Server: %s\n", Object_Server);
	wprintf(L"Object_Type: %s\n", Object_Type);
	wprintf(L"Object_Name: %s\n", Object_Name);
	wprintf(L"Handle_ID: %s\n", Handle_ID);
	wprintf(L"Resource_Attributes: %s\n", Resource_Attributes);
}

MyObject::~MyObject(void)
{

}