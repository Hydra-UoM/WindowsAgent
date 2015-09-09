#ifndef IO_H
#define IO_H
#include "Data.h"

class IO:public Data
{
public:
	int countD;
	int countU;

	IO();
	virtual ~IO();
	vector<Process> GetAllNetDownloadProc();
	vector<Process> GetAllNetUploadProc();
	double GetTotalNetDownload();
	double GetTotalNetUpload();
	Process GetMaxNetDownload();
	Process GetMinNetDownload();
	Process GetMaxNetUpload();
	Process GetMinNetUpload();
	vector<Process> GetAllAvgNetDownload();
	vector<Process> GetAllAvgNetUpload();
};

#endif