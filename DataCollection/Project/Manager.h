#ifndef Manager_H
#define Manager_H

#include "CPU.h"
#include "Memory.h"
#include "IO.h"
#include "Data.h"

class Manager:public CPU,public Memory,public IO
{
public:
	Manager();
	virtual ~Manager();
	void Start();
	vector<ProcessF> FilterAllProcesses(double value1, double value2, double value3,double value4);
	vector<ProcessF> FilterAllAvgProcesses(int samples, double value1, double value2, double value3, double value4);
	ProcessF GetAvgProcess_PID(int PID,int sample);
	vector<ProcessF> GetAllProcesses();
};

#endif