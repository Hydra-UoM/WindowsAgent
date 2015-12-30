#include "stdafx.h"
#include "MyLayerInformation.h"

MyLayerInformation::MyLayerInformation(const wchar_t* Layer_ID1,const wchar_t* Layer_Name1,const wchar_t* Layer_Runtime_ID1):
			Layer_ID(Layer_ID1),Layer_Name(Layer_Name1),Layer_Runtime_ID(Layer_Runtime_ID1)
{
	    Layer_ID = Layer_ID1;
	    Layer_Name = Layer_Name1;
		Layer_Runtime_ID = Layer_Runtime_ID1;
}

myLayerInformation MyLayerInformation::toLogStruct(int summarizationLevel)
{
	myLayerInformation m;
	if (summarizationLevel == 0)
	{
		m.Layer_ID = CW2A(Layer_ID);							
		m.Layer_Name = CW2A(Layer_Name);						
		m.Layer_Runtime_ID = CW2A(Layer_Runtime_ID);			
	}
	else
	{
		//m.Layer_ID = CW2A(Layer_ID);
		m.Layer_Name = CW2A(Layer_Name);
		//m.Layer_Runtime_ID = CW2A(Layer_Runtime_ID);
	}
	return m;
}

void MyLayerInformation::print()
{
	wprintf(L"Layer_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Layer_ID: %s\n", Layer_ID);
	wprintf(L"Layer_Name: %s\n", Layer_Name);
	wprintf(L"Layer_Runtime_ID: %s\n", Layer_Runtime_ID);
}

MyLayerInformation::~MyLayerInformation(void)
{

}