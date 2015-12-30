#include "stdafx.h"
#ifndef MyNetworkInformation_H
#define MyNetworkInformation_H

#include <stdio.h>
#include <string>
#include <list>
#include <atlstr.h>
#include "MyLogStruct.h"
using namespace myStruct;
using namespace std;

class MyNetworkInformation
{
	public:
		MyNetworkInformation(const wchar_t* Workstation_Name1,const wchar_t* Direction1, const wchar_t* Source_Address1, const wchar_t* Source_Port1,
			const wchar_t* Destination_Address1,const wchar_t* Destination_Port1,const wchar_t* Protocol1);
		myNetworkInformation toLogStruct(int summarizationLevel);
		~MyNetworkInformation(void);

		void print();

		const wchar_t* Direction;
		const wchar_t* Source_Address;
		const wchar_t* Source_Port;
		const wchar_t* Destination_Address;
		const wchar_t* Destination_Port;
		const wchar_t* Protocol;
		const wchar_t* Workstation_Name;
};
#endif