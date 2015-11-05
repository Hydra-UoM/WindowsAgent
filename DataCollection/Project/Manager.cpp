#include "RegisterDeviceService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "Manager.h"
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace  HydraCN;
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

void Manager::deviceClient(){

	boost::shared_ptr<TTransport> socket(new TSocket("172.20.10.7", 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RegisterDeviceServiceClient client(protocol);
	HydraCN::Device device;
	

	//Device Register Function 
	std::string type = "Windows";
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
	device.deviceId = mac;
	device.IPAddress = address;
	device.type = type;
	try {
		transport->open();
		client.registerDevice(device);
		cout << "Registered" << endl;

		transport->close();
	}
	catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}

}

	