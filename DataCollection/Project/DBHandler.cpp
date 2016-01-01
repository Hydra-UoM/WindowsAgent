#include "RegisterDeviceService.h"
#include <stdio.h>
#include <stdlib.h>
#include "DBHandler.h"
#include "sqlite3.h"
#include "Manager.h"


sqlite3 *db;
char *zErrMsg = 0;
int  rc;
char *sql;
sqlite3_stmt *res;


DBHandler::DBHandler()
{
}


DBHandler::~DBHandler()
{
}

int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


void DBHandler::openDB(){
	rc = sqlite3_open("processlist.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}
}

void DBHandler::createTable(){
	openDB();
	//Create SQL statement 
	sql = "CREATE TABLE  IF NOT EXISTS PerformData("  \
		"NAME         TEXT," \
		"CPU          REAL,"\
		"Memory       REAL,"\
		"Upload       REAL,"\
		"Download       REAL,"\
		"MAC       TEXT,"\
		"Type      TEXT,"\
		"Time      TEXT,"\
		"PID    TEXT); ";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
}

void DBHandler::insertData(vector<HydraCN::ThriftAgentProcessInfo> procF){
	Manager manage;

	sqlite3_stmt *pointer;
	int k = 1;
	char *q; // query
	//char fp[MAX_PATH];
	createTable();
	q = "Insert into PerformData (Name, CPU, Memory, Upload, Download, MAC, Type, Time, PID) VALUES(?1,?2,?3,?4,?5,?6,?7,?8,?9)";
	for (auto i : procF)
	{
		if (sqlite3_prepare_v2(db, q, strlen(q), &pointer, 0) == SQLITE_OK){
			//cout<<"\n\n inside loop i values is "<<i<<"\n\n";
			sqlite3_bind_text(pointer, 1, i.name.c_str(), strlen(i.name.c_str()), 0); //Process Name 
			sqlite3_bind_double(pointer, 2, i.cpuUsage);
			sqlite3_bind_double(pointer, 3, i.ramUsage);
			sqlite3_bind_double(pointer, 4, i.sentData);
			sqlite3_bind_double(pointer, 5, i.receiveData);
			sqlite3_bind_text(pointer, 6, i.mac.c_str(), strlen(i.mac.c_str()), 0);
			sqlite3_bind_text(pointer, 7, i.type.c_str(), strlen(i.type.c_str()), 0);
			sqlite3_bind_text(pointer, 8, i.timestamp.c_str(), strlen(i.timestamp.c_str()), 0);
			sqlite3_bind_text(pointer, 9, i.pid.c_str(), strlen(i.pid.c_str()), 0);
			sqlite3_step(pointer);   // prepare statemnt Ready 
			sqlite3_reset(pointer);
		}
		sqlite3_finalize(pointer);
	}

	printf("Data inserted successfully");

}

void DBHandler::deleteData(){

	openDB();
	sql = "DELETE from PerformData; " \
		"SELECT * from PerformData";

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Data deleted successfully\n");
	}
	sqlite3_close(db);

}

void DBHandler::viewData(){
	openDB();
	sql = "SELECT * from PerformData";

	// Execute SQL statement 
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Saved Process Details \n");
	}
	sqlite3_close(db);
}
vector<HydraCN::ThriftAgentProcessInfo> DBHandler::sendData(){
	openDB();
	string get_data_qry = "SELECT * FROM PerformData";
	std::vector<HydraCN::ThriftAgentProcessInfo> processList;
	sqlite3_stmt *pointer;

	if (sqlite3_prepare_v2(db, ((char*)get_data_qry.c_str()), -1, &pointer, 0) == SQLITE_OK)
	{
		int result = sqlite3_step(pointer);
		while (result == SQLITE_ROW)
		{
			HydraCN::ThriftAgentProcessInfo pro;
			pro.name = (char*)sqlite3_column_text(pointer, 0);
			pro.cpuUsage = sqlite3_column_double(pointer, 1);
			pro.ramUsage = sqlite3_column_double(pointer, 2);
			pro.sentData = sqlite3_column_double(pointer, 3);
			pro.receiveData = sqlite3_column_double(pointer, 4);
			pro.mac = (char*)sqlite3_column_text(pointer, 5);
			pro.type= (char*)sqlite3_column_text(pointer, 6);
			pro.timestamp = (char*)sqlite3_column_text(pointer, 7);
			pro.pid = sqlite3_column_double(pointer, 8);
			result = sqlite3_step(pointer);
			processList.push_back(pro);
		}
	}
	sqlite3_close(db);
	return processList;
}