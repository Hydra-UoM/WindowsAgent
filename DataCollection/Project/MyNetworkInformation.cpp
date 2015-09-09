/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyNetworkInformation.h"

MyNetworkInformation::MyNetworkInformation(const wchar_t* Direction1,const wchar_t* Source_Address1,const wchar_t* Source_Port1,
										   const wchar_t* Destination_Address1,const wchar_t* Destination_Port1,
										   const wchar_t* Protocol1):
			Direction(Direction1),Source_Address(Source_Address1),Source_Port(Source_Port1),Destination_Address(Destination_Address1),
			Destination_Port(Destination_Port1),Protocol(Protocol1)
{
	    Direction = Direction1;
	    Source_Address = Source_Address1;
		Source_Port = Source_Port1;
		Destination_Address = Destination_Address1;
		Destination_Port = Destination_Port1;
		Protocol = Protocol1;
}

void MyNetworkInformation::print()
{
	wprintf(L"Network_Information:\n");
	wprintf(L"*********************************\n");
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