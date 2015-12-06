#include "stdafx.h"
#ifndef MyNetworkInformation_H
#define MyNetworkInformation_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyNetworkInformation
{
	public:
		MyNetworkInformation(const wchar_t* Direction1,const wchar_t* Source_Address1,const wchar_t* Source_Port1,
			const wchar_t* Destination_Address1,const wchar_t* Destination_Port1,const wchar_t* Protocol1);
		~MyNetworkInformation(void);

		void print();

		const wchar_t* Direction;
		const wchar_t* Source_Address;
		const wchar_t* Source_Port;
		const wchar_t* Destination_Address;
		const wchar_t* Destination_Port;
		const wchar_t* Protocol;
};
#endif