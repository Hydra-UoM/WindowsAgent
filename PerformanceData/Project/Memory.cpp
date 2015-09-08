#include "Memory.h"


Memory::Memory()
{
}


Memory::~Memory()
{
}


vector<Process> Memory::GetAllMemProc()
{
	count = 0;
	vector <Process> temp;
	Process p;
	GetData();
	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<2>(i);
		if (p.value != -1)
		{
			temp.push_back(p);
			count++;
		}
	}

	return temp;
}

double Memory::GetTotalMemory()
{
	GetData();
	return TotalMemUsage;
}

Process Memory::GetMaxMemory()
{
	vector<Process> p = GetAllMemProc();

	return GetMax(p, 0, count - 1);
}

Process Memory::GetMinMemory()
{
	vector<Process> p = GetAllMemProc();

	return GetMin(p, 0, count - 1);
}

vector<Process> Memory::GetAvgMem()
{
	vector<Process> temp;
	Process p;
	GetData();

	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<4>(i);
		temp.push_back(p);
	}

	return temp;
}