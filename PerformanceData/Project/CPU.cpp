#include "CPU.h"


CPU::CPU()
{
}


CPU::~CPU()
{
}

vector<Process> CPU::GetAllCpuProc()
{
	count = 0;
	vector <Process> temp;
	Process p;

	GetData();
	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<3>(i);
		temp.push_back(p);
		count++;
	}
	return temp;
}

double CPU::GetTotalCPU()
{
	GetData();
	return TotalCpuUsage;
}

Process CPU::GetMaxCPU()
{
	vector<Process> p = GetAllCpuProc();

	return GetMax(p, 0, count - 1);
}
Process CPU::GetMinCPU()
{
	vector<Process> p = GetAllCpuProc();

	return GetMin(p, 0, count - 1);
}

vector<Process> CPU::GetAvgCPU()
{
	vector<Process> temp;
	Process p;
	GetData();

	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<5>(i);
		temp.push_back(p);
	}

	return temp;
}