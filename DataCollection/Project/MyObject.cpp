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

myObject MyObject::toLogStruct(int summarizationLevel)
{
	myObject m;
	if (summarizationLevel == 0)
	{
		m.Object_Server = CW2A(Object_Server);				
		m.Object_Type = CW2A(Object_Type);					
		m.Object_Name = CW2A(Object_Name);					
		m.Handle_ID = CW2A(Handle_ID);						
		m.Resource_Attributes = CW2A(Resource_Attributes);
	}
	else// if(summarizationLevel == 1)
	{
		m.Object_Server = CW2A(Object_Server);
		m.Object_Type = CW2A(Object_Type);
		m.Object_Name = CW2A(Object_Name);
		//m.Handle_ID = CW2A(Handle_ID);
		//m.Resource_Attributes = CW2A(Resource_Attributes);
	}
	return m;
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