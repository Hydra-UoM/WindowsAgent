#include "Objects.h"
#include "ProcessStats.h"
#include "Manager.h"
#include "MyLogManager.h"
#include "DBHandler.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>

#pragma comment(lib, "iphlpapi.lib")

#define DIV 1048576

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::HydraWindows;
Manager manage;

DBHandler dbhan;

class ProcessStatsHandler : virtual public ProcessStatsIf {
public:
	ProcessStatsHandler() {
		// Your initialization goes here
	}

	Process p;
	vector< Process > proc;
	vector<ProcessF> procF;
	ProcessF pF;
	list<string>::iterator i;
	double getTotalCPU() {
		// Your implementation goes here
		printf("getTotalCPU\n");
		return manage.GetTotalCPU();
	}

	double getTotalMemory() {
		// Your implementation goes here
		printf("getTotalMemory\n");
		return manage.GetTotalMemory();
	}

	int64_t getFreeMemory() {
		// Your implementation goes here
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		printf("getFreeMemory\n");
		return (statex.ullAvailPhys / DIV);

	}

	double getTotalNetDownload() {
		// Your implementation goes here
		printf("getTotalNetDownload\n");
		return manage.GetTotalNetDownload();
	}

	double getTotalNetUpload() {
		// Your implementation goes here
		printf("getTotalNetUpload\n");
		return manage.GetTotalNetUpload();
	}

	void getMaxCPU(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMaxCPU\n");
		p = manage.GetMaxCPU();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMinCPU(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMinCPU\n");
		p = manage.GetMinCPU();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMaxMemory(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMaxMemory\n");
		p = manage.GetMaxMemory();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMinMemory(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMinMemory\n");
		p = manage.GetMinMemory();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMaxNetDownload(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMaxNetDownload\n");
		p = manage.GetMaxNetDownload();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMinNetDownload(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMinNetDownload\n");
		p = manage.GetMinNetDownload();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMaxNetUpload(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMaxNetUpload\n");
		p = manage.GetMaxNetUpload();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getMinNetUpload(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getMinNetUpload\n");
		p = manage.GetMinNetUpload();
		_return.push_back(p.name);
		_return.push_back(std::to_string(p.value));
	}

	void getAllCpuProc(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getAllCpuProc\n");
		proc = manage.GetAllCpuProc();
		for (auto i : proc)
		{
			_return.push_back(i.name);
			_return.push_back(std::to_string(i.value));
		}
	}

	void getAllMemProc(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getAllMemProc\n");
		proc = manage.GetAllMemProc();
		for (auto i : proc)
		{
			_return.push_back(i.name);
			_return.push_back(std::to_string(i.value));
		}
	}

	void getAllNetDownloadProc(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getAllNetDownloadProc\n");
		proc = manage.GetAllNetDownloadProc();
		for (auto i : proc)
		{
			_return.push_back(i.name);
			_return.push_back(std::to_string(i.value));
		}
	}

	void getAllNetUploadProc(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getAllNetUploadProc\n");
		proc = manage.GetAllNetUploadProc();
		for (auto i : proc)
		{
			_return.push_back(i.name);
			_return.push_back(std::to_string(i.value));
		}
	}

	void filterAllProcesses(std::vector<std::string> & _return, const double cpu, const double mem, const double down, const double up, const std::string& processname) {
		// Your implementation goes here
		printf("filterAllProcesses\n");
		int count = 0;
		procF = manage.FilterAllProcesses(cpu, mem, down, up);
		for (auto i : procF)
		{

			if (processname == i.name.c_str()){
				_return.push_back(processname);
				_return.push_back(std::to_string(i.cpu));
				_return.push_back(std::to_string(i.mem / 1024));
				_return.push_back(std::to_string(i.down / 1024));
				_return.push_back(std::to_string(i.up / 1024));
				_return.push_back(";");
				count++;
			}
		}
		if (count == 0){
			for (auto i : procF)
			{
				_return.push_back(i.name);
				_return.push_back(std::to_string(i.cpu));
				_return.push_back(std::to_string(i.mem / 1024));
				_return.push_back(std::to_string(i.down / 1024));
				_return.push_back(std::to_string(i.up / 1024));
				_return.push_back(";");

			}
		}
	}


	void filterAllAvgProcesses(const int64_t sample, const double cpu, const double mem, const double down, const double up, const std::vector<std::string> & processList) {
		// Your implementation goes here
		printf("filterAllAvgProcesses\n");
		auto memfunc = &Manager::FilterAllAvgProcesses;
		std::thread thread1(memfunc, sample, cpu, mem, down, up, processList);
		thread1.detach();
	}

	void getAvgProcess_PID(std::vector<std::string> & _return, const int64_t PID, const int64_t sample) {
		// Your implementation goes here
		printf("getAvgProcess_PID\n");
		pF = manage.GetAvgProcess_PID(PID, sample);
		_return.push_back(pF.name);
		_return.push_back(std::to_string(pF.cpu));
		_return.push_back(std::to_string(pF.mem));
		_return.push_back(std::to_string(pF.down));
		_return.push_back(std::to_string(pF.up));
		_return.push_back(std::to_string(pF.avgcpu));
		_return.push_back(std::to_string(pF.avgmem));
		_return.push_back(std::to_string(pF.avgdown));
		_return.push_back(std::to_string(pF.avgup));
	}

	void getAllProcesses(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getAllProcesses\n");
		procF = manage.GetAllProcesses();
		for (auto i : procF)
		{
			_return.push_back(i.name);
			_return.push_back(std::to_string(i.cpu));
			_return.push_back(std::to_string(i.mem));
			_return.push_back(std::to_string(i.down));
			_return.push_back(std::to_string(i.up));
			_return.push_back(";");
		}
	}

	void fullData(const int32_t timeInMinute){
		auto memfunc = &Manager::fullData;
		tRetired = true;
		std::thread thread1(memfunc, timeInMinute);
		thread1.detach();
	}
	void currentData(const int32_t timeInMinute){
		auto memfunc = &Manager::currentData;
		tRetired = true;
		std::thread thread1(memfunc, timeInMinute);
		thread1.detach();

	}

	void importantData(const int32_t timeInMinute){
		auto memfunc = &Manager::importantData;
		tRetired = true;
		std::thread thread1(memfunc, timeInMinute);
		thread1.detach();
	}

	void getNetwork(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getNetwork\n");
		list<string> NetList;
		list<string>::iterator itr;
		NetwerkObject Net;

		NetList.clear();
		NetList = Net.getNetwork();
		for (itr = NetList.begin(); itr != NetList.end(); ++itr)
		{
			_return.push_back(*itr);
		}
	}

	void getTCP(std::vector<std::string> & _return) {
		// Your implementation goes here
		printf("getTCP\n");
		printf("getTCP\n");
		list<string> TCPlist;
		list<string>::iterator itr1;
		TCPObject Tcp;
		TCPlist = Tcp.getTCP();
		for (itr1 = TCPlist.begin(); itr1 != TCPlist.end(); ++itr1)
		{
			_return.push_back(*itr1);
		}
	}

	void getLogRelatedInformation(const int16_t timeInMinute, const int16_t summarizationLevel, const std::vector<std::string> & eventIndices, const std::string& logType, const std::string& process_name, const std::string& securityLevel) {
		// Your implementation goes here
		printf("getLogRelatedInformation\n");
		auto memfunc = &MyLogManager::getLogRelatedInformation;
		std::thread thread1(memfunc, timeInMinute, summarizationLevel, eventIndices, logType, process_name, securityLevel);
		thread1.detach();
	}

	void getImportantLogEventsWithoutSummarization(const int16_t timeInMinute) {
		// Your implementation goes here
		printf("getImportantLogEventsWithoutSummarization\n");
		auto memfunc = &MyLogManager::getImportantLogEventsWithoutSummarization;
		std::thread thread1(memfunc, timeInMinute);
		thread1.detach();
	}

	void getImportantLogEvents(const int16_t timeInMinute) {
		// Your implementation goes here
		printf("getImportantLogEvents\n");
		auto memfunc = &MyLogManager::getImportantLogEvents;
		std::thread thread1(memfunc, timeInMinute);
		thread1.detach();
	}

	void getFullLogInformation(const int16_t timeInMinute) {
		// Your implementation goes here
		printf("getFullLogInformation\n");
		auto memfunc = &MyLogManager::getFullLogInformation;
		std::thread thread1(memfunc, timeInMinute);
		thread1.detach();
	}

  void stop() {
    // Your implementation goes here
    printf("stop\n");
	manage.retire();
  }

  void stopLogInfo() {
    // Your implementation goes here
    printf("stopLogInfo\n");
	MyLogManager::stopExecution();
  }

};

int main(int argc, char **argv) {
	WSADATA wsaData = {};
	WORD wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	//DBHandler db;
	/**
	if (!MyLogManager::isInternetConnectionAvailable())
	{
		while (true){
			db.insertData();
			MyLogManager::storeImportantLogData();
			Sleep(50000);
		}
	}
	*/
	//else{
		manage.deviceClient();
		//manage.sendStoredData();
		//MyLogManager::sendStoredData();
		int port = 9090;

		using boost::shared_ptr;
		shared_ptr<ProcessStatsHandler> handler(new ProcessStatsHandler());
		shared_ptr<TProcessor> processor(new ProcessStatsProcessor(handler));
		shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
		shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
		shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

		TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
		server.serve();
		return 0;
	//}
}

