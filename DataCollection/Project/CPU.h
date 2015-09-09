#ifndef CPU_H
#define CPU_H

#include "Data.h"

class CPU:public Data
{
public:
	int count;
	CPU();
	virtual ~CPU();
	vector<Process> GetAllCpuProc();
	double GetTotalCPU();
	Process GetMaxCPU();
	Process GetMinCPU();
	vector<Process> GetAvgCPU();
private:

protected:
};

#endif