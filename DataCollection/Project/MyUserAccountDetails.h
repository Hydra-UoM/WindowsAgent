#include "stdafx.h"
#ifndef MyUserAccountDetails_H
#define MyUserAccountDetails_H

#include "MyUserAccountDetailsStructure.h"

class MyUserAccountDetails
{
public:
	MyUserAccountDetails();
	list<string>getCurrentLoggedOnUserInformation();
	void printError(TCHAR* msg);
	list<string>getAllUserInformation();
	~MyUserAccountDetails(void);
};
#endif