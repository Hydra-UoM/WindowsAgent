#include "RegisterDeviceService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "Manager.h"
#include "DBHandler.h"
#include "wininet.h"
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace  HydraCN;

HydraCN::Device device;
bool tCompleted = false;
bool tRetired;
DBHandler db;
Manager::Manager()
{
}


Manager::~Manager()
{
}

string Manager::ConfigFile(){

	ifstream myfile("IP.txt");
	string ip;
	string group;
	string fileRead;
	int num1, num2;
	if (myfile.is_open())
	{
		getline(myfile, ip);
		ip += ",";
		while (myfile)
		{
			group += ip;
			getline(myfile, ip);

		}

		myfile.close();
		return group;

	}

	else cout << "Unable to open file";
}

string Manager::getMAC(){
	std::string address;
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);
	char *mac_addr = (char*)malloc(17);
	//char *address = (char*)malloc(17);
	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");

	}

	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen     variable
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {

		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		if (AdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
		do {
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

			address = pAdapterInfo->IpAddressList.IpAddress.String;
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	free(AdapterInfo);
	std::string mac(mac_addr);

	return mac;
}

string Manager::getIP(){
	std::string address;
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);
	char *mac_addr = (char*)malloc(17);
	//char *address = (char*)malloc(17);
	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");

	}

	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen     variable
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {

		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		if (AdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
		do {
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

			address = pAdapterInfo->IpAddressList.IpAddress.String;
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	free(AdapterInfo);
	return address;
}
vector<ProcessF> Manager::FilterAllProcesses(double value1, double value2, double value3, double value4)
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

void Manager::retire(){
	tRetired = false;
	while (!tCompleted){}
	tCompleted = false;
}
//filter all average processes by maximum sample , CPU usage,Memory usage,Network Download ,Network Upload
void Manager::FilterAllAvgProcesses(int samples, double value1, double value2, double value3, double value4, vector<string> processList)
{
	string line;
	Manager manage;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;
	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}

	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);
	Data d;
	HydraCN::ThriftAgentProcessInfo proc;
	std::vector<HydraCN::ThriftAgentProcessInfo> process;
	d.MAX_SAMPLES = samples;

	while (tRetired == true){
		manage.Register();


		bool val = false;
		std::vector<HydraCN::ThriftAgentProcessInfo> process;
		for (int i = 0; i < samples + 2; i++)
		{
			d.GetData();
			Sleep(30000);
		}
		proc.mac = manage.getMAC();
		proc.type = "Windows";
		proc.timestamp = manage.getTime();


		if (!processList.empty()){

			for (auto i : d.myData)
			{
				for (std::vector<string>::iterator it = processList.begin(); it != processList.end(); ++it) {

					if (get<0>(i).c_str() == *it){

						//if a process haves the values equal or bigger than the user input the process is contained in temp
						if (get<5>(i) >= value1 && get<4>(i) >= value2 && get<8>(i) >= value3 && get<9>(i) >= value4)
						{
							proc.name = get<0>(i);
							proc.cpuUsage = get<5>(i);
							proc.ramUsage = (get<4>(i));
							proc.sentData = (get<8>(i));
							proc.receiveData = (get<9>(i));
							proc.pid = std::to_string(get<1>(i));
							
							process.push_back(proc);
						}
					}
				}
			}
		}

		if (processList.empty()){
			for (auto i : d.myData)
			{
				if (get<5>(i) >= value1 && get<4>(i) >= value2 && get<8>(i) >= value3 && get<9>(i) >= value4)
				{


					proc.name = get<0>(i);
					proc.cpuUsage = get<5>(i);
					proc.ramUsage = (get<4>(i));
					proc.sentData = (get<8>(i));
					proc.receiveData = (get<9>(i));
					proc.pid = std::to_string(get<1>(i));
					process.push_back(proc);
				}
			}
		}
		manage.sendStoredData();

		try {
			transport->open();
			val = client.pushProcessesInfo(process);
			cout << "Data Pushed" << endl;
			db.deleteData();
			transport->close();
		}
		catch (TException& tx) {
			db.insertData(process);
			cout << "ERROR: " << tx.what() << endl;
		}

	}
	tCompleted = true;
}



ProcessF Manager::GetAvgProcess_PID(int PID, int sample)
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
		if (get<2>(i) >= 0){
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
	}

	return temp;
}

void Manager::fullData(int time){
	Manager manage;
	vector<ProcessF> procF;
	string line;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;
	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}
	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);
	HydraCN::ThriftAgentProcessInfo proc;
	std::vector<HydraCN::ThriftAgentProcessInfo> process;

	while (tRetired == true){
		manage.sendStoredData();
		procF = manage.GetAllProcesses();
		bool val = false;
		std::vector<HydraCN::ThriftAgentProcessInfo> process;
		proc.mac = manage.getMAC();
		proc.type = "Windows";
		proc.timestamp = manage.getTime();
		for (auto i : procF)
		{
			proc.name = i.name;
			proc.cpuUsage = i.cpu;
			proc.ramUsage = i.mem;
			proc.sentData = i.up;
			proc.receiveData = i.down;
			proc.pid = std::to_string(i.id);
			process.push_back(proc);
		}


		try {
			transport->open();
			cout << "Data Pushed" << endl;
			val = client.pushProcessesInfo(process);

			transport->close();
		}
		catch (TException& tx) {
			db.insertData(process);
			manage.Register();
			cout << "ERROR: " << tx.what() << endl;
		}
		Sleep(time * 60000);
	}

	tCompleted = true;
}

void Manager::currentData(int time){
	Manager manage;
	vector<ProcessF> procF;
	string line;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;
	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}


	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);
	HydraCN::ThriftAgentProcessInfo proc;
	std::vector<HydraCN::ThriftAgentProcessInfo> process;

	while (tRetired == true){

		procF = manage.FilterAllProcesses(30, 1024 * 300, 0, 0);
		bool val = false;
		std::vector<HydraCN::ThriftAgentProcessInfo> process;
		proc.mac = manage.getMAC();
		proc.type = "Windows";
		proc.timestamp = manage.getTime();
		for (auto i : procF)
		{
			proc.name = i.name;
			proc.cpuUsage = i.cpu;
			proc.ramUsage = i.mem;
			proc.sentData = i.up;
			proc.receiveData = i.down;
			proc.pid = std::to_string(i.id);
			process.push_back(proc);
		}
		manage.sendStoredData();
		try {
			transport->open();
			cout << "Data Pushed" << endl;
			val = client.pushProcessesInfo(process);
			db.deleteData();
			transport->close();
		}
		catch (TException& tx) {
			db.insertData(process);
			manage.Register();
			cout << "ERROR: " << tx.what() << endl;
		}
		Sleep(time * 60000);
	}
	tCompleted = true;
}

void Manager::importantData(int time){
	Manager manage;
	vector<ProcessF> procF;
	string line;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;
	Data d;
	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}

	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);
	HydraCN::ThriftAgentProcessInfo proc;
	std::vector<HydraCN::ThriftAgentProcessInfo> process;

	while (tRetired == true){
		manage.sendStoredData();
		for (int i = 0; i < time + 2; i++)
		{
			d.GetData();
			Sleep(30000);
		}
		bool val = false;
		std::vector<HydraCN::ThriftAgentProcessInfo> process;
		proc.mac = manage.getMAC();
		proc.type = "Windows";
		proc.timestamp = manage.getTime();
		for (auto i : d.myData)
		{
			if (get<0>(i).c_str() == "Project" || get<5>(i) >= 30 && get<4>(i) >= 5000 && get<8>(i) >= 0 && get<9>(i) >= 0)
			{
				proc.name = get<0>(i);
				proc.cpuUsage = get<5>(i);
				proc.ramUsage = (get<4>(i));
				proc.sentData = (get<8>(i));
				proc.receiveData = (get<9>(i));
				proc.pid = std::to_string(get<1>(i));
				process.push_back(proc);
			}
		}
		try {
			transport->open();
			cout << "Data Pushed" << endl;
			val = client.pushProcessesInfo(process);

			transport->close();
		}
		catch (TException& tx) {
			db.insertData(process);
			manage.Register();
			cout << "ERROR: " << tx.what() << endl;
		}
	}
	tCompleted = true;
}

void Manager::sendStoredData(){
	Manager manage;
	vector<HydraCN::ThriftAgentProcessInfo> procF;
	string line;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;
	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}

	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);
	HydraCN::ThriftAgentProcessInfo proc;
	std::vector<HydraCN::ThriftAgentProcessInfo> process;
	DBHandler Dbh;
	procF = Dbh.sendData();


	for (auto i : procF)
	{
		proc.name = i.name;
		proc.cpuUsage = i.cpuUsage;
		proc.ramUsage = i.ramUsage;
		proc.sentData = i.sentData;
		proc.receiveData = i.receiveData;
		proc.pid = i.pid;
		process.push_back(proc);
	}

	boolean val = false;
	try {
		transport->open();
		cout << "Data Pushed" << endl;
		val = client.pushProcessesInfo(process);
		transport->close();
		//Dbh.deleteData();
	}
	catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}

}
void Manager::Register(){
	Manager manage;
	//MyLogManager myMan;
	string line;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;

	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}
	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);

	//Device Register Function 
	std::string type = "Windows";

	device.deviceId = manage.getMAC();
	device.IPAddress = manage.getIP();
	device.type = type;
	device.group = fileRead[1];
	device.name = manage.getComputerName();


	bool val = false;
	try {

		transport->open();
		val = client.registerDevice(device);
		cout << "Registered" << endl;
		transport->close();
	}

	catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}
void Manager::deviceClient(){
	Manager manage;
	//MyLogManager myMan;
	string line;
	vector<string> fileRead;
	line = manage.ConfigFile();
	istringstream ss(line);
	string token;

	while (getline(ss, token, ',')) {
		fileRead.push_back(token);
	}
	boost::shared_ptr<TTransport> socket(new TSocket(fileRead[0], 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);

	//Device Register Function 
	std::string type = "Windows";

	device.deviceId = manage.getMAC();
	device.IPAddress = manage.getIP();
	device.type = type;
	device.group = fileRead[1];
	device.name = manage.getComputerName();


	bool val = false;
	do{
		try {

			transport->open();
			val = client.registerDevice(device);
			cout << "Registered" << endl;

			transport->close();
		}

		catch (TException& tx) {
			cout << "ERROR: " << tx.what() << endl;
		}
	} while (!val);
}

void Manager::printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n\t%s failed with error %d (%s)"), msg, eNum, sysMsg);
}

string Manager::getComputerName()
{
	string str_computerName;
	LPWSTR  computerName;
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	// Get and display the name of the computer. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetComputerName(infoBuf, &bufCharCount))
	{
		printError(TEXT("GetComputerName"));
	}
	computerName = infoBuf;
	if (computerName){ str_computerName = CW2A(computerName); }
	return str_computerName;
}

string Manager::getTime() {
	time_t seconds;

	seconds = time(NULL);
	std::ostringstream ss;
	ss << seconds;
	string str = ss.str();

	return str;
}