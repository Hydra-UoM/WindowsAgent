#include "Manager.h"
#include "MyLogManager.h"
//#include "MyLogRetriever.h"

#define RESULT_POSITION1 5
#define RESULT_POSITION2 26
#define RESULT_POSITION3 32

list<string>logsList;
list<string>userList;

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

char Menu()
{
	char c;
	cout << "******************************Menu*******************************" << endl
		<< left << '*' << setw(38) << " A.Total CPU Usage." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " B.Total Memory Usage." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " C.Total Network Usage (DOWN)." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " D.Total Network Usage (UP)." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " E.Min/Max CPU Process." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " F.Min/Max Memory Process." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " G.Min/Max NetDownlaod Process." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " H.Min/Max NetUpload Process." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " I.All CPU Processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " J.All Memory Processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " K.All Network Download Processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " L.All Network Upload Processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " M.Filter All Processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " N.Filter All Average Processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " O.Get Average Process(PID)." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " P.Get All Processes." << "\t\t\t\t*" << endl


		<< left << '*' << setw(38) << " Q.Get Logon failures." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " R.Get Logs for all processes." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " S.Get Logs for a process." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " T.Get logs for all processes with a security level constraint." << "*" << endl
		<< left << '*' << setw(38) << " U.Get logs for a process with a security level constraint." << "\t*" << endl
		<< left << '*' << setw(38) << " V.Get currently logged in user information." << "\t\t\t*" << endl
		<< left << '*' << setw(38) << " W.Get All user information." << "\t\t\t\t*" << endl
		<< left << '*' << setw(38) << " Y.Get Success Login information." << "\t\t\t\t*" << endl

		<< left << '*' << setw(38) << " X.Exit." << "\t\t\t\t*" << endl
		<<"*****************************************************************"<<endl
		<<endl<< "Insert the option then press ENTER: ";

	c = getchar();
	c = tolower(c);
	cout << endl;
	fflush(stdin);

	return c;
	
}

int main()
{
	MyLogManager manager;
	Manager manage;
	Process p;
	ProcessF pF;
	Data d;
	vector < Process > proc;
	vector<ProcessF> procF;
	double cpu, mem, down, up;
	int sample,PID;
	list<string>::iterator i;

	MyLogRetriever myLogRetriever;
	while (true)
	{
	system("cls");
		//press ESCAPE to exit the functions from i to o
		switch(Menu())
		{
		case 'a':
			gotoxy(0, RESULT_POSITION2);
			cout << "Total CPU usage: " << fixed << setprecision(2) << manage.GetTotalCPU() << " %";
			getch();
			break;
		case 'b':
			gotoxy(0, RESULT_POSITION2);
			cout << "Total Memory usage: " << fixed << setprecision(0) << manage.GetTotalMemory() << " Kb";
			getch();
			break;
		case 'c':
			cout << "Total Network Usage(DOWN): " << fixed << setprecision(1) << manage.GetTotalNetDownload() << " Kbps";
			getch();
			break;
		case 'd':
			cout << "Total Network Usage(UP): " << fixed << setprecision(1) << manage.GetTotalNetUpload() << " Kbps";
			getch();
			break;
		case 'e':
			gotoxy(0, RESULT_POSITION2);
			p = manage.GetMaxCPU();
			cout << "Maximum Cpu Process: " << endl
				<< p.name << " " << p.value <<" %"<< endl;
			p = manage.GetMinCPU();
			cout << "Minimum Cpu Process: " << endl
				<< p.name << " " << p.value << " %";
			getch();
			break;
		case 'f':
			gotoxy(0, RESULT_POSITION2);
			p = manage.GetMaxMemory();
			cout << "Maximum Memory Process: " << endl
				<< p.name << " " << p.value <<" Kb"<< endl;
			p = manage.GetMinMemory();
			cout << "Minimum Memory Process: " << endl
				<< p.name << " " << p.value << " Kb";
			getch();
			break;
		case 'g':
			gotoxy(0, RESULT_POSITION2);
			p = manage.GetMaxNetDownload();
			cout << "Maximum NetDowload Process: " << endl
				<< p.name << " " << p.value <<" Kbps"<< endl;
			p = manage.GetMinNetDownload();
			cout << "Minimum NetUpload Process: " << endl
				<< p.name << " " << p.value<<" Kbps";
			getch();
			break;
		case 'h':
			gotoxy(0, RESULT_POSITION2);
			p = manage.GetMaxNetUpload();
			cout << "Maximum NetUpload Process: " << endl
				<< p.name << " " << p.value << " Kbps"<<endl;
			p = manage.GetMinNetUpload();
			cout << "Minimum NetUpload Process: " << endl
				<< p.name << " " << p.value << " Kbps";
			getch();
			break;
		case 'i':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				cout << left << setw(RESULT_POSITION3) << setprecision(2) << "Process name" << "Usage" << endl;
				proc = manage.GetAllCpuProc();

				for (auto i : proc)
				{
					cout << left << setw(RESULT_POSITION3) << i.name << i.value << " %" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			
			break;
		case 'j':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{	
				system("cls");
				cout << left << setw(RESULT_POSITION3) << "Process name" << "Usage" << endl;
				proc = manage.GetAllMemProc();
			
				for (auto i : proc)
				{
					cout << left << setw(RESULT_POSITION3) << i.name << i.value << " Kb" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			break;
		case 'k':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				cout << left << setw(RESULT_POSITION3) << "Process name" << "Usage" << endl;
				proc = manage.GetAllNetDownloadProc();
				
				for (auto i : proc)
				{
					cout << left << setw(RESULT_POSITION3) << i.name << i.value << " Kbps" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			break;
		case 'l':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				cout << left << setw(RESULT_POSITION3) << "Process name" << "Usage" << endl;
				proc = manage.GetAllNetUploadProc();
	
				for (auto i : proc)
				{
					cout << left << setw(30) << i.name << i.value << " Kbps" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			break;
		case 'm':
			cout << "Enter the CPU usage: "; cin >> cpu; cout << endl;
			cout << "Enter the Memory usage: "; cin >> mem; cout << endl;
			cout << "Enter the Netowrk Download usage: "; cin >> down; cout << endl;
			cout << "Enter the Network Upload usage: "; cin >> up; cout << endl;
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{	
				system("cls");

				cout << setw(RESULT_POSITION3) << left << "Process Name"
					<< right
					<< setw(RESULT_POSITION1) << left << "CPU"
					<< setw(RESULT_POSITION2) << "Mem"
					<< setw(RESULT_POSITION1) << "Down"
					<< setw(RESULT_POSITION1) << "Up" << endl;

				procF = manage.FilterAllProcesses(cpu, mem, down, up);

				for (auto i : procF)
				{
					cout << left << setw(RESULT_POSITION3) << i.name
						<< fixed << setprecision(1) << i.cpu << setw(5) << " %"
						<< i.mem << setw(RESULT_POSITION1) << " Kb"
						<< i.down << setw(RESULT_POSITION1) << " Kbps"
						<< i.up << setw(RESULT_POSITION1) << " Kbps"
						<< endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			break;
		case 'n':
			cout << "Enter the maximum samples: "; cin >> sample; cout << endl;
			cout << "Enter the CPU usage: "; cin >> cpu; cout << endl;
			cout << "Enter the Memory usage: "; cin >> mem; cout << endl;
			cout << "Enter the Netowrk Download usage: "; cin >> down; cout << endl;
			cout << "Enter the Network Upload usage: "; cin >> up; cout << endl;
	
				system("cls");
				cout << setw(RESULT_POSITION3) << left << "Process Name"
					<< right
					<< setw(RESULT_POSITION1) << left << "CPU"
					<< setw(RESULT_POSITION2) << "Mem"
					<< setw(RESULT_POSITION1) << "Down"
					<< setw(RESULT_POSITION1) << "Up" << endl;

				procF = manage.FilterAllAvgProcesses(sample, cpu, mem, down, up);

				for (auto i : procF)
				{
					cout << left << setw(RESULT_POSITION3) << i.name
						<< fixed << setprecision(1) << i.cpu << setw(5) << " %"
						<< i.mem << setw(RESULT_POSITION1) << " Kb"
						<< i.down << setw(RESULT_POSITION1) << " Kbps"
						<< i.up << setw(RESULT_POSITION1) << " Kbps"
						<< endl;
				}
				gotoxy(0,0);
				getch();
			break;
		case 'o':
			cout << "Enter the PID: "; cin >> PID; cout << endl;
			cout << "Enter the samples: "; cin >> sample; cout << endl;

			cout << setw(RESULT_POSITION3) << left << "Process Name"
					<< right
					<< setw(RESULT_POSITION1) << left << "CPU"
					<< setw(RESULT_POSITION2) << "Mem"
					<< setw(RESULT_POSITION1) << "Down"
					<< setw(RESULT_POSITION1) << "Up"
					<< setw(RESULT_POSITION1) << left << "AvgCPU"
					<< setw(RESULT_POSITION2) << "AvgMem"
					<< setw(RESULT_POSITION1) << "AvgDown"
					<< setw(RESULT_POSITION1) << "AvgUp" << endl;

				pF = manage.GetAvgProcess_PID(PID,sample);
	
				cout << left << setw(RESULT_POSITION3) << pF.name
						<< fixed << setprecision(1) << pF.cpu << setw(5) << " %"
						<< pF.mem << setw(RESULT_POSITION1) << " Kb"
						<< pF.down << setw(RESULT_POSITION1) << " Kbps"
						<< pF.up << setw(RESULT_POSITION1) << " Kbps"
						<< pF.avgcpu << setw(5) << " %"
						<< pF.avgmem << setw(RESULT_POSITION1) << " Kb"
						<< pF.avgdown << setw(RESULT_POSITION1) << " Kbps"
						<< pF.avgup << setw(RESULT_POSITION1) << " Kbps"
						<< endl;
				getch();
			break;
		case 'p':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				
				cout << setw(RESULT_POSITION3) << left << "Process Name"
					<< right
					<< setw(RESULT_POSITION1) << left << "CPU"
					<< setw(RESULT_POSITION1) << "ID"
					<< setw(RESULT_POSITION2) << "Mem"
					<< setw(RESULT_POSITION1) << "Down"
					<< setw(RESULT_POSITION1) << "Up"
					<< setw(RESULT_POSITION1) << "avgCPU"
					<< setw(RESULT_POSITION2) << "avgMem"
					<< setw(RESULT_POSITION1) << "avgDown"
					<< setw(RESULT_POSITION1) << "avgUp" << endl;

				procF = manage.GetAllProcesses();

				for (auto i : procF)
				{
					cout << left << setw(RESULT_POSITION3) << i.name
						<< setw(RESULT_POSITION1) << setprecision(0) << i.id
						<< fixed << setprecision(1) << i.cpu << setw(5) << " %"
						<< i.mem << setw(RESULT_POSITION1) << " Kb"
						<< i.down << setw(RESULT_POSITION1) << " Kbps"
						<< i.up << setw(RESULT_POSITION1) << " Kbps"
						<< fixed << setprecision(1) << i.avgcpu << setw(5) << " %"
						<< i.avgmem << setw(RESULT_POSITION1) << " Kb"
						<< i.avgdown << setw(RESULT_POSITION1) << " Kbps"
						<< i.avgup << setw(RESULT_POSITION1) << " Kbps"
						<< endl;
				}
				gotoxy(0, 0); Sleep(2000);
			}
			break;


		case 'q': // Need to test with manual logon failure
			logsList = manager.getLogonFailures();				// return list<string>
			for (i = logsList.begin(); i != logsList.end(); ++i)
			cout << *i << "\n";
			getch();
			break;
		case 'r':
			manager.getLogsForAllProcesses();
			myLogRetriever.handleLogRetrivalInfo(manager.logType, manager.strSecurityLevelConstraint, manager.process_id, manager.timePeriodInMilliSeconds);
			//myLogRetriever.getLogs();
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				//hasLogsListUpdated = false;
				myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery, myLogRetriever.process_id);
				myLogRetriever.printResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);

				//logsList = myLogRetriever.returnResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);
				/*
				for (i = logsList.begin(); i != logsList.end(); ++i)
					cout << *i << "\n";
				*/
				// need to send event in this point
				myLogRetriever.numberOfAvailableEvents = 0;
				//releaseMemory();
				Sleep(myLogRetriever.timePeriodInMilliSeconds);
			}
			break;
		case 's':
			manager.getLogsForAProcess();
			myLogRetriever.handleLogRetrivalInfo(manager.logType, manager.strSecurityLevelConstraint, manager.process_id, manager.timePeriodInMilliSeconds);
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				//hasLogsListUpdated = false;
				myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery, myLogRetriever.process_id);
				myLogRetriever.printResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);

				//logsList = myLogRetriever.returnResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);
				/*
				for (i = logsList.begin(); i != logsList.end(); ++i)
				cout << *i << "\n";
				*/
				// need to send event in this point
				myLogRetriever.numberOfAvailableEvents = 0;
				//releaseMemory();
				Sleep(myLogRetriever.timePeriodInMilliSeconds);
			}
			break;
		case 't':
			manager.getLogsForAllProcessesWithSecurityConstraint();
			myLogRetriever.handleLogRetrivalInfo(manager.logType, manager.strSecurityLevelConstraint, manager.process_id, manager.timePeriodInMilliSeconds);
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				//hasLogsListUpdated = false;
				myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery, myLogRetriever.process_id);
				myLogRetriever.printResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);

				//logsList = myLogRetriever.returnResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);
				/*
				list<string>::iterator i;
				for (i = logsList.begin(); i != logsList.end(); ++i)
				cout << *i << "\n";
				*/
				// need to send event in this point
				myLogRetriever.numberOfAvailableEvents = 0;
				//releaseMemory();
				Sleep(myLogRetriever.timePeriodInMilliSeconds);
			}
			break;
		case 'u':
			manager.getLogsForAProcessWithSecurityConstraint();
			myLogRetriever.handleLogRetrivalInfo(manager.logType, manager.strSecurityLevelConstraint, manager.process_id, manager.timePeriodInMilliSeconds);
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				//hasLogsListUpdated = false;
				myLogRetriever.getEvents(myLogRetriever.lpcwstrLogType, myLogRetriever.pwsQuery, myLogRetriever.process_id);
				myLogRetriever.printResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);

				//logsList = myLogRetriever.returnResultedEvent(myLogRetriever.myLogStructures, myLogRetriever.numberOfAvailableEvents);
				/*
				for (i = logsList.begin(); i != logsList.end(); ++i)
				cout << *i << "\n";
				*/
				// need to send event in this point
				myLogRetriever.numberOfAvailableEvents = 0;
				//releaseMemory();
				Sleep(myLogRetriever.timePeriodInMilliSeconds);
			}
			break;
		case 'v':
			userList = manager.getCurrentLoggedInUser();  // return list<string>
			/**
			for (i = userList.begin(); i != userList.end(); ++i)
				cout << *i << "\n";
			getch();
			*/
			break;
		case 'w':
			userList = manager.getAllUserInformation();	// return list<string>
			/**
			for (i = userList.begin(); i != userList.end(); ++i)
				cout << *i << "\n";
			getch();
			*/
			break;
		case 'y':
			logsList = manager.getSuccessLoginInformation(); // return list<string>
			/**
			for (i = logsList.begin(); i != logsList.end(); ++i)
			cout << *i << "\n";
			getch();
			*/
			break;
		case 'x':
			return 0;
		default:
			cout << "Invalid option!";
			getch();
		}
	}

	return 0;
}