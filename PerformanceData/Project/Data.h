#ifndef Data_H
#define Data_H
//#define _WIN32_WINNT 0x0501
#define MAX 5
#include "stdafx.h"

#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string.h>
#include <map>
#include <sstream>
#include <vector>
#include<conio.h>
#include <tchar.h>
#include <iostream>
#include <tuple> 
#include <string>
#include <iomanip>
#include <algorithm>
// basic file operations
#include <iostream>
#include <fstream>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <limits.h>

#pragma comment(lib, "pdh.lib")

using namespace std;

namespace std
{
	typedef std::basic_string<TCHAR> tstring;
	typedef std::basic_ostream<TCHAR> tostream;
	typedef std::basic_istream<TCHAR> tistream;
	typedef std::basic_ostringstream<TCHAR> tostringstream;
	typedef std::basic_istringstream<TCHAR> tistringstream;
	typedef std::basic_stringstream<TCHAR> tstringstream;
} // end namespace

#ifdef UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif
#define isnan(x) _isnan(x)
#define isinf(x) (!_finite(x))
#define fpu_error(x) (isinf(x) || isnan(x))

//structure defined to contain the name and the usage of a process
struct Process
{
	string name;
	double value;
};
//structure defined to contain the name and all usages of a process
struct ProcessF
{
	string name;
	double cpu;
	double mem;
	double down;
	double up;
	double id;
	double avgcpu;
	double  avgmem;
	double avgdown;
	double avgup;
};



class Data
{
public:

	 double TotalMemUsage;
	
	 double TotalCpuUsage;

	 int MAX_SAMPLES=MAX;

	 

	 //vector<tuple<string, double, double, double>> finalData;
	 //store process name, process id, memory usage, cpu usage, avg cpu, avg mem,network down, network up
	 vector<tuple<string, int, double, double, double, double, double, double, double, double>> myData;

	Data();
	~Data();
	void DumpMap(std::map<std::tstring, double> const &m);
	double DumpMapTotal(map<std::tstring, double> const &m);
	int setProcInfo(int procID, double cpu, double mem,double down,double up,string name);
	Process GetMax(vector<Process> p, int i, int j);
	Process GetMin(vector<Process> p, int i, int j);
	double getProcCpuAvg(int procID);
	double getProcMemAvg(int procID);
	void GetData();
	double getProcNetDownAvg(int procID);
	double getProcNetUpAvg(int procID);
	ProcessF getAllProcAvg(int procID);
	double TotalNetDown();
	double TotalNetUp();
	
	
private:

protected:

};



#endif