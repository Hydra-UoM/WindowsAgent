#include "stdafx.h"
#include "GetProcessID.h"
#include <comdef.h>		// for using bstr_t class

bool getSetOfProcessID(string processName, std::vector<DWORD>*SetOfPID)
{
	wstring wscons = stringToWidestring(processName);
	LPCTSTR p_Name = wscons.c_str();
	GetProcessID(p_Name, *SetOfPID);
	if (SetOfPID->empty())		// Process is not running
	{
		return false;
	}
	return true;
}

std::wstring stringToWidestring(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

PPERF_OBJECT_TYPE FirstObject(PPERF_DATA_BLOCK PerfData)
{
	return((PPERF_OBJECT_TYPE)((PBYTE)PerfData +
		PerfData->HeaderLength));
}

PPERF_OBJECT_TYPE NextObject(PPERF_OBJECT_TYPE PerfObj)
{
	return((PPERF_OBJECT_TYPE)((PBYTE)PerfObj +
		PerfObj->TotalByteLength));
}

PPERF_INSTANCE_DEFINITION FirstInstance(PPERF_OBJECT_TYPE PerfObj)
{
	return((PPERF_INSTANCE_DEFINITION)((PBYTE)PerfObj +
		PerfObj->DefinitionLength));
}

PPERF_INSTANCE_DEFINITION NextInstance(
	PPERF_INSTANCE_DEFINITION PerfInst)
{
	PPERF_COUNTER_BLOCK PerfCntrBlk;

	PerfCntrBlk = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst +
		PerfInst->ByteLength);

	return((PPERF_INSTANCE_DEFINITION)((PBYTE)PerfCntrBlk +
		PerfCntrBlk->ByteLength));
}

PPERF_COUNTER_DEFINITION FirstCounter(PPERF_OBJECT_TYPE PerfObj)
{
	return((PPERF_COUNTER_DEFINITION)((PBYTE)PerfObj +
		PerfObj->HeaderLength));
}

PPERF_COUNTER_DEFINITION NextCounter(
	PPERF_COUNTER_DEFINITION PerfCntr)
{
	return((PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr +
		PerfCntr->ByteLength));
}


PPERF_COUNTER_BLOCK CounterBlock(PPERF_INSTANCE_DEFINITION PerfInst)
{
	return (PPERF_COUNTER_BLOCK)((LPBYTE)PerfInst + PerfInst->ByteLength);
}

#define TOTALBYTES    64*1024
#define BYTEINCREMENT 1024

#define PROCESS_OBJECT_INDEX	230
#define PROC_ID_COUNTER			784


void GetProcessID(LPCTSTR pProcessName, std::vector<DWORD>& SetOfPID)
{
	PPERF_DATA_BLOCK pPerfData = NULL;
	PPERF_OBJECT_TYPE pPerfObj;
	PPERF_INSTANCE_DEFINITION pPerfInst;
	PPERF_COUNTER_DEFINITION pPerfCntr, pCurCntr;
	PPERF_COUNTER_BLOCK PtrToCntr;
	DWORD BufferSize = TOTALBYTES;
	DWORD i, j;
	LONG k;

	// Allocate the buffer for the performance data.

	pPerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);

	wchar_t szKey[32];
	swprintf_s(szKey, L"%d %d", PROCESS_OBJECT_INDEX, PROC_ID_COUNTER);
	LONG lRes;
	while ((lRes = RegQueryValueEx(HKEY_PERFORMANCE_DATA,
		szKey,
		NULL,
		NULL,
		(LPBYTE)pPerfData,
		&BufferSize)) == ERROR_MORE_DATA)
	{
		// Get a buffer that is big enough.

		BufferSize += BYTEINCREMENT;
		pPerfData = (PPERF_DATA_BLOCK)realloc(pPerfData, BufferSize);
	}

	// Get the first object type.

	pPerfObj = FirstObject(pPerfData);

	// Process all objects.

	for (i = 0; i < pPerfData->NumObjectTypes; i++)
	{

		if (pPerfObj->ObjectNameTitleIndex != PROCESS_OBJECT_INDEX)
		{
			pPerfObj = NextObject(pPerfObj);
			continue;
		}

		SetOfPID.clear();

		// Get the first counter.

		pPerfCntr = FirstCounter(pPerfObj);

		// Get the first instance.

		pPerfInst = FirstInstance(pPerfObj);

		_bstr_t bstrProcessName, bstrInput;

		// Retrieve all instances.
		for (k = 0; k < pPerfObj->NumInstances; k++)
		{
			pCurCntr = pPerfCntr;
			bstrInput = pProcessName;
			bstrProcessName = (wchar_t *)((PBYTE)pPerfInst + pPerfInst->NameOffset);
			if (!_wcsicmp((LPCTSTR)bstrProcessName, (LPCTSTR)bstrInput))
			{
				// Retrieve all counters.

				for (j = 0; j < pPerfObj->NumCounters; j++)
				{
					if (pCurCntr->CounterNameTitleIndex == PROC_ID_COUNTER)
					{
						PtrToCntr = CounterBlock(pPerfInst);
						DWORD *pdwValue = (DWORD*)((LPBYTE)PtrToCntr + pCurCntr->CounterOffset);
						SetOfPID.push_back(*pdwValue);
						break;
					}


					// Get the next counter.

					pCurCntr = NextCounter(pCurCntr);
				}
			}

			// Get the next instance.

			pPerfInst = NextInstance(pPerfInst);
		}
	}
	free(pPerfData);
	RegCloseKey(HKEY_PERFORMANCE_DATA);
}