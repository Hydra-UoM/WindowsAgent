/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyNetworkInformation.h"

MyNetworkInformation::MyNetworkInformation(const wchar_t* Workstation_Name1,const wchar_t* Direction1, const wchar_t* Source_Address1, const wchar_t* Source_Port1,
										   const wchar_t* Destination_Address1,const wchar_t* Destination_Port1,
										   const wchar_t* Protocol1):
			Workstation_Name(Workstation_Name1),
			Direction(Direction1),Source_Address(Source_Address1),Source_Port(Source_Port1),Destination_Address(Destination_Address1),
			Destination_Port(Destination_Port1),Protocol(Protocol1)
{
	Workstation_Name = Workstation_Name1;
	Direction = Direction1;
	Source_Address = Source_Address1;
	Source_Port = Source_Port1;
	Destination_Address = Destination_Address1;
	Destination_Port = Destination_Port1;
	Protocol = Protocol1;
}

myNetworkInformation MyNetworkInformation::toLogStruct(int summarizationLevel)
{
	myNetworkInformation m;
	if (summarizationLevel == 0)
	{
		m.Workstation_Name = CW2A(Workstation_Name);				
		m.Direction = CW2A(Direction);								
		m.Source_Address = CW2A(Source_Address);					
		m.Source_Port = CW2A(Source_Port);							
		m.Destination_Address = CW2A(Destination_Address);			
		m.Destination_Port = CW2A(Destination_Port);				
		m.Protocol = CW2A(Protocol);								
	}
	else
	{
		m.Workstation_Name = CW2A(Workstation_Name);
		//m.Direction = CW2A(Direction);
		//m.Source_Address = CW2A(Source_Address);
		m.Source_Port = CW2A(Source_Port);
		m.Destination_Address = CW2A(Destination_Address);
		m.Destination_Port = CW2A(Destination_Port);
		m.Protocol = CW2A(Protocol);
	}
	return m;
}

void MyNetworkInformation::print()
{
	wprintf(L"Network_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Workstation_Name: %s\n", Workstation_Name);
	wprintf(L"Direction: %s\n", Direction);
	wprintf(L"Source_Address: %s\n", Source_Address);
	wprintf(L"Source_Port: %s\n", Source_Port);
	wprintf(L"Destination_Address: %s\n", Destination_Address);
	wprintf(L"Destination_Port: %s\n", Destination_Port);
	wprintf(L"Protocol: %s\n", Protocol);
}

MyNetworkInformation::~MyNetworkInformation(void)
{

}