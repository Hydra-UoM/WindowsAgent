#pragma once
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
	void insertData();
	void deleteData();
	void viewData();
	vector<ProcessF> sendData();
};


