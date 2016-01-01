#ifndef Manager_H
#define Manager_H

#include "CPU.h"
#include "Memory.h"
#include "IO.h"
#include "Data.h"
#include <tchar.h>
#include <atlstr.h>
#define INFO_BUFFER_SIZE 32767
extern bool tRetired;
class Manager :public CPU, public Memory, public IO
{
public:
	Manager();
	virtual ~Manager();
	string ConfigFile();
	void Start();

	vector<ProcessF> FilterAllProcesses(double value1, double value2, double value3, double value4);
	static void FilterAllAvgProcesses(int samples, double value1, double value2, double value3, double value4, vector<string> processList);
	ProcessF GetAvgProcess_PID(int PID, int sample);
	void deviceClient();
	vector<ProcessF> GetAllProcesses();
	static void retire();
	static void fullData(int time);
	static void currentData(int time);
	static void importantData(int time);
	static void sendStoredData();
	string getMAC();
	string getIP();
	string getTime(); 
	string getComputerName();
	//void printError(TCHAR* msg);
};

#endif