#pragma once
#include "RegisterDeviceService.h"
#include <stdio.h>
#include <stdlib.h>
#include "DBHandler.h"
#include "sqlite3.h"
#include "Manager.h"

class DBHandler
{
public:
	DBHandler();
	~DBHandler();
	void openDB();
	void createTable();
	void insertData(vector<HydraCN::ThriftAgentProcessInfo> procF);
	void deleteData();
	void viewData();
	vector<HydraCN::ThriftAgentProcessInfo> sendData();
};


