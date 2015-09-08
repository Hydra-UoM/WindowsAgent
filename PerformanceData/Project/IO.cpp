#include "IO.h"


IO::IO()
{
}


IO::~IO()
{
}

vector<Process> IO::GetAllNetDownloadProc()
{
	countD = 0;
	vector <Process> temp;
	Process p;
	GetData();
	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<6>(i);
		temp.push_back(p);
		countD++;
	}

	return temp;
}

vector<Process> IO::GetAllNetUploadProc()
{
	countU = 0;
	vector <Process> temp;
	Process p;
	GetData();
	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<7>(i);
		temp.push_back(p);
		countU++;
	}

	return temp;
}

double IO::GetTotalNetDownload()
{
	return TotalNetDown();
}

double IO::GetTotalNetUpload()
{
	return TotalNetUp();
}


Process IO::GetMaxNetDownload()
{
	vector<Process> p = GetAllNetDownloadProc();

	return GetMax(p, 0, countD - 1);
}

Process IO::GetMinNetDownload()
{
	vector<Process> p = GetAllNetDownloadProc();

	return GetMin(p, 0, countD - 1);
}

Process IO::GetMaxNetUpload()
{
	vector<Process> p = GetAllNetUploadProc();

	return GetMax(p, 0, countU - 1);
}

Process IO::GetMinNetUpload()
{
	vector<Process> p = GetAllNetUploadProc();

	return GetMin(p, 0, countU - 1);
}

vector<Process> IO::GetAllAvgNetDownload()
{
	vector<Process> temp;
	Process p;
	GetData();

	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<8>(i);
		temp.push_back(p);
	}

	return temp;
}

vector<Process> IO::GetAllAvgNetUpload()
{
	vector<Process> temp;
	Process p;
	GetData();

	for (auto i : myData)
	{
		p.name = get<0>(i);
		p.value = get<9>(i);
		temp.push_back(p);
	}

	return temp;
}