#include<stdio.h>
#include "PollEngine.h"
#include "DataBase.h"

int _tmain(int argc, char* argv[])
{

	//AllocConsole();

	std::vector<RTU> rtus(InitializeRTUs());
	CRITICAL_SECTION cs = InitializeTCPDriver();
	int value = rtus[0].analogInputs[0].Raw;
	rtus[0].analogInputs[0].Raw = 123;
	SOCKET connectSocket = ConnectToRTU(5001, "127.0.0.1");
	T_Header *tHeader = (T_Header*)malloc(sizeof(T_Header));
	tHeader->transactionIdentifier = 1;
	tHeader->protocolIdentifier = 0;
	tHeader->unitIdentifier = 1;
	tHeader->length = 6;
	DWORD itSendDataToModSim;
	DWORD itReceiveDataFromModSim;
	DWORD itPollAllData;
	T_Message *message = (T_Message*)malloc(sizeof(T_Message));
	T_Header *header = (T_Header*)malloc(sizeof(T_Header));
	header->length = 6;
	header->transactionIdentifier = 1;
	header->protocolIdentifier = 0;
	header->unitIdentifier = 1;
	message->header = *header;
	message->data = (char*)malloc(sizeof(char) * 5);
	message->data[0] = 0x01;
	*(short*)(message->data + 1) = 1001;
	*(short*)(message->data + 3) = 0x01;
	
	T_DataForModBusHandle tstruct = { std::vector<T_Message>(), std::vector<T_Message>(), connectSocket };

	T_DataForPolling *dfp = new T_DataForPolling();
	dfp->cs = cs;
	dfp->pollMessages = &tstruct.vectorOfPollMessages;
	dfp->rtus = &rtus;
	//tstruct->vectorOfMessages()
	//EnterCriticalSection(&cs);
	//tstruct.vectorOfPollMessages.push_back(*message);
	//LeaveCriticalSection(&cs);
	HANDLE hReceiveData = CreateThread(NULL, 0, &ReceiveDataFromModSim, &connectSocket, 0, &itReceiveDataFromModSim);
	HANDLE hSendDataToModSim = CreateThread(NULL, 0, &SendDataToModSim, &tstruct, 0, &itSendDataToModSim);
	//PollDataFromRTU(&tstruct.vectorOfPollMessages, 0, 6, 0x01, 0x01, 1001, 1);
	
	HANDLE hPollAllData = CreateThread(NULL, 0, &PollAllData, dfp, 0, &itPollAllData);

	/*PollDataFromRTU(&tstruct.vectorOfPollMessages, 0, 6, 0x01, 0x01, 1001, 1, cs); 
	//Sleep(1);
	PollDataFromRTU(&tstruct.vectorOfPollMessages, 0, 6, 0x01, 0x01, 1001, 1, cs);*/
	/*EnterCriticalSection(cs);
	tstruct.vectorOfMessages.push_back(*message);
	LeaveCriticalSection(cs);*/
	//int iResult = PrepareAndSendRequest(*tHeader, FunctionCode::READ_COILS, 1001, 1, connectSocket);
	//T_Message *message = ReceiveResponse(connectSocket);
	getchar();
	closesocket(connectSocket);
	WSACleanup();
	return true;
}
