#include "stdafx.h"
#include "MyLogStructure.h"

MyLogStructure::MyLogStructure(/**LPWSTR eventMessageString1, */LPWSTR levelMessageString1, LPWSTR taskMessageString1, LPWSTR opCodeMessageString1,
	LPWSTR channelMessageString1, LPWSTR providerMessageString1, int version1, int level1, int task1, int opCode1, UINT64 keywords1,
	UINT64 eventRecordID1, UINT32 executionProcessID1, UINT32 executionThreadID1, const wchar_t* channel1,
	LPCWSTR computer1, DWORD EventID1, MyTimeStamp& timeStamp1) :

	/**eventMessageString(eventMessageString1), */levelMessageString(levelMessageString1), taskMessageString(taskMessageString1),
	opCodeMessageString(opCodeMessageString1), channelMessageString(channelMessageString1), providerMessageString(providerMessageString1),
	version(version1), level(level1), task(task1), opCode(opCode1), keywords(keywords1), eventRecordID(eventRecordID1),
	executionProcessID(executionProcessID1), channel(channel1), computer(computer1), EventID(EventID1), timeStamp(timeStamp1)/**,mySubject(mySubject1)*/
{
	//eventMessageString = eventMessageString1;
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
	//message = message1;
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

	isAvailableMyLogonType = false;
	isAvailableMyImpersonationLevel = false;
	isAvailableMyAccountForWhichLogonFailed = false;
	isAvailableMyFailureInformation = false;
	isAvailableMyNewLogon = false;
	isAvailableMyDetailedAuthenticationInformation = false;
}

void MyLogStructure::printExtractedEventMessageString()
{
	wcout << message << endl;
	if (isAvailableMySubject)
	{
		mySubject->print();
	}
	if (isAvailableMyObject)
	{
		myObject->print();
	}
	if (isAvailableMyProcessInformation)
	{
		myProcessInformation->print();
	}
	if (isAvailableMyProcessInformation)
	{
		myProcessInformation->print();
	}
	if (isAvailableMyApplicationInformation)
	{
		myApplicationInformation->print();
	}
	if (isAvailableMyNetworkInformation)
	{
		myNetworkInformation->print();
	}
	if (isAvailableMyProviderInformation)
	{
		myProviderInformation->print();
	}
	if (isAvailableMyChangeInformation)
	{
		myChangeInformation->print();
	}
	if (isAvailableMyAccessRequestInformation)
	{
		myAccessRequestInformation->print();
	}
	if (isAvailableMyFilterInformation)
	{
		myFilterInformation->print();
	}
	if (isAvailableMyLayerInformation)
	{
		myLayerInformation->print();
	}
	if (isAvailableMyCalloutInformation)
	{
		myCalloutInformation->print();
	}
	if (isAvailableMyRuleInformation)
	{
		myRuleInformation->print();
	}
	if (isAvailableMyErrorInformation)
	{
		myErrorInformation->print();
	}
	/**
	MyAdditionalInformation myAdditionalInformation;
	*/
}

void MyLogStructure::print()
{
	printExtractedEventMessageString();
	/**
	wprintf(L"EventRecordID: %I64u\n", eventRecordID);
	wprintf(L"EventID: %lu\n", EventID);
	//wprintf(L"Provider Name: %s\n", providerMessageString);
	//wprintf(L"Version: %u\n",version);
	wprintf(L"Level: %u\n", level);
	//wprintf(L"Task: %hu\n",task);
	//wprintf(L"Opcode: %u\n",opCode);
	//wprintf(L"Keywords: 0x%I64x\n",keywords);
	wprintf(L"Execution ProcessID: %lu\n", executionProcessID);
	//cout << "****************************************************" << endl;
	//cout << "****************************************************" << endl;
	//wprintf(L"Execution ThreadID: %lu\n",executionThreadID);
	//wprintf(L"Channel: %s\n",channel);
	//wprintf(L"Computer: %s\n",computer);
	if (eventMessageString)
	{
		wprintf(L"Event message string: %s\n", eventMessageString);
		//free(eventMessageString);
		//eventMessageString = NULL;
	}*/
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

myLogStructure MyLogStructure::toStruct(int summarizationLevel)
{
	//sizeOfEvent = sizeOfEvent + str_eventID.length() + str_level.length() + str_executionProcessID.length() + str_eventMessageString.length();
	myLogStructure m;
	m.myTimeStamp1 = timeStamp.toTimeStampStruct();
	if (summarizationLevel == 2) // High level summarization.
	{
		if (message){ m.message = CW2A(message); }
		//m.levelMessageString = CW2A(levelMessageString);
		//m.taskMessageString = CW2A(taskMessageString);
		//m.opCodeMessageString = CW2A(opCodeMessageString);
		//m.channelMessageString = CW2A(channelMessageString);
		//m.providerMessageString = CW2A(providerMessageString);
		//m.keywords = keywords;
		m.executionProcessID = to_string(executionProcessID);
		m.executionThreadID = to_string(executionThreadID);
		if (computer){ m.computer = CW2A(computer); }
		m.EventID = to_string(EventID);
	}
	else if (summarizationLevel == 1) // Medium level summarization.
	{
		if (message){ m.message = CW2A(message); }
		if (levelMessageString){ m.levelMessageString = CW2A(levelMessageString); }
		if (taskMessageString){ m.taskMessageString = CW2A(taskMessageString); }
		//m.opCodeMessageString = CW2A(opCodeMessageString);
		//m.channelMessageString = CW2A(channelMessageString);
		if (providerMessageString){ m.providerMessageString = CW2A(providerMessageString); }
		//m.keywords = keywords;
		m.executionProcessID = to_string(executionProcessID);
		m.executionThreadID = to_string(executionThreadID);
		if (computer){ m.computer = CW2A(computer); }
		m.EventID = to_string(EventID);
	}
	else
	{
		if (message){ m.message = CW2A(message); }
		if (levelMessageString){ m.levelMessageString = CW2A(levelMessageString); }
		if (taskMessageString){ m.taskMessageString = CW2A(taskMessageString); }
		if (opCodeMessageString){ m.opCodeMessageString = CW2A(opCodeMessageString); }
		if (channelMessageString){ m.channelMessageString = CW2A(channelMessageString); }
		if (providerMessageString){ m.providerMessageString = CW2A(providerMessageString); }
		m.keywords = to_string(keywords);
		m.executionProcessID = to_string(executionProcessID);
		m.executionThreadID = to_string(executionThreadID);
		if (computer){ m.computer = CW2A(computer); }
		m.EventID = to_string(EventID);
	}
	if (summarizationLevel == 2) // High level summarization. Drop some object level information
	{
		if (isAvailableMySubject)
		{
			m.mySubject1 = mySubject->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyObject)
		{
			m.myObject1 = myObject->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyProcessInformation)
		{
			m.myProcessInformation1 = myProcessInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyApplicationInformation)
		{
			m.myApplicationInformation1 = myApplicationInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyNetworkInformation)
		{
			m.myNetworkInformation1 = myNetworkInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyProviderInformation)
		{
			//m.myProviderInformation1 = myProviderInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyChangeInformation)
		{
			m.myChangeInformation1 = myChangeInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyAccessRequestInformation)
		{
			m.myAccessRequestInformation1 = myAccessRequestInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyFilterInformation)
		{
			//m.myFilterInformation1 = myFilterInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyLayerInformation)
		{
			//m.myLayerInformation1 = myLayerInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyCalloutInformation)
		{
			//m.myCalloutInformation1 = myCalloutInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyRuleInformation)
		{
			//m.myRuleInformation1 = myRuleInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyErrorInformation)
		{
			m.myErrorInformation1 = myErrorInformation->toLogStruct(summarizationLevel);
		}
	}
	else
	{
		if (isAvailableMySubject)
		{
			m.mySubject1 = mySubject->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyObject)
		{
			m.myObject1 = myObject->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyProcessInformation)
		{
			m.myProcessInformation1 = myProcessInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyApplicationInformation)
		{
			m.myApplicationInformation1 = myApplicationInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyNetworkInformation)
		{
			m.myNetworkInformation1 = myNetworkInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyProviderInformation)
		{
			m.myProviderInformation1 = myProviderInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyChangeInformation)
		{
			m.myChangeInformation1 = myChangeInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyAccessRequestInformation)
		{
			m.myAccessRequestInformation1 = myAccessRequestInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyFilterInformation)
		{
			m.myFilterInformation1 = myFilterInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyLayerInformation)
		{
			m.myLayerInformation1 = myLayerInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyCalloutInformation)
		{
			m.myCalloutInformation1 = myCalloutInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyRuleInformation)
		{
			m.myRuleInformation1 = myRuleInformation->toLogStruct(summarizationLevel);
		}
		if (isAvailableMyErrorInformation)
		{
			m.myErrorInformation1 = myErrorInformation->toLogStruct(summarizationLevel);
		}
	}
	//m.channel = CW2A(channel);
	//m.version = version;
	//m.level = level;
	//m.task = task;
	//m.opCode = opCode;
	//m.eventRecordID = eventRecordID;
	//m.channel = CW2A(channel);
	return m;
}

void MyLogStructure::releaseMemory()
{
	delete mySubject;
	delete myProviderInformation;
	delete myObject;
	delete myNetworkInformation;
	delete myLayerInformation;
	delete myFilterInformation;
	delete myChangeInformation;
	delete myCalloutInformation;
	delete myApplicationInformation;
	delete myAdditionalInformation;
	delete myAccessRequestInformation;
	delete myRuleInformation;
	delete myErrorInformation;
	delete myProcessInformation;

	delete myLogonType;
	delete myImpersonationLevel;
	delete myAccountForWhichLogonFailed;
	delete myFailureInformation;
	delete myNewLogon;
	delete myDetailedAuthenticationInformation;
}

MyLogStructure::~MyLogStructure(void)
{

}