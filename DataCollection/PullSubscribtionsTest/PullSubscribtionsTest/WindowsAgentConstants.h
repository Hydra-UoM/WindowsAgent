#include "stdafx.h"
#include <iostream>

using namespace std;

#ifndef WindowsAgentConstants_H
#define WindowsAgentConstants_H

enum SecurityLevelRelationshipConstraints
{
	LESS_THAN,LESS_THAN_OR_EQUAL_TO,GREATER_THAN,GREATER_THAN_OR_EQUAL_TO,EQUAL_TO,ALL
};

string MY_SECURITY = "Security";
string MY_APPLICATION = "Application";
string MY_SYSTEM = "System";
string MY_SETUP = "Setup";
string MY_OPERATIONAL = "Operational";

#endif