#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<string>
#include<iostream>
#include<WinSock2.h>
#include "stdafx.h"

#include <list>
#include <cstring>
#include <atlstr.h>
#include <string.h>
 
#include <winsock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <time.h>
#include <iphlpapi.h>
#include <cstdio>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "irprops.lib")

#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
using namespace std;
class TCPObject{
private:
	//TCPObject*link;
public:
	string State;
	string LocalAddr;
	u_short LocalPort;
	string RemoteAddr;
	u_short RemotePort;
	float OwningPID;
	string OffloadState;

	TCPObject()
	{
		State = "";
		LocalAddr = "";
		LocalPort = 0;
		RemoteAddr = "";
		RemotePort = 0;
		OwningPID = 0;
		OffloadState = "";
		//link=NULL;
	}
	//TCPObject(string St,string LA,u_short LP,string RemoteA,u_short RemoteP,float OwnPID,string OffloadSt,TCPObject* Link)
	//{
	//	State=St;
	//	LocalAddr=LA;
	//	LocalPort=LP;
	//	RemoteAddr=RemoteA;
	//	RemotePort=RemoteP;
	//	OwningPID=OwnPID;
	//	OffloadState=OffloadSt;
	//	link=Link;
	//}
	//TCPObject(TCPObject* P)
	//{
	//	State=P->State;
	//	LocalAddr=P->LocalAddr;
	//	LocalPort=P->LocalPort;
	//	RemoteAddr=P->RemoteAddr;
	//	RemotePort=P->RemotePort;
	//	OwningPID=P->OwningPID;
	//	OffloadState=P->OffloadState;
	//	link=P->getAddress();
	//}
	void Display()
	{
		cout << endl;
		cout << "State: " << State << endl;
		cout << "Local Addr: " << LocalAddr << endl;
		cout << "Local Port " << LocalPort << endl;
		cout << "Remote Addr: " << RemoteAddr << endl;
		cout << "Remote Prot: " << RemotePort << endl;
		cout << "Owning PID: " << OwningPID << endl;
		cout << "Offload State: " << OffloadState << endl;
	}
	string toString()
	{
		string s;
		s = State + "," + LocalAddr + "," + std::to_string(LocalPort) + "," + RemoteAddr + "," + std::to_string(RemotePort) + "," + std::to_string(OwningPID )+ "," + OffloadState;
		return s;
	}
	//TCPObject* getAddress()
	//{
	//	return link;
	//}
	//void SetAddress(TCPObject* L)
	//{
	//	link=L;
	//}
	//TCPlinkList*
	list<string> getTCP(){
		// Declare and initialize variables
		//	TCPlinkList* Tcplist=new TCPlinkList();
		TCPObject List;
		list<string> listTcp;
		PMIB_TCPTABLE2 pTcpTable;
		ULONG ulSize = 0;
		DWORD dwRetVal = 0;

		char szLocalAddr[128];
		char szRemoteAddr[128];

		struct in_addr IpAddr;

		int i;

		pTcpTable = (MIB_TCPTABLE2 *)MALLOC(sizeof(MIB_TCPTABLE2));
		if (pTcpTable == NULL) {
			//printf("Error allocating memory\n");
			return listTcp;
		}

		ulSize = sizeof(MIB_TCPTABLE);
		// Make an initial call to GetTcpTable2 to
		// get the necessary size into the ulSize variable
		if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) ==
			ERROR_INSUFFICIENT_BUFFER) {
			FREE(pTcpTable);
			pTcpTable = (MIB_TCPTABLE2 *)MALLOC(ulSize);
			if (pTcpTable == NULL) {
				//printf("Error allocating memory\n");
				return listTcp;
			}
		}
		// Make a second call to GetTcpTable2 to get
		// the actual data we require
		if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) == NO_ERROR) {
			//printf("\tNumber of entries: %d\n", (int)pTcpTable->dwNumEntries);

			for (i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
				//create list of the TCP Object to store the values

				/*printf("\n\tTCP[%d] State: %ld - ", i,
				pTcpTable->table[i].dwState);*/
				switch (pTcpTable->table[i].dwState) {
				case MIB_TCP_STATE_CLOSED:
				{
											 List.State = "1 - CLOSED";
											 //printf("CLOSED\n");
											 break; }
				case MIB_TCP_STATE_LISTEN:{
											  List.State = "2 - LISTEN";
											  //printf("LISTEN\n");
											  break; }
				case MIB_TCP_STATE_SYN_SENT:{
												List.State = "3 - SYN-SENT";
												//printf("SYN-SENT\n");
												break; }
				case MIB_TCP_STATE_SYN_RCVD:{
												List.State = "4 - SYN-RECEIVED";
												//printf("SYN-RECEIVED\n");
												break; }
				case MIB_TCP_STATE_ESTAB:{
											 List.State = "5 - ESTABLISHED";
											 //printf("ESTABLISHED\n");
											 break; }
				case MIB_TCP_STATE_FIN_WAIT1:{
												 List.State = "6 - FIN-WAIT-1";
												 //printf("FIN-WAIT-1\n");
												 break; }
				case MIB_TCP_STATE_FIN_WAIT2:{
												 List.State = "7 - FIN-WAIT-2";
												 //printf("FIN-WAIT-2 \n");
												 break; }
				case MIB_TCP_STATE_CLOSE_WAIT:{
												  List.State = "8 - CLOSE-WAIT";
												  //printf("CLOSE-WAIT\n");
												  break; }
				case MIB_TCP_STATE_CLOSING:{
											   List.State = "9 - CLOSING";
											   //printf("CLOSING\n");
											   break; }
				case MIB_TCP_STATE_LAST_ACK:{
												List.State = "10 - LAST-ACK";
												//printf("LAST-ACK\n");
												break; }
				case MIB_TCP_STATE_TIME_WAIT:{
												 List.State = "11 - TIME-WAIT";
												 //printf("TIME-WAIT\n");
												 break; }
				case MIB_TCP_STATE_DELETE_TCB:{
												  List.State = "12 - DELETE-TCB";
												  //printf("DELETE-TCB\n");
												  break; }
				default:{
							List.State = "UNKNOWN dwState value";
							//printf("UNKNOWN dwState value\n");
							break; }
				}
				IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwLocalAddr;
				strcpy_s(szLocalAddr, sizeof(szLocalAddr), inet_ntoa(IpAddr));
				List.LocalAddr = szLocalAddr;
				List.LocalPort = ntohs((u_short)pTcpTable->table[i].dwLocalPort);
				//printf("\tTCP[%d] Local Addr: %s\n", i, szLocalAddr);
				//printf("\tTCP[%d] Local Port: %d \n", i,ntohs((u_short)pTcpTable->table[i].dwLocalPort));

				IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwRemoteAddr;
				strcpy_s(szRemoteAddr, sizeof(szRemoteAddr), inet_ntoa(IpAddr));

				List.RemoteAddr = szRemoteAddr;
				List.RemotePort = ntohs((u_short)pTcpTable->table[i].dwRemotePort);
				/*printf("\tTCP[%d] Remote Addr: %s\n", i, szRemoteAddr);
				printf("\tTCP[%d] Remote Port: %d\n", i,
				ntohs((u_short)pTcpTable->table[i].dwRemotePort));*/

				List.OwningPID = pTcpTable->table[i].dwOwningPid;
				//List[i].OffloadState
				/*printf("\tTCP[%d] Owning PID: %d\n", i, pTcpTable->table[i].dwOwningPid);
				printf("\tTCP[%d] Offload State: %ld - ", i,
				pTcpTable->table[i].dwOffloadState);*/
				switch (pTcpTable->table[i].dwOffloadState) {
				case TcpConnectionOffloadStateInHost:{
														 List.OffloadState = "0 - Owned by the network stack and not offloaded ";
														 //printf("Owned by the network stack and not offloaded \n");
														 break; }
				case TcpConnectionOffloadStateOffloading:{
															 List.OffloadState = "1 - In the process of being offloaded";
															 //printf("In the process of being offloaded\n");
															 break; }
				case TcpConnectionOffloadStateOffloaded:{
															List.OffloadState = "2 - Offloaded to the network interface control";
															//printf("Offloaded to the network interface control\n");
															break; }
				case TcpConnectionOffloadStateUploading:{
															List.OffloadState = "3 - In the process of being uploaded back to the network stack ";
															//printf("In the process of being uploaded back to the network stack \n");
															break; }
				default:{
							List.OffloadState = "UNKNOWN Offload state value";
							//printf("UNKNOWN Offload state value\n");
							break; }
				}
				listTcp.push_back(List.toString());
			}
		}
		else {
			//printf("\tGetTcpTable2 failed with %d\n", dwRetVal);
			FREE(pTcpTable);
			return listTcp;
		}

		if (pTcpTable != NULL) {
			FREE(pTcpTable);
			pTcpTable = NULL;
		}
		//getchar();
		return listTcp;
	}
};


class NetwerkObject{
public:
	string ComboIndex;
	string AdapterName;
	string AdapterDesc;
	string AdapterAddr;
	string Index;
	string Type;
	string IPAddress;
	string IPMask;
	string Gateway;
	string DHCPEnable;
	string LaeseObtained;
	string LeaseExpires;
	string win;
	string PrimWinAdap;
	string SecWinAdap;
	string DHCPServer;
	NetwerkObject()
	{
		ComboIndex = "";
		AdapterName = "";
		AdapterDesc = "";
		AdapterAddr = "";
		Index = "";
		Type = "";
		IPAddress = "";
		IPMask = "";
		Gateway = "";
		DHCPEnable = "";
		DHCPServer = "";
		LaeseObtained = "";
		LeaseExpires = "";
		win = "";
		PrimWinAdap = "";
		SecWinAdap = "";
	}
	void Dispay(){
		cout << "Combo Index: " << ComboIndex << "\n";
		cout << "Adapter Name: " << AdapterName << "\n";
		cout << "Adapter Descpriction: " << AdapterDesc << "\n";
		cout << "Adapter Addr: " << AdapterAddr << "\n";
		cout << "Index: " << Index << "\n";
		cout << "Type: " << Type << "\n";
		cout << "IP Address: " << IPAddress << "\n";
		cout << "IP Mask: " << IPMask << "\n";
		cout << "Gateway: " << Gateway << "\n";
		cout << "DHCPEnable: " << DHCPEnable << "\n";
		cout << "DHCP server: " << DHCPServer << "\n";
		cout << "Laese Obtained: " << LaeseObtained << "\n";
		cout << "Lease Expires: " << LeaseExpires << "\n";
		cout << "Have Wins: " << win << "\n";
		cout << PrimWinAdap << "\n";
		cout << SecWinAdap << "\n";
	}
	string toString()
	{
		string s;
		//cout << " check : " << ComboIndex << "\n\n";
		s = ComboIndex + ","+AdapterName + ","+AdapterDesc + ","+AdapterAddr + ","+Index +","+Type + ","+IPAddress + "," +
		IPMask + "," +Gateway + "," +DHCPEnable + "," +DHCPServer + "," +LaeseObtained + "," +LeaseExpires + "," +win + "," + 
		PrimWinAdap + "," +SecWinAdap;
		 return s;
	}

	list<string> getNetwork(){

		//cout << "\n\n\n\n**********************Connected Network Adpaters*************************" << endl;

		/* Declare and initialize variables */

		// It is possible for an adapter to have multiple
		// IPv4 addresses, gateways, and secondary WINS servers
		// assigned to the adapter. 
		//
		// Note that this sample code only prints out the 
		// first entry for the IP address/mask, and gateway, and
		// the primary and secondary WINS server for each adapter. 
		NetwerkObject object;
		list<string> NetList;
		PIP_ADAPTER_INFO pAdapterInfo;
		PIP_ADAPTER_INFO pAdapter = NULL;
		DWORD dwRetVal = 0;
		UINT i;

		/* variables used to print DHCP time info */
		struct tm newtime;
		char buffer[32];
		errno_t error;

		ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
		if (pAdapterInfo == NULL) {
			//printf("Error allocating memory needed to call GetAdaptersinfo\n");
			//return 1;
		}
		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			FREE(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
			if (pAdapterInfo == NULL) {
				//printf("Error allocating memory needed to call GetAdaptersinfo\n");
				//return 1;
			}
		}

		if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			pAdapter = pAdapterInfo;
			while (pAdapter) {
				object.ComboIndex = std::to_string(pAdapter->ComboIndex);

				object.AdapterName = pAdapter->AdapterName;
				object.AdapterDesc = pAdapter->Description;
				/*printf("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
				printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
				printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
				printf("\tAdapter Addr: \t");*/

				CString csMacAddress;
				for (i = 0; i < pAdapter->AddressLength; i++) {
					if (i == (pAdapter->AddressLength - 1)){
						csMacAddress.Format(_T("%.2X\n"), (int)pAdapter->Address[i]);
						//printf("%.2X\n", (int)pAdapter->Address[i]);
					}
					else{
						csMacAddress.Format(_T("%.2X-"), (int)pAdapter->Address[i]);
						//printf("%.2X-", (int)pAdapter->Address[i]);
					}
				}


				csMacAddress.Format(_T("%.02x:%.02x:%.02x:%.02x:%.02x:%.02x"),
					(int)pAdapter->Address[0],
					(int)pAdapter->Address[1],
					(int)pAdapter->Address[2],
					(int)pAdapter->Address[3],
					(int)pAdapter->Address[4],
					(int)pAdapter->Address[5]);
				//cout << "\n\naddress: " << (LPCTSTR)csMacAddress << "\n\n";
				size_t size = (csMacAddress.GetLength() + 1) * 4;
				char* buf = new char[size + 1];
				if (buf)
				{
					int lt = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)csMacAddress, csMacAddress.GetLength() + 1, buf, size, 0, 0);
				}
				object.AdapterAddr = buf;

				//			object.AdapterAddr
				//printf("\tIndex: \t%d\n", pAdapter->Index);
				object.Index = to_string(pAdapter->Index);
				//printf("\tType: \t");
				switch (pAdapter->Type) {
				case MIB_IF_TYPE_OTHER:{
										   object.Type = "Other";
										   //printf("Other\n");
										   break; }
				case MIB_IF_TYPE_ETHERNET:{
											  object.Type = "Ethernet";
											  //printf("Ethernet\n");
											  break; }
				case MIB_IF_TYPE_TOKENRING:{
											   object.Type = "Token Ring";
											   //printf("Token Ring\n");
											   break; }
				case MIB_IF_TYPE_FDDI:{
										  object.Type = "FDDI";
										  //printf("FDDI\n");
										  break; }
				case MIB_IF_TYPE_PPP:{
										 object.Type = "PPP";
										 //printf("PPP\n");
										 break; }
				case MIB_IF_TYPE_LOOPBACK:{
											  object.Type = "Lookback";
											  //printf("Lookback\n");
											  break; }
				case MIB_IF_TYPE_SLIP:{
										  object.Type = "Slip";
										  //printf("Slip\n");
										  break; }
				default:{
							object.Type = "Unknown type " + to_string(pAdapter->Type);
							//printf("Unknown type %ld\n", pAdapter->Type);
							break; }
				}

				/*printf("\tIP Address: \t%s\n",
				pAdapter->IpAddressList.IpAddress.String);*/
				object.IPAddress = pAdapter->IpAddressList.IpAddress.String;

				//printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
				object.IPMask = pAdapter->IpAddressList.IpMask.String;

				//printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
				object.Gateway = pAdapter->GatewayList.IpAddress.String;
				/*printf*/("\t***\n");

				if (pAdapter->DhcpEnabled) {
					/*printf("\tDHCP Enabled: Yes\n");*/
					object.DHCPEnable = "Yes";
					/*printf("\t  DHCP Server: \t%s\n",
					pAdapter->DhcpServer.IpAddress.String);*/
					object.DHCPServer = pAdapter->DhcpServer.IpAddress.String;

					//printf("\t  Lease Obtained: ");
					/* Display local time */
					error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseObtained);
					if (error)
					{//printf("Invalid Argument to _localtime32_s\n");
					}
					else {
						// Convert to an ASCII representation 
						error = asctime_s(buffer, 32, &newtime);
						if (error)
						{//printf("Invalid Argument to asctime_s\n");
						}
						else{
							/* asctime_s returns the string terminated by \n\0 */
							//printf("%s", buffer);
							object.LaeseObtained = buffer;
						}
					}

					//printf("\t  Lease Expires:  ");
					error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseExpires);
					if (error)
					{
						//printf("Invalid Argument to _localtime32_s\n");
					}
					else {
						// Convert to an ASCII representation 
						error = asctime_s(buffer, 32, &newtime);
						if (error)
						{//printf("Invalid Argument to asctime_s\n");
						}
						else
						{
							/* asctime_s returns the string terminated by \n\0 */
							//printf("%s", buffer);
							object.LeaseExpires = buffer;
						}
					}
				}
				else{
					//printf("\tDHCP Enabled: No\n");
					object.DHCPEnable = "No";
				}

				if (pAdapter->HaveWins) {
					//printf("\tHave Wins: Yes\n");
					//printf("\t  Primary Wins Server:    %s\n",
					//	pAdapter->PrimaryWinsServer.IpAddress.String);
					object.PrimWinAdap = pAdapter->PrimaryWinsServer.IpAddress.String;
					/*printf("\t  Secondary Wins Server:  %s\n",
					pAdapter->SecondaryWinsServer.IpAddress.String);*/
					object.SecWinAdap = pAdapter->SecondaryWinsServer.IpAddress.String;
				}
				else
					//printf("\tHave Wins: No\n");
					object.win = "No";
				pAdapter = pAdapter->Next;
				//printf("\n");
				NetList.push_back(object.toString());
			}
		}
		else {
			//printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

		}
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		return NetList;
		getchar();
	}


private:

};


//
//class TCPlinkList{
//public:
//	TCPObject* head;
//	TCPObject* tail;
//	TCPlinkList(){
//		head=NULL;
//		tail=NULL;
//	}
//	void inert(string St,string LA,string LP,string RemoteA,string RemoteP,string OwnPID,string OffloadSt)
//	{
//		head=new TCPObject(St,LA,LP,RemoteA,RemoteP,OwnPID,OffloadSt,head);
//	}
//	void ObjectInsert(TCPObject* ptr)
//	{
//		TCPObject* temp=head;
//		head=new TCPObject(ptr);
//		head->SetAddress(temp);
//	}
//	void Count()
//	{
//		int coun=0;
//		TCPObject* temp=head;
//		while(temp!=NULL)
//		{
//			coun++;
//			temp=temp->getAddress();
//		}
//		cout<<"\n\nTotal:"<<coun<<"\n\n";
//	}
//	void Display()
//	{
//		head->Display();
//	}
//
//};