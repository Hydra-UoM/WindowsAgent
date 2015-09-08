//#define _WIN32_WINNT 0x0501
//#define SORT_CPU 1
#include "stdafx.h"
/*
Source: https://askldjd.wordpress.com/2011/01/05/a-pdh-helper-class-cpdhquery/
*/

#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <map>
#include <sstream>
#include <vector>
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

class CPdhQuery
{
public:

	// Inner exception class to report error.
	class CException
	{
	public:
		CException(std::tstring const & errorMsg) : m_errorMsg(errorMsg)    {}
		std::tstring What() const { return m_errorMsg; }
	private:
		std::tstring m_errorMsg;
	};

	//! Constructor
	explicit CPdhQuery(std::tstring const &counterPath)
		: m_pdhQuery(NULL)
		, m_pdhStatus(ERROR_SUCCESS)
		, m_pdhCounter(NULL)
		, m_counterPath(counterPath)
	{
		if (m_pdhStatus = PdhOpenQuery(NULL, 0, &m_pdhQuery))
		{
			throw CException(GetErrorString(m_pdhStatus));
		}

		// Specify a counter object with a wildcard for the instance.
		if (m_pdhStatus = PdhAddCounter(
			m_pdhQuery,
			m_counterPath.c_str(),
			0,
			&m_pdhCounter)
			)
		{
			GetErrorString(m_pdhStatus);
			throw CException(GetErrorString(m_pdhStatus));
		}
	}

	//! Destructor. The counter and query handle will be closed.
	~CPdhQuery()
	{
		m_pdhCounter = NULL;
		if (m_pdhQuery)
			PdhCloseQuery(m_pdhQuery);
	}

	//! Collect all the data since the last sampling period.
	std::map<std::tstring, double> CollectQueryData()
	{
		std::map<std::tstring, double> collectedData;

		while (true)
		{
			// Collect the sampling data. This might cause
			// PdhGetFormattedCounterArray to fail because some query type
			// requires two collections (or more?). If such scenario is
			// detected, the while loop will retry.
			if (m_pdhStatus = PdhCollectQueryData(m_pdhQuery))
			{
				throw CException(GetErrorString(m_pdhStatus));
			}

			// Size of the pItems buffer
			DWORD bufferSize = 0;

			// Number of items in the pItems buffer
			DWORD itemCount = 0;

			PDH_FMT_COUNTERVALUE_ITEM *pdhItems = NULL;

			// Call PdhGetFormattedCounterArray once to retrieve the buffer
			// size and item count. As long as the buffer size is zero, this
			// function should return PDH_MORE_DATA with the appropriate
			// buffer size.
			m_pdhStatus = PdhGetFormattedCounterArray(
				m_pdhCounter,
				PDH_FMT_DOUBLE,
				&bufferSize,
				&itemCount,
				pdhItems);

			// If the returned value is nto PDH_MORE_DATA, the function
			// has failed.
			if (PDH_MORE_DATA != m_pdhStatus)
			{
				throw CException(GetErrorString(m_pdhStatus));
			}

			std::vector<unsigned char> buffer(bufferSize);
			pdhItems = (PDH_FMT_COUNTERVALUE_ITEM *)(&buffer[0]);

			m_pdhStatus = PdhGetFormattedCounterArray(
				m_pdhCounter,
				PDH_FMT_DOUBLE,
				&bufferSize,
				&itemCount,
				pdhItems);

			if (ERROR_SUCCESS != m_pdhStatus)
			{
				continue;
			}

			tstring name;
			// Everything is good, mine the data.
			for (DWORD i = 0; i < itemCount; i++)			{
				name = tstring(pdhItems[i].szName);

				// Add no after duplicate processes eg chrome, chrome#2
				map<std::tstring, double>::iterator it;
				it = collectedData.find(name);
				u_int count = 1;
				string appData = "";
				wstring tmp;
				while (it != collectedData.end()) {
					appData = "#" + to_string(count);
					count++;
					tmp = tstring(appData.begin(), appData.end());
					it = collectedData.find(name + tmp);
				}

				collectedData.insert(
					std::make_pair(
					std::tstring(pdhItems[i].szName + tmp),
					pdhItems[i].FmtValue.doubleValue)
					);
			}

			pdhItems = NULL;
			bufferSize = itemCount = 0;
			break;
		}
		return collectedData;
	}

private:
	//! Helper function that translate the PDH error code into
	//! an useful message.
	std::tstring GetErrorString(PDH_STATUS errorCode)
	{
		HANDLE hPdhLibrary = NULL;
		LPTSTR pMessage = NULL;
		DWORD_PTR pArgs[] = { (DWORD_PTR)m_searchInstance.c_str() };
		std::tstring errorString;

		hPdhLibrary = LoadLibrary(_T("pdh.dll"));
		if (NULL == hPdhLibrary)
		{
			std::tstringstream ss;
			ss
				<< _T("Format message failed with ")
				<< std::hex << GetLastError();
			return ss.str();
		}

		if (!FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			/*FORMAT_MESSAGE_IGNORE_INSERTS |*/
			FORMAT_MESSAGE_ARGUMENT_ARRAY,
			hPdhLibrary,
			errorCode,
			0,
			(LPTSTR)&pMessage,
			0,
			(va_list*)pArgs))
		{
			std::tstringstream ss;
			ss
				<< m_counterPath
				<< _T(" ")
				<< _T("Format message failed with ")
				<< std::hex
				<< GetLastError()
				<< std::endl;
			errorString = ss.str();
		}
		else
		{
			errorString += m_counterPath;
			errorString += _T(" ");
			errorString += pMessage;
			LocalFree(pMessage);
		}

		return errorString;
	}

private:
	PDH_HQUERY m_pdhQuery;
	PDH_STATUS m_pdhStatus;
	PDH_HCOUNTER m_pdhCounter;
	std::tstring m_searchInstance;
	std::tstring m_counterPath;
};

class getProcessCpuUsage {
public:
	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
	DWORD procid;

	~getProcessCpuUsage() {
		CloseHandle(self);
	}

	getProcessCpuUsage(DWORD procId) {
		self = OpenProcess(PROCESS_ALL_ACCESS, TRUE, procId);
		procid = procId;

		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;


		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

	double get(){
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));
		memcpy(&user, &fuser, sizeof(FILETIME));
		percent = double(sys.QuadPart - lastSysCPU.QuadPart) + double(user.QuadPart - lastUserCPU.QuadPart);
		percent /= (now.QuadPart - lastCPU.QuadPart);
		percent /= numProcessors;
		lastCPU = now;
		lastUserCPU = user;
		lastSysCPU = sys;
		percent *= 100;

		if (isnan(percent) || isinf(percent)) return 0;

		return percent;
	}
};

class MemUsageClass
{
public:
/*
checks for filtering

//set the min/max value for filtering here
//also set corresponding check to true i.e if  you want to filter memory usage set memory_limit=true ,same with cpu case,set both to true for combined filtering
*/
int min_memory_limit;
int max_memory_limit;
float min_cpu_limit;
float max_cpu_limit;
boolean memory_limit; //check to enable/disable filtering on memory usage
boolean cpu_limit;//check to enable/disable cpu usage filtering
vector<tuple<double, double>> memProcUsages;

MemUsageClass(){
	min_memory_limit	= 0; //value of minimum memory usage to be listed
	max_memory_limit	= INT_MAX; //max value of memory usage to be printed
	min_cpu_limit		= 0;
	max_cpu_limit		= 100;
	memory_limit		= true; //check to enable/disable filtering on memory usage
	cpu_limit			= true;//check to enable/disable cpu usage filtering
};

boolean checkLimit(int min_limit,int max_limit,int usage)
{
    if(usage>min_limit && usage <max_limit)
        return true;
    return false;
}

boolean check_cpu_Limit(float min_limit,float max_limit,float usage)
{
    if(usage>min_limit && usage <max_limit)
        return true;
    return false;
}

double getProcMem(double id){
	//for (std::vector<tuple<double, double>>::iterator it = memProcUsages.begin() ; it != memProcUsages.end(); ++it){
		//std::cout << ' ' << it->get<0> <<endl;
	//}
	ofstream myfile;
	myfile.open ("memUsages.txt");
	//search to find the process id 
	for (unsigned int i = 0; i < memProcUsages.size(); i++)
    {
		if(id==get<0>(memProcUsages[i])){
			return get<1>(memProcUsages[i]);
		}
		//write to an output file only for debugging
		//cout << get<1>(memProcUsages[i]);
		myfile<<get<0>(memProcUsages[i]);
		myfile<<"  ";
		myfile<<get<1>(memProcUsages[i]);
		myfile<<"\n";
    }
		
	myfile.close();
    std::cout << '\n';
	return -1.0;		//return -1 for unknown processes
}

vector<tuple<double,double>> GetProcessList()
{
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
       // return( FALSE );
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof( PROCESSENTRY32 );

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if( !Process32First( hProcessSnap, &pe32 ) )
    {
        printError( TEXT("Process32First") ); // show cause of failure
        CloseHandle( hProcessSnap );          // clean the snapshot object
       // return( FALSE );
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    //_tprintf( TEXT("\n\n=========================================================================================" ));
    //_tprintf( TEXT("\nPROCESS NAME \tProcess ID \tDate & Time\tMemory usage \tCPU usage\t"));
    //_tprintf( TEXT("\n-----------------------------------------------------------------------------------------" ));
	int count = 0;
    do
    {
        hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
        PROCESS_MEMORY_COUNTERS_EX  pmc;
        FILETIME p_entry_time, p_exit_time, p_kernal_time, p_user_time;
        FILETIME cpu_idle_time, cpu_kernal_time, cpu_user_time;
        GetSystemTimes(&cpu_idle_time,&cpu_kernal_time,&cpu_user_time);
        int flag=0;
        if(memory_limit || cpu_limit)
        {
            int memory_flag=0,cpu_flag=0;
            if(memory_limit)
            {
                if(GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)) )
                {
                    if(checkLimit(min_memory_limit,max_memory_limit,(int)pmc.PrivateUsage/1024))
                        memory_flag=1;
                }
            }
            if(cpu_limit)
            {
                if(GetProcessTimes(hProcess, &p_entry_time, &p_exit_time, &p_kernal_time, &p_user_time))
                {
                    float cpu_usage_limit= (float) (1.0*(p_user_time.dwHighDateTime - p_user_time.dwLowDateTime) + (p_kernal_time.dwHighDateTime - p_kernal_time.dwLowDateTime))
                                           /
                                           (100.0*(cpu_user_time.dwHighDateTime-cpu_user_time.dwLowDateTime)+(cpu_kernal_time.dwHighDateTime-cpu_kernal_time.dwLowDateTime));
                    if(check_cpu_Limit(min_cpu_limit,max_cpu_limit,cpu_usage_limit))
                        cpu_flag=1;
                }
            }
            if(memory_limit && cpu_limit)
                flag=memory_flag*cpu_flag;
            else if(memory_limit)
                flag=memory_flag;
            else
                flag=cpu_flag;
        }
        if(memory_limit||cpu_limit)
            if(flag==0)
                continue;
        //_tprintf( TEXT("\n%s\n\t\t"), pe32.szExeFile );

        //_tprintf( TEXT("%9d\t"), (int)pe32.th32ProcessID );

        //hProcess = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe32.th32ProcessID );

        //if(hProcess == NULL)
        //    //_tprintf(TEXT("\tN/A\t "));
        //else if (GetProcessTimes(hProcess,
        //                         &createTime, &exitTime, &kernelTime, &kernelTime ) != -1 )
        //{
        //    SYSTEMTIME stLocal;
        //    if ( FileTimeToSystemTime( &createTime, &stLocal ) != -1 )
        //       // _tprintf( TEXT("%02d/%02d/%d %02d:%02d "),
        //       //           stLocal.wMonth, stLocal.wDay, stLocal.wYear,
        //       //           stLocal.wHour, stLocal.wMinute);
        //}



        if( GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)) )
        {
            //printf( "%9d KB\t", (int)pmc.PrivateUsage/1024  );
			memProcUsages.push_back(make_tuple((int)pe32.th32ProcessID,(int)pmc.PrivateUsage/1024));
        }
        /*else printf("\tN/A\t");


        if(GetProcessTimes(hProcess, &p_entry_time, &p_exit_time, &p_kernal_time, &p_user_time))
        {
            printf("%.2f\t", (
                       (1.0*(p_user_time.dwHighDateTime - p_user_time.dwLowDateTime) + (p_kernal_time.dwHighDateTime - p_kernal_time.dwLowDateTime))
                       /
                       (100.0*(cpu_user_time.dwHighDateTime-cpu_user_time.dwLowDateTime)+(cpu_kernal_time.dwHighDateTime-cpu_kernal_time.dwLowDateTime))));
        }
        else printf("N/A\t");*/

        CloseHandle( hProcess );
		count++;
    }
    while( Process32Next( hProcessSnap, &pe32 ) );

	//printf("Process Count : %d\n",count-1);
    CloseHandle( hProcessSnap );

   // _tprintf( TEXT("\n===================================================================================" ));
	return memProcUsages;
	//return( TRUE );
}


void printError( TCHAR* msg )
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError( );
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, eNum,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                   sysMsg, 256, NULL );

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) )
        ++p;
    do
    {
        *p-- = 0;
    }
    while( ( p >= sysMsg ) &&
            ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}


};


void DumpMap(std::map<std::tstring, double> const &m)
{
	std::map<std::tstring, double>::const_iterator itr = m.begin();
	while (m.end() != itr)
	{
		tcout << itr->first << " " << itr->second << std::endl;
		++itr;
	}
}

void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
		);
}

double DumpMapTotal(map<std::tstring, double> const &m)
{
	double counter = 0;
	std::map<std::tstring, double>::const_iterator itr = m.begin();
	while (m.end() != itr)
	{
		counter += itr->second;
		++itr;
	}
	return counter;
}
CPdhQuery pdhQuery(std::tstring(_T("\\Process(*)\\ID Process")));
CPdhQuery pdhQueryCpuUsage(tstring(_T("\\Processor Information(_Total)\\% Processor Time")));
CPdhQuery pdhQueryDownload(std::tstring(_T("\\Network Interface(*)\\Bytes Received/sec")));
CPdhQuery pdhQueryUpload(tstring(_T("\\Network Interface(*)\\Bytes Received/sec")));

map<tstring, tuple<getProcessCpuUsage*, CPdhQuery*, CPdhQuery*, CPdhQuery*>> queries;

void getCpuStats()
{
	double SumCpuUsage = 0;
	double SumMemUsage = 0;
	gotoxy(32, 10);
	cout << "Loading...";

	ofstream myfile;
	myfile.open ("example.txt");
	//try
	//{
		//while (true) {
	map<tstring, double> data = pdhQuery.CollectQueryData();


	for (map<tstring, double>::iterator itr = data.begin(); itr != data.end(); ++itr)
	{
		if (itr->first == L"_Total" || itr->first == L"Idle") {
			itr++;
			continue;
		}

		if (queries.find(itr->first) == queries.end()) {
			
			myfile<<itr->second;
			myfile<<"  ";
			myfile<<string(itr->first.begin(), itr->first.end());
			myfile<<"\n";
			
			getProcessCpuUsage *q1 = new getProcessCpuUsage((DWORD)itr->second);
			q1->get();
			
			CPdhQuery *q2 = new CPdhQuery(L"\\Process(" + itr->first + L")\\IO Read Bytes/sec");
			CPdhQuery *q3 = new CPdhQuery(L"\\Process(" + itr->first + L")\\IO Write Bytes/sec");
			CPdhQuery *q4 = new CPdhQuery(L"\\Process(" + itr->first + L")\\% Processor Time");

			queries[itr->first] = make_tuple(q1, q2, q3, q4);
		}
	}

	ostringstream oss;
	//vector<tuple<string, double, double, double>> finalData;
	//store process name, memory usage, cpu usage, network down, network up
	vector<tuple<string, double,double, double, double>> finalData;

	SumCpuUsage = 0;
	SumMemUsage = 0;
	//calcuclate memory for each process
	MemUsageClass memUsage;
	memUsage.GetProcessList();
	
	for (map<tstring, tuple<getProcessCpuUsage*, CPdhQuery*, CPdhQuery*, CPdhQuery*>>::iterator it = queries.begin();
		it != queries.end(); ++it) {
		try {
			string processName = string(it->first.begin(), it->first.end());
			processName.resize(20);

			double procId   = get<0>(it->second)->procid;     //get process id
			double procMem  = memUsage.getProcMem(procId);    //obtain memory usage for the above process id
			
			double cpuUsage = get<0>(it->second)->get();
			double tmpCpuUsage = DumpMapTotal(get<3>(it->second)->CollectQueryData());
			if (cpuUsage == 0 && tmpCpuUsage > 0 && tmpCpuUsage < 100)
            cpuUsage = tmpCpuUsage;

			SumCpuUsage += cpuUsage;
			if(procMem != -1){
				SumMemUsage += procMem;
			}
			double read = DumpMapTotal(get<1>(it->second)->CollectQueryData());
			double write = DumpMapTotal(get<2>(it->second)->CollectQueryData());

			finalData.push_back(make_tuple(processName, procMem, cpuUsage, read, write));
		}
		catch (CPdhQuery::CException const &e)
		{
			//If process terminates
			e;
			delete get<0>(it->second);
			delete get<1>(it->second);
			delete get<2>(it->second);
			queries.erase(it++);
		}

	}

	sort(finalData.begin(), finalData.end(),
		[](tuple<string, double, double, double, double> a, tuple<string, double,  double, double, double> b) {
		#ifdef SORT_CPU
			return (get<2>(a) > get<2>(b));
		#else
			return (get<1>(a) > get<1>(b));
		#endif
	}
	);

	double tmpCpuUsage = DumpMapTotal(pdhQueryCpuUsage.CollectQueryData());
	if (tmpCpuUsage == 100 && SumCpuUsage < 100) {
		tmpCpuUsage = SumCpuUsage;
	}

	oss << fixed << "Total CPU Usage: " << setprecision(1) << SumCpuUsage << "%"<<endl
		<< "Total Mem Usage: " << setprecision(0) << SumMemUsage << "KB"
		<< endl
		<< "Total Network Usage (DOWN): " << fixed << setprecision(2) << DumpMapTotal(pdhQueryDownload.CollectQueryData()) / (1024 * 8) << " Kbps" << endl
		<< "Total Network Usage (UP): " << fixed << setprecision(2) << DumpMapTotal(pdhQueryUpload.CollectQueryData()) / (1024 * 8) << " Kbps" << endl
		<< endl

		<< setw(20) << left << "Process Name"
		<< right
		<< setw(20) << left << "Process Memory"
		<< setw(13) << "CPU Usage"
		<< setw(13) << "Sent"
		<< setw(13) << "Received" << endl;

	int count = 0;

	for (auto d : finalData) {
		count++;
		oss << left << setw(15) << get<0>(d)
			<< right << setprecision(1) << fixed
			<< setw(10) << get<1>(d) << "Kb"
			<< setw(10) << get<2>(d) << "%"
			<< setw(10) << get<3>(d) / (1024 * 8) << "Kb/s"
			<< setw(10) << get<4>(d) / (1024 * 8) << "Kb/s" << endl;
	}
	system("cls");
	
	oss<<"Process Count : "<<count<<endl;
	
	cout << oss.str();

	gotoxy(0, 0);
	oss.str("");
			//Sleep(1500);
		//}
	//}
	//catch (CPdhQuery::CException const &e)
	//{
	//	tcout << e.What() << std::endl;
	//}
}

void main(){
	while(true){
		getCpuStats();
		Sleep(1000);
	}
	
}
