#include "stdafx.h"
#include "MyTimeStamp.h"

MyTimeStamp::MyTimeStamp(ULONGLONG ullTimeStamp1,SYSTEMTIME& st1,FILETIME& ft1):
			ullTimeStamp(ullTimeStamp1),st(st1),ft(ft1)
{
		ullTimeStamp = ullTimeStamp1;
		st = st1;
		ft = ft1;

		ft.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
		ft.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);

		FileTimeToSystemTime(&ft, &st);
		ullNanoseconds = (ullTimeStamp % 10000000) * 100; // Display nanoseconds instead of milliseconds for higher resolution

		month = st.wMonth;
		day = st.wDay;
		year = st.wYear;
		hour = st.wHour;
		minute = st.wMinute;
		second = st.wSecond;
}

myStruct::myTimeStamp MyTimeStamp::toTimeStampStruct()
{
	myStruct::myTimeStamp m;
	m.year = year;
	m.day = day;
	m.month = month;
	m.second = second;
	m.hour = hour;
	m.minute = minute;
	return m;
}

void MyTimeStamp::ToPrintTimeStamp()
{
	wprintf(L"TimeCreated SystemTime: %02d/%02d/%02d %02d:%02d:%02d.%I64u)\n", month, day, year, hour, minute, second, ullNanoseconds);
}

MyTimeStamp::~MyTimeStamp(void)
{

}