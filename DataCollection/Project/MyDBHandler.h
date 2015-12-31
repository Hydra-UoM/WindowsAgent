#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string>
#include <iostream>
#include <vector>
#include "MyLogStruct.h"

class MyDBHandler
{
public:
	MyDBHandler();
	~MyDBHandler();
	static int query(char* qry);
	static bool openDB();
	static void createLogTable();
	static void createUsersTable();
	static void createCurrentUserTable();
	static void storeLogData(std::vector<myStruct::myLogStructure>logList);
	static void storeUsersData(std::vector<myStruct::myUserAccountDetailsStruct>usersList);
	static void storeCurrentUserData(myStruct::myUserAccountDetailsStruct currentUser);
	static std::vector<myStruct::myUserAccountDetailsStruct> retrieveCurrentUserData();
	static std::vector<myStruct::myUserAccountDetailsStruct> retrieveUsersData();
	static std::vector<myStruct::myLogStructure> retrieveLogData();
	static void deleteLogData();
	static void deleteCurrentUserData();
	static void deleteUsersData();
	static void close();
	static bool isAvailableLogData;
	static bool isAvailableUsersData;
	static bool isAvailableCurrentUserData;
};