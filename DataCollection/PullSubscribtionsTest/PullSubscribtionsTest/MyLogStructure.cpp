#include "stdafx.h"
#include "MyLogStructure.h"

MyLogStructure::MyLogStructure(LPWSTR eventMessageString1,LPWSTR levelMessageString1,LPWSTR taskMessageString1,LPWSTR opCodeMessageString1,
			LPWSTR channelMessageString1,LPWSTR providerMessageString1,int version1,int level1,int task1,int opCode1,UINT64 keywords1,
			UINT64 eventRecordID1,UINT32 executionProcessID1,UINT32 executionThreadID1,const wchar_t* channel1,
			LPCWSTR computer1,DWORD EventID1,MyTimeStamp& timeStamp1/**,WCHAR* processImageName1*/):

			eventMessageString(eventMessageString1),levelMessageString(levelMessageString1),taskMessageString(taskMessageString1),
			opCodeMessageString(opCodeMessageString1),channelMessageString(channelMessageString1),providerMessageString(providerMessageString1),
			version(version1),level(level1),task(task1),opCode(opCode1),keywords(keywords1),eventRecordID(eventRecordID1),
			executionProcessID(executionProcessID1),channel(channel1),computer(computer1),EventID(EventID1),timeStamp(timeStamp1)/**,
			processImageName(processImageName1)*/
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
		extractEventMessageString();
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

		mySubject = (MySubject*)malloc(sizeof(MySubject));
}

void MyLogStructure::extractEventMessageString()
{
	LPWSTR separators = L":\n\t";
	vector<wstring> results = splitLPWSTRWithManyDelimiters(eventMessageString, separators);
	int noOfSplitedStrings = 0;
	enum attributeName{Message,Client_ID,
		Source_Handle_Information,Source_Handle_ID,Source_Process_ID,
		New_Handle_Information,Target_Handle_ID,Target_Process_ID
		};
	const wchar_t*arrayOfSplitted[200];
    for (auto const& w : results)
	{
		const wchar_t* st = w.c_str();
        //wprintf(L"Splitted: %ls\n", st);
		arrayOfSplitted[noOfSplitedStrings] = st;
		noOfSplitedStrings++;
	}
	//wprintf(L"Message: %ls\n", arrayOfSplitted[0]);
	message = arrayOfSplitted[0];
	// Setup logs contains Client id
	int i = 1;
	while(i < noOfSplitedStrings)
	{
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Subject") == 0)// There is Subject type
		{
			isAvailableMySubject = true;
			i = i + 2;
			const wchar_t* Security_ID;
			const wchar_t* Account_Name;
			const wchar_t* Account_Domain;
			const wchar_t* Logon_ID;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Security ID") == 0)
			{
				i++;
				Security_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Security_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Account Name") == 0)
			{
				i++;
				Account_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Account_Name = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Account Domain") == 0)
			{
				i++;
				Account_Domain = arrayOfSplitted[i];
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Account_Domain = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Logon ID") == 0)
			{
				i++;
				Logon_ID = arrayOfSplitted[i];
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Logon_ID = L"Unavailable for this event";
			}
			mySubject = new MySubject(Security_ID,Account_Name,Account_Domain,Logon_ID);
			//*mySubject = mySubject1;
			//mySubject->print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Object") == 0)// There is Object type
		{
			isAvailableMyObject = true;
			i = i + 2;
			const wchar_t* Object_Server;
			const wchar_t* Object_Type;
			const wchar_t* Object_Name;
			const wchar_t* Handle_ID;
			const wchar_t* Resource_Attributes;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Object Server") == 0)
			{
				i++;
				Object_Server = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Object_Server = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Object Type") == 0)
			{
				i++;
				Object_Type = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Object_Type = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Object Name") == 0)
			{
				i++;
				Object_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Domain: %ls\n", Account_Domain);
				i++;
			}
			else
			{
				Object_Name = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Handle ID") == 0)
			{
				i++;
				Handle_ID = arrayOfSplitted[i];
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Handle_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Resource Attributes") == 0)
			{
				i++;
				Resource_Attributes = arrayOfSplitted[i];
				//wprintf(L"Logon_ID: %ls\n", Logon_ID);
				i++;
			}
			else
			{
				Resource_Attributes = L"Unavailable for this event";
			}
			MyObject myObject1(Object_Server,Object_Type,Object_Name,Handle_ID,Resource_Attributes);
				//*mySubject = mySubject1;
			//myObject1.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Process Information") == 0)// There is Process Information type
		{
			isAvailableMyProcessInformation = true;
			i = i + 2;
			const wchar_t* Process_ID;
			const wchar_t* Process_Name;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Process ID") == 0)
			{
				i++;
				Process_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Process_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Process Name") == 0)
			{
				i++;
				Process_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Process_Name = L"Unavailable for this event";
			}
			MyProcessInformation myProcessInformation1(Process_ID,Process_Name);
				//*mySubject = mySubject1;
			//myProcessInformation1.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Application Information") == 0)// There is Application Information type
		{
			isAvailableMyApplicationInformation = true;
			i = i + 2;
			const wchar_t* Application_Process_ID;
			const wchar_t* Application_Name;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Process ID") == 0)
			{
				i++;
				Application_Process_ID = arrayOfSplitted[i];
				//wprintf(L"Application_Process_ID: %ls\n", Application_Process_ID);
				i++;
			}
			else
			{
				Application_Process_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Application Name") == 0)
			{
				i++;
				Application_Name = arrayOfSplitted[i];
				//wprintf(L"Application_Name: %ls\n", Application_Name);
				i++;
			}
			else
			{
				Application_Name = L"Unavailable for this event";
			}
			MyApplicationInformation myApplicationInformation(Application_Process_ID,Application_Name);
				//*mySubject = mySubject1;
			//myApplicationInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Network Information") == 0)// There is Network Information type
		{
			isAvailableMyNetworkInformation = true;
			i = i + 2;
			const wchar_t* Direction;
			const wchar_t* Source_Address;
			const wchar_t* Source_Port;
			const wchar_t* Destination_Address;
			const wchar_t* Destination_Port;
			const wchar_t* Protocol;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Direction") == 0)
			{
				i++;
				Direction = arrayOfSplitted[i];
				//wprintf(L"Direction: %ls\n", Direction);
				i++;
			}
			else
			{
				Direction = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Source Address") == 0)
			{
				i++;
				Source_Address = arrayOfSplitted[i];
				//wprintf(L"Source_Address: %ls\n", Source_Address);
				i++;
			}
			else
			{
				Source_Address = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Source Port") == 0)
			{
				i++;
				Source_Port = arrayOfSplitted[i];
				//wprintf(L"Source_Port: %ls\n", Source_Port);
				i++;
			}
			else
			{
				Source_Port = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Destination Address") == 0)
			{
				i++;
				Destination_Address = arrayOfSplitted[i];
				//wprintf(L"Destination_Address: %ls\n", Destination_Address);
				i++;
			}
			else
			{
				Destination_Address = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Destination Port") == 0)
			{
				i++;
				Destination_Port = arrayOfSplitted[i];
				//wprintf(L"Destination_Port: %ls\n", Destination_Port);
				i++;
			}
			else
			{
				Destination_Port = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Protocol") == 0)
			{
				i++;
				Protocol = arrayOfSplitted[i];
				//wprintf(L"Protocol: %ls\n", Protocol);
				i++;
			}
			else
			{
				Protocol = L"Unavailable for this event";
			}
			MyNetworkInformation myNetworkInformation(Direction,Source_Address,Source_Port,Destination_Address,Destination_Port,Protocol);
				//*mySubject = mySubject1;
			//myNetworkInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Provider Information") == 0)// There is Provider Information type
		{
			isAvailableMyProviderInformation = true;
			i = i + 2;
			const wchar_t* Provider_ID;
			const wchar_t* Provider_Name;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"ID") == 0)
			{
				i++;
				Provider_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Provider_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Name") == 0)
			{
				i++;
				Provider_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Provider_Name = L"Unavailable for this event";
			}
			MyProviderInformation myProviderInformation(Provider_ID,Provider_Name);
				//*mySubject = mySubject1;
			//myProviderInformation.print();
		}
		if(wcscmp(arrayOfSplitted[i],L"Change Information") == 0)// There is Change Information type
		{
			isAvailableMyChangeInformation = true;
			i = i + 2;
			const wchar_t* Change_Type;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Change Type") == 0)
			{
				i++;
				Change_Type = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Change_Type = L"Unavailable for this event";
			}
			MyChangeInformation myChangeInformation(Change_Type);
				//*mySubject = mySubject1;
			//myChangeInformation.print();
		}
		if(wcscmp(arrayOfSplitted[i],L"Access Request Information") == 0)// There is Access Request Information type
		{
			isAvailableMyAccessRequestInformation = true;
			i = i + 2;
			const wchar_t* Transaction_ID;
			const wchar_t* Accesses;
			const wchar_t* Access_Reasons;
			const wchar_t* Access_Mask;
			const wchar_t* Privileges_Used_For_Access_Check;
			const wchar_t* Restricted_SID_Count;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Transaction ID") == 0)
			{
				i++;
				Transaction_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Transaction_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Accesses") == 0)
			{
				i++;
				Accesses = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Accesses = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Access Reasons") == 0)
			{
				i++;
				Access_Reasons = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Access_Reasons = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Access Mask") == 0)
			{
				i++;
				Access_Mask = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Access_Mask = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Privileges Used for Access Check") == 0)
			{
				i++;
				Privileges_Used_For_Access_Check = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Privileges_Used_For_Access_Check = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Restricted SID Count") == 0)
			{
				i++;
				Restricted_SID_Count = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Restricted_SID_Count = L"Unavailable for this event";
			}
			MyAccessRequestInformation myAccessRequestInformation(Transaction_ID,Accesses,Access_Reasons,Access_Mask,
					Privileges_Used_For_Access_Check,Restricted_SID_Count);
				//*mySubject = mySubject1;
			//myAccessRequestInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Filter Information") == 0)// There is Filter Information type
		{
			isAvailableMyFilterInformation = true;
			i = i + 2;
			const wchar_t* Filter_ID;
			const wchar_t* Filter_Name;
			const wchar_t* Filter_Type;
			const wchar_t* Runtime_ID;
			const wchar_t* Filter_Runtime_ID;
			const wchar_t* Filter_Layer_Name;
			const wchar_t* Filter_Layer_Runtime_ID;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"ID") == 0)
			{
				i++;
				Filter_ID = arrayOfSplitted[i];
				//wprintf(L"Filter_ID: %ls\n", Filter_ID);
				i++;
			}
			else
			{
				Filter_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Name") == 0)
			{
				i++;
				Filter_Name = arrayOfSplitted[i];
				//wprintf(L"Filter_Name: %ls\n", Filter_Name);
				i++;
			}
			else
			{
				Filter_Name = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Type") == 0)
			{
				i++;
				Filter_Type = arrayOfSplitted[i];
				//wprintf(L"Filter_Type: %ls\n", Filter_Type);
				i++;
			}
			else
			{
				Filter_Type = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Run-Time ID") == 0)
			{
				i++;
				Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Runtime_ID: %ls\n", Runtime_ID);
				i++;
			}
			else
			{
				Runtime_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Filter Run-Time ID") == 0)
			{
				i++;
				Filter_Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Filter_Runtime_ID: %ls\n", Filter_Runtime_ID);
				i++;
			}
			else
			{
				Filter_Runtime_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Layer Name") == 0)
			{
				i++;
				Filter_Layer_Name = arrayOfSplitted[i];
				//wprintf(L"Filter_Layer_Name: %ls\n", Filter_Layer_Name);
				i++;
			}
			else
			{
				Filter_Layer_Name = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Layer Run-Time ID") == 0)
			{
				i++;
				Filter_Layer_Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Filter_Layer_Runtime_ID: %ls\n", Filter_Layer_Runtime_ID);
				i++;
			}
			else
			{
				Filter_Layer_Runtime_ID = L"Unavailable for this event";
			}
			MyFilterInformation myFilterInformation(Filter_ID,Filter_Name,Filter_Type,Runtime_ID,Filter_Runtime_ID,Filter_Layer_Name,
										 Filter_Layer_Runtime_ID);
				//*mySubject = mySubject1;
			//myFilterInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Layer Information") == 0)// There is Layer Information type
		{
			isAvailableMyLayerInformation = true;
			i = i + 2;
			const wchar_t* Layer_ID;
			const wchar_t* Layer_Name;
			const wchar_t* Layer_Runtime_ID;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"ID") == 0)
			{
				i++;
				Layer_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Layer_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Name") == 0)
			{
				i++;
				Layer_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Layer_Name = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Run-Time ID") == 0)
			{
				i++;
				Layer_Runtime_ID = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Layer_Runtime_ID = L"Unavailable for this event";
			}
			MyLayerInformation myLayerInformation(Layer_ID,Layer_Name,Layer_Runtime_ID);
				//*mySubject = mySubject1;
			//myLayerInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Callout Information") == 0)// There is Callout Information type
		{
			isAvailableMyCalloutInformation = true;
			i = i + 2;
			const wchar_t* Callout_ID;
			const wchar_t* Callout_Name;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"ID") == 0)
			{
				i++;
				Callout_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Callout_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Name") == 0)
			{
				i++;
				Callout_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Callout_Name = L"Unavailable for this event";
			}
			MyCalloutInformation myCalloutInformation(Callout_ID,Callout_Name);
				//*mySubject = mySubject1;
			//myCalloutInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Rule Information") == 0)// There is Rule Information type
		{
			isAvailableMyRuleInformation = true;
			i = i + 2;
			const wchar_t* Rule_ID;
			const wchar_t* Rule_Name;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"ID") == 0)
			{
				i++;
				Rule_ID = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Rule_ID = L"Unavailable for this event";
			}
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Name") == 0)
			{
				i++;
				Rule_Name = arrayOfSplitted[i];
				//wprintf(L"Account_Name: %ls\n", Account_Name);
				i++;
			}
			else
			{
				Rule_Name = L"Unavailable for this event";
			}
			MyRuleInformation myRuleInformation(Rule_ID,Rule_Name);
				//*mySubject = mySubject1;
			//myRuleInformation.print();
		}
		if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Error Information") == 0)// There is Error Information type
		{
			isAvailableMyErrorInformation = true;
			i = i + 2;
			const wchar_t* Error_Reason;
			if(i < noOfSplitedStrings && wcscmp(arrayOfSplitted[i],L"Reason") == 0)
			{
				i++;
				Error_Reason = arrayOfSplitted[i];
				//wprintf(L"Security_ID: %ls\n", Security_ID);
				i++;
			}
			else
			{
				Error_Reason = L"Unavailable for this event";
			}
			MyErrorInformation myErrorInformation(Error_Reason);
				//*mySubject = mySubject1;
			//myErrorInformation.print();
		}
		i++;
	}
}

std::vector<wstring> MyLogStructure::splitLPWSTRWithManyDelimiters(const wstring &original, const wstring &delimiters)
{
    std::wstringstream stream(original);
    std::wstring line;
    vector <wstring> wordVector;

    while (std::getline(stream, line))
    {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(delimiters, prev)) != std::wstring::npos)
        {
            if (pos > prev)
                wordVector.emplace_back(line.substr(prev, pos-prev));

            prev = pos + 1;
        }

        if (prev < line.length())
            wordVector.emplace_back(line.substr(prev, std::wstring::npos));
    }

    return wordVector;
}

void MyLogStructure::printExtractedEventMessageString()
{
		wprintf(L"Message: %ls\n", message);
		if(isAvailableMySubject)
		{
			std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
			mySubject->print();
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
	printExtractedEventMessageString();
	//wprintf(L"EventRecordID: %I64u\n",eventRecordID);
	wprintf(L"EventID: %lu\n", EventID);
	//wprintf(L"Provider Name: %s\n", providerMessageString);
    //wprintf(L"Version: %u\n",version);
    //wprintf(L"Level: %u\n",level);
    //wprintf(L"Task: %hu\n",task);
    //wprintf(L"Opcode: %u\n",opCode);
	//wprintf(L"Keywords: 0x%I64x\n",keywords);
	wprintf(L"Execution ProcessID: %lu\n",executionProcessID);
    //wprintf(L"Execution ThreadID: %lu\n",executionThreadID);
    //wprintf(L"Channel: %s\n",channel);
    //wprintf(L"Computer: %s\n",computer);
	/**if (eventMessageString)
    {
        wprintf(L"Event message string: %s\n", eventMessageString);
        free(eventMessageString);
        eventMessageString = NULL;
    }
	*/
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
	timeStamp.ToPrintTimeStamp();
	//_tprintf( TEXT("Process's image name:  %ls\n\n"),processImageName);
}

MyLogStructure::~MyLogStructure(void)
{

}