#include "stdafx.h"

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>

#include <sddl.h>


#pragma comment(lib, "wevtapi.lib")

class MyTimeCreated
{
	public:
		MyTimeCreated();
			
		~MyTimeCreated(void);

	//private:
};