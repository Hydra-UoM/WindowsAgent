/** for unavailable values checks against L"Unavailable for this event"*/

#include "stdafx.h"
#include "MyAdditionalInformation.h"

MyAdditionalInformation::MyAdditionalInformation(const wchar_t* Weight1,const wchar_t* Conditions1,
								const wchar_t* Condition_ID11,const wchar_t* Match_Value11,const wchar_t* Condition_Value11,
								const wchar_t* Condition_ID21,const wchar_t* Match_Value21,const wchar_t* Condition_Value21,
								const wchar_t* Condition_ID31,const wchar_t* Match_Value31,const wchar_t* Condition_Value31,
								const wchar_t* Filter_Action1):
						Weight(Weight1),Conditions(Conditions1),
						Condition_ID1(Condition_ID11),Match_Value1(Match_Value11),Condition_Value1(Condition_Value11),
						Condition_ID2(Condition_ID21),Match_Value2(Match_Value21),Condition_Value2(Condition_Value21),
						Condition_ID3(Condition_ID31),Match_Value3(Match_Value31),Condition_Value3(Condition_Value31),
						Filter_Action(Filter_Action1)
{
	    Weight = Weight1;
	    Conditions = Conditions1;
		Condition_ID1 = Condition_ID11;
		Match_Value1 = Match_Value11;
		Condition_Value1 = Condition_Value11;
		Condition_ID2 = Condition_ID21;
		Match_Value2 = Match_Value21;
		Condition_Value2 = Condition_Value21;
		Condition_ID3 = Condition_ID31;
		Match_Value3 = Match_Value31;
		Condition_Value3 = Condition_Value31;
		Filter_Action = Filter_Action1;
}

void MyAdditionalInformation::print()
{
	wprintf(L"Additional_Information:\n");
	wprintf(L"*********************************\n");
	wprintf(L"Weight: %s\n", Weight);
	wprintf(L"Conditions: %s\n", Conditions);
	wprintf(L"Condition_ID1: %s\n", Condition_ID1);
	wprintf(L"Match_Value1: %s\n", Match_Value1);
	wprintf(L"Condition_Value1: %s\n", Condition_Value1);
	wprintf(L"Condition_ID2: %s\n", Condition_ID2);
	wprintf(L"Match_Value2: %s\n", Match_Value2);
	wprintf(L"Condition_Value2: %s\n", Condition_Value2);
	wprintf(L"Condition_ID3: %s\n", Condition_ID3);
	wprintf(L"Match_Value3: %s\n", Match_Value3);
	wprintf(L"Condition_Value3: %s\n", Condition_Value3);
	wprintf(L"Filter_Action: %s\n", Filter_Action);
}

MyAdditionalInformation::~MyAdditionalInformation(void)
{

}