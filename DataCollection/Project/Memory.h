#ifndef Memory_H
#define Memory_H

#include "Data.h"

class Memory:public Data
{
public:
	int count;

	Memory();
	virtual ~Memory();
	vector<Process> GetAllMemProc();
	double GetTotalMemory();
	Process GetMaxMemory();
	Process GetMinMemory();
	vector<Process> GetAvgMem();

protected:
private:
};

#endif
