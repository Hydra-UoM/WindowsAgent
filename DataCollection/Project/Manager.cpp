#include "Manager.h"


Manager::Manager()
{
}


Manager::~Manager()
{
}

void Manager::Start()
{
	

}

vector<ProcessF> Manager::FilterAllProcesses(double value1, double value2, double value3,double value4)
{
	vector<ProcessF> temp;
	ProcessF p;
	Data d;
	d.GetData();

	for (auto i : d.myData)
	{
		if (get<3>(i) >= value1 && get<2>(i) >= value2 && get<6>(i) >= value3 && get<7>(i) >= value4)
		{
			p.name = get<0>(i);
			p.cpu = get<3>(i);
			p.mem = get<2>(i);
			p.down = get<6>(i);
			p.up = get<7>(i);
			temp.push_back(p);
		}
	}

	return temp;
}
//filter all average processes by maximum sample , CPU usage,Memory usage,Network Download ,Network Upload
vector<ProcessF> Manager::FilterAllAvgProcesses(int samples,double value1, double value2, double value3, double value4)
{
	vector<ProcessF> temp;
	ProcessF p;
	Data d;
	d.MAX_SAMPLES = samples;

	for (int i = 0; i < samples + 2; i++)
	{
		d.GetData();
	}

	for (auto i : d.myData)
	{	//if a process haves the values equal or bigger than the user input the process is contained in temp
		if (get<5>(i) >= value1 && get<4>(i) >= value2 && get<8>(i) >= value3 && get<9>(i) >= value4)
		{
			p.name = get<0>(i);
			p.cpu = get<5>(i);
			p.mem = get<4>(i);
			p.down = get<8>(i);
			p.up = get<9>(i);

			temp.push_back(p);
		}
	}

	return temp;
}

ProcessF Manager::GetAvgProcess_PID(int PID,int sample)
{
	ProcessF p;
	Data d;
	
	d.MAX_SAMPLES = sample;

	for (int i = 0; i < sample + 2; i++)
	{
		d.GetData();
	}

	for (auto i : d.myData)
	{
		if (get<1>(i) == PID)
		{
			p.name = get<0>(i);
			p.cpu = get<3>(i);
			p.mem = get<2>(i);
			p.down = get<6>(i);
			p.up = get<7>(i);
			p.avgcpu = get<5>(i);
			p.avgmem = get<4>(i);
			p.avgdown = get<8>(i);
			p.avgup = get<9>(i);
		}
		
	}

	return p;
}

vector<ProcessF> Manager::GetAllProcesses()
{
	ProcessF p;
	vector<ProcessF> temp;
	Data d;
	d.GetData();

	for (auto i : d.myData)
	{
			p.name = get<0>(i);
			p.cpu = get<3>(i);
			p.mem = get<2>(i);
			p.down = get<6>(i);
			p.up = get<7>(i);
			p.id = get<1>(i);
			p.avgcpu = get<5>(i);
			p.avgmem = get<4>(i);
			p.avgdown = get<8>(i);
			p.avgup = get<9>(i);

			temp.push_back(p);
	}

	return temp;
}