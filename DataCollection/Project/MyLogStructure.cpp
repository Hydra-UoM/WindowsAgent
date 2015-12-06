#include "stdafx.h"
#include "MyLogStructure.h"

MyLogStructure::MyLogStructure(LPWSTR eventMessageString1,LPWSTR levelMessageString1,LPWSTR taskMessageString1,LPWSTR opCodeMessageString1,
			LPWSTR channelMessageString1,LPWSTR providerMessageString1,int version1,int level1,int task1,int opCode1,UINT64 keywords1,
			UINT64 eventRecordID1,UINT32 executionProcessID1,UINT32 executionThreadID1,const wchar_t* channel1,
			LPCWSTR computer1, DWORD EventID1, MyTimeStamp& timeStamp1/**,WCHAR* processImageName1, MySubject*mySubject1*/) :

			eventMessageString(eventMessageString1),levelMessageString(levelMessageString1),taskMessageString(taskMessageString1),
			opCodeMessageString(opCodeMessageString1),channelMessageString(channelMessageString1),providerMessageString(providerMessageString1),
			version(version1),level(level1),task(task1),opCode(opCode1),keywords(keywords1),eventRecordID(eventRecordID1),
			executionProcessID(executionProcessID1),channel(channel1),computer(computer1),EventID(EventID1),timeStamp(timeStamp1)/**,
			processImageName(processImageName1),mySubject(mySubject1)*/
{
	    eventMessageString = eventMessageString1;
	    levelMessageString = levelMessageString1;
		taskMessageString = taskMessageString1;
		opCodeMessageString = opCodeMessageString1;
		channelMessageString = channelMessageString1;
		providerMessageString = providerMessageString1;

		version = version1;
		level = level1;
		task = task1;
		opCode = opCode1;
		keywords = keywords1;
		eventRecordID = eventRecordID1;
		executionProcessID = executionProcessID1;
		executionThreadID = executionThreadID1;
		channel = channel1;
		computer = computer1;
		EventID = EventID1;
		timeStamp = timeStamp1;
		//processImageName = processImageName1;
		initializeAvailableInformation();
}

string MyLogStructure::wchar_t_pointerToString(const wchar_t*wchar_t_pointer_text)
{
		wstring wstring_text(wchar_t_pointer_text);
		string string_text(wstring_text.begin(), wstring_text.end());
		return string_text;
}

void MyLogStructure::initializeAvailableInformation()
{
		isAvailableMySubject = false;
		isAvailableMyProviderInformation = false;
		isAvailableMyProcessInformation = false;
		isAvailableMyObject = false;
		isAvailableMyNetworkInformation = false;
		isAvailableMyLayerInformation = false;
		isAvailableMyFilterInformation = false;
		isAvailableMyChangeInformation = false;
		isAvailableMyCalloutInformation = false;
		isAvailableMyApplicationInformation = false;
		isAvailableMyAdditionalInformation = false;
		isAvailableMyAccessRequestInformation = false;
		isAvailableMyErrorInformation = false;
		isAvailableMyRuleInformation = false;

		//mySubject = (MySubject*)malloc(sizeof(MySubject));
}

void MyLogStructure::printExtractedEventMessageString()
{
		//wprintf(L"Message: %ls\n", message);
		if(isAvailableMySubject)
		{
			//std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
			//mySubject->print();
		}
		/**
		MySubject mySubject;
		MyProviderInformation myProviderInformation;
		MyProcessInformation myProcessInformation;
		MyObject myObject;
		MyNetworkInformation myNetworkInformation;
		MyLayerInformation myLayerInformation;
		MyFilterInformation myFilterInformation;
		MyChangeInformation myChangeInformation;
		MyCalloutInformation myCalloutInformation;
		MyApplicationInformation myApplicationInformation;
		MyAdditionalInformation myAdditionalInformation;
		MyAccessRequestInformation myAccessRequestInformation;
		*/
		/**
		isAvailableMyProviderInformation = false;
		isAvailableMyProcessInformation = false;
		isAvailableMyObject = false;
		isAvailableMyNetworkInformation = false;
		isAvailableMyLayerInformation = false;
		isAvailableMyFilterInformation = false;
		isAvailableMyChangeInformation = false;
		isAvailableMyCalloutInformation = false;
		isAvailableMyApplicationInformation = false;
		isAvailableMyAdditionalInformation = false;
		isAvailableMyAccessRequestInformation = false;
		*/
}

void MyLogStructure::print()
{
	wprintf(L"EventRecordID: %I64u\n",eventRecordID);
	wprintf(L"EventID: %lu\n", EventID);
	//wprintf(L"Provider Name: %s\n", providerMessageString);
    //wprintf(L"Version: %u\n",version);
    wprintf(L"Level: %u\n",level);
    //wprintf(L"Task: %hu\n",task);
    //wprintf(L"Opcode: %u\n",opCode);
	//wprintf(L"Keywords: 0x%I64x\n",keywords);
	wprintf(L"Execution ProcessID: %lu\n",executionProcessID);
	printExtractedEventMessageString();
    //wprintf(L"Execution ThreadID: %lu\n",executionThreadID);
    //wprintf(L"Channel: %s\n",channel);
    //wprintf(L"Computer: %s\n",computer);
	if (eventMessageString)
    {
        wprintf(L"Event message string: %s\n", eventMessageString);
        free(eventMessageString);
        eventMessageString = NULL;
    }
	/**
	// don't need to extract
    if (levelMessageString)
    {
        wprintf(L"Level message string: %s\n", levelMessageString);
        free(levelMessageString);
        levelMessageString = NULL;
    }
	// don't need to extract
    if (taskMessageString)
    {
        wprintf(L"Task message string: %s\n", taskMessageString);
        free(taskMessageString);
        taskMessageString = NULL;
    }
	// don't need to extract
	if (opCodeMessageString)
    {
        wprintf(L"Opcode message string: %s\n", opCodeMessageString);
        free(opCodeMessageString);
        opCodeMessageString = NULL;
    }
	// don't need to extract
	// no for Setup && Operational logs
    if (channelMessageString)
    {
        wprintf(L"Channel message string: %s\n\n", channelMessageString);
        free(channelMessageString);
        channelMessageString = NULL;
    }
	// don't need to extract
	if (providerMessageString)
    {
        wprintf(L"Provider message string: %s\n\n", providerMessageString);
        free(providerMessageString);
        providerMessageString = NULL;
    }
	*/
	//timeStamp.ToPrintTimeStamp();
	//_tprintf( TEXT("Process's image name:  %ls\n\n"),processImageName);
}

list<string> MyLogStructure::addExtractedString(list<string> prepart) /** Need to fix*/
{
	return prepart;
}

list<string> MyLogStructure::toLogString() /** Need to fix*/
{	
	list<string> logString;
	//string str_eventRecordID = to_string(eventRecordID);			logString.push_back(str_eventRecordID);
	string str_eventID = to_string(EventID);						logString.push_back(str_eventID);
	string str_level = to_string(level);							logString.push_back(str_level);
	string str_executionProcessID = to_string(executionProcessID);	logString.push_back(str_executionProcessID);

	logString = addExtractedString(logString);

	string str_eventMessageString = CW2A(eventMessageString);		logString.push_back(str_eventMessageString);
	return logString;
}


MyLogStructure::~MyLogStructure(void)
{

}