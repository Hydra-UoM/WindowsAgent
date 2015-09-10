#include "stdafx.h"
#ifndef MyUserAccountDetails_H
#define MyUserAccountDetails_H

#include "MyUserAccountDetailsStructure.h"

class MyUserAccountDetails
{
public:
	MyUserAccountDetails();
	void getCurrentLoggedOnUserInformation();
	void printError(TCHAR* msg);
	void getAllUserInformation();
	~MyUserAccountDetails(void);
};
#endif