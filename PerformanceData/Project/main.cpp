#include "Manager.h"


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
	cout << "******************Menu******************" << endl
		<< left << '*' << setw(38) << " A.Total CPU Usage." << '*' << endl
		<< left << '*' << setw(38) << " B.Total Memory Usage." << '*' << endl
		<< left << '*' << setw(38) << " C.Total Network Usage (DOWN)." << '*' << endl
		<< left << '*' << setw(38) << " D.Total Network Usage (UP)." << '*' << endl
		<< left << '*' << setw(38) << " E.Min/Max CPU Process." << '*' << endl
		<< left << '*' << setw(38) << " F.Min/Max Memory Process." << '*' << endl
		<< left << '*' << setw(38) << " G.Min/Max NetDownlaod Process." << '*' << endl
		<< left << '*' << setw(38) << " H.Min/Max NetUpload Process." << '*' << endl
		<< left << '*' << setw(38) << " I.All CPU Processes." << '*' << endl
		<< left << '*' << setw(38) << " J.All Memory Processes." << '*' << endl
		<< left << '*' << setw(38) << " K.All Network Download Processes." << '*' << endl
		<< left << '*' << setw(38) << " L.All Network Upload Processes." << '*' << endl
		<< left << '*' << setw(38) << " M.Filter All Processes." << '*' << endl
		<< left << '*' << setw(38) << " N.Filter All Average Processes." << '*' << endl
		<< left << '*' << setw(38) << " O.Get Average Process(PID)." << '*' << endl
		<< left << '*' << setw(38) << " P.Get All Processes." << '*' << endl
		<< left << '*' << setw(38) << " X.Exit." << '*' << endl
		<<"****************************************"<<endl
		<<endl<< "Insert the option then press ENTER: ";

	c = getchar();
	c = tolower(c);
	cout << endl;
	fflush(stdin);

	return c;
	
}

int main()
{

	Manager manage;
	Process p;
	ProcessF pF;
	Data d;
	vector < Process > proc;
	vector<ProcessF> procF;
	double cpu, mem, down, up;
	int sample,PID;

	while (true)
	{
	system("cls");
		//press ESCAPE to exit the functions from i to o
		switch (Menu())
		{
		case 'a':
			gotoxy(0, 15);
			cout << "Total CPU usage: " << fixed << setprecision(2) << manage.GetTotalCPU() << " %";
			getch();
			break;
		case 'b':
			gotoxy(0, 15);
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
			gotoxy(0, 15);
			p = manage.GetMaxCPU();
			cout << "Maximum Cpu Process: " << endl
				<< p.name << " " << p.value <<" %"<< endl;
			p = manage.GetMinCPU();
			cout << "Minimum Cpu Process: " << endl
				<< p.name << " " << p.value << " %";
			getch();
			break;
		case 'f':
			gotoxy(0, 15);
			p = manage.GetMaxMemory();
			cout << "Maximum Memory Process: " << endl
				<< p.name << " " << p.value <<" Kb"<< endl;
			p = manage.GetMinMemory();
			cout << "Minimum Memory Process: " << endl
				<< p.name << " " << p.value << " Kb";
			getch();
			break;
		case 'g':
			gotoxy(0, 15);
			p = manage.GetMaxNetDownload();
			cout << "Maximum NetDowload Process: " << endl
				<< p.name << " " << p.value <<" Kbps"<< endl;
			p = manage.GetMinNetDownload();
			cout << "Minimum NetUpload Process: " << endl
				<< p.name << " " << p.value<<" Kbps";
			getch();
			break;
		case 'h':
			gotoxy(0, 15);
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
				cout << left << setw(25) << setprecision(2)<<"Process name" << "Usage" << endl;
				proc = manage.GetAllCpuProc();

				for (auto i : proc)
				{
					cout << left << setw(25) << i.name << i.value << " %" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			
			break;
		case 'j':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{	
				system("cls");
				cout << left << setw(25) << "Process name" << "Usage" << endl;
				proc = manage.GetAllMemProc();
			
				for (auto i : proc)
				{
					cout << left << setw(25) << i.name << i.value << " Kb" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			break;
		case 'k':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				cout << left << setw(25) << "Process name" << "Usage" << endl;
				proc = manage.GetAllNetDownloadProc();
				
				for (auto i : proc)
				{
					cout << left << setw(25) << i.name << i.value << " Kbps" << endl;
				}
				gotoxy(0,0); Sleep(2000);
			}
			break;
		case 'l':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				cout << left << setw(25) << "Process name" << "Usage" << endl;
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

				cout << setw(25) << left << "Process Name"
					<< right
					<< setw(10) << left << "CPU"
					<< setw(15) << "Mem"
					<< setw(10) << "Down"
					<< setw(10) << "Up" << endl;

				procF = manage.FilterAllProcesses(cpu, mem, down, up);

				for (auto i : procF)
				{
					cout << left << setw(25) << i.name
						<< fixed << setprecision(1) << i.cpu << setw(5) << " %"
						<< i.mem << setw(10) << " Kb"
						<< i.down << setw(10) << " Kbps"
						<< i.up << setw(10) << " Kbps"
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
				cout << setw(25) << left << "Process Name"
					<< right
					<< setw(10) << left << "CPU"
					<< setw(15) << "Mem"
					<< setw(10) << "Down"
					<< setw(10) << "Up" << endl;

				procF = manage.FilterAllAvgProcesses(sample, cpu, mem, down, up);

				for (auto i : procF)
				{
					cout << left << setw(25) << i.name
						<< fixed << setprecision(1) << i.cpu << setw(5) << " %"
						<< i.mem << setw(10) << " Kb"
						<< i.down << setw(10) << " Kbps"
						<< i.up << setw(10) << " Kbps"
						<< endl;
				}
				gotoxy(0,0);
				getch();
			break;
		case 'o':
			cout << "Enter the PID: "; cin >> PID; cout << endl;
			cout << "Enter the samples: "; cin >> sample; cout << endl;

				cout << setw(25) << left << "Process Name"
					<< right
					<< setw(10) << left << "CPU"
					<< setw(15) << "Mem"
					<< setw(10) << "Down"
					<< setw(10) << "Up" 
					<< setw(10) << left << "AvgCPU"
					<< setw(15) << "AvgMem"
					<< setw(10) << "AvgDown"
					<< setw(10) << "AvgUp" << endl;

				pF = manage.GetAvgProcess_PID(PID,sample);
	
					cout << left << setw(25) << pF.name
						<< fixed << setprecision(1) << pF.cpu << setw(5) << " %"
						<< pF.mem << setw(10) << " Kb"
						<< pF.down << setw(10) << " Kbps"
						<< pF.up << setw(10) << " Kbps"
						<< pF.avgcpu << setw(5) << " %"
						<< pF.avgmem << setw(10) << " Kb"
						<< pF.avgdown << setw(10) << " Kbps"
						<< pF.avgup << setw(10) << " Kbps"
						<< endl;
				getch();
			break;
		case 'p':
			while (GetAsyncKeyState(VK_ESCAPE) != true)
			{
				system("cls");
				
				cout << setw(25) << left << "Process Name"
					<< right
					<< setw(10) << left << "CPU"
					<< setw(10)  << "ID"
					<< setw(15) << "Mem"
					<< setw(10) << "Down"
					<< setw(10) << "Up" 
					<< setw(10) << "avgCPU"
					<< setw(15) << "avgMem"
					<< setw(10) << "avgDown"
					<< setw(10) << "avgUp" << endl;

				procF = manage.GetAllProcesses();

				for (auto i : procF)
				{
					cout << left << setw(25) << i.name
						<< setw(10)<<setprecision(0)<<i.id
						<< fixed << setprecision(1) << i.cpu << setw(5) << " %"
						<< i.mem << setw(10) << " Kb"
						<< i.down << setw(10) << " Kbps"
						<< i.up << setw(10) << " Kbps"
						<< fixed << setprecision(1) << i.avgcpu << setw(5) << " %"
						<< i.avgmem << setw(10) << " Kb"
						<< i.avgdown << setw(10) << " Kbps"
						<< i.avgup << setw(10) << " Kbps"
						<< endl;
				}
				gotoxy(0, 0); Sleep(2000);
			}
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
