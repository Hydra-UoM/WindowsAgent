#include "stdafx.h"
#ifndef MyUserAccountDetails_H
#define MyUserAccountDetails_H

#include "MyUserAccountDetailsStructure.h"

class MyUserAccountDetails
{
public:
	MyUserAccountDetails();
	myStruct::myUserAccountDetailsStruct getCurrentLoggedOnUserInformation(int summarizationLevel);
	void printError(TCHAR* msg);
	std::vector<myStruct::myUserAccountDetailsStruct>getAllUserInformation(int summarizationLevel);
	~MyUserAccountDetails(void);
};
#endif