#include "stdafx.h"
#include "GetProcessImageNameFromPID.h"

BOOL GetProcessImageNameFromPID::getProcessNameFromProcessID(DWORD processId,WCHAR**processName)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
	printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
	return( FALSE );
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
	printError( TEXT("Process32First") ); // show cause of failure
	CloseHandle( hProcessSnap );          // clean the snapshot object
	return( FALSE );
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	int i = 0;
	do
	{
	 WCHAR*allprocessName = pe32.szExeFile;
	//_tprintf( TEXT("\n%d)PROCESS NAME:  %s"), i, allprocessName);

	// Retrieve the priority class.
	dwPriorityClass = 0;
	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
	if( hProcess == NULL )
		printError( TEXT("OpenProcess") );
	else
	{
		dwPriorityClass = GetPriorityClass( hProcess );
		if( !dwPriorityClass )
		printError( TEXT("GetPriorityClass") );
		CloseHandle( hProcess );
	}
	DWORD pid = pe32.th32ProcessID;
	//_tprintf( TEXT("\n  Process ID        = %d"), pid );
	if(pid == processId)
	{
		*processName = allprocessName;
		//_tprintf( TEXT("Inside Method:\n"));
		//_tprintf( TEXT("PROCESS NAME:  %s\n\n"),*processName);
		return TRUE;
	}
	i++;
	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return( FALSE );
}

void GetProcessImageNameFromPID::printError( TCHAR* msg )
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
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
 // _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}

GetProcessImageNameFromPID::~GetProcessImageNameFromPID(void)
{

}