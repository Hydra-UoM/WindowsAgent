#include <vector>
#include <windows.h>
#include <iostream>
#include <stdio.h>

using namespace std;

void GetProcessID(LPCTSTR pProcessName, std::vector<DWORD>& SetOfPID);
std::wstring stringToWidestring(const std::string& s);
bool getSetOfProcessID(string processName, std::vector<DWORD>*SetOfPID);