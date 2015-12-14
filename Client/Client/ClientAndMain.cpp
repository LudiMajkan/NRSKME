#include<stdio.h>
#include "PollEngine.h"
#include "DataBase.h"
#include "CommandEngine.h"
#include <time.h>

int _tmain(int argc, char* argv[])
{

	//AllocConsole();

	std::vector<RTU> rtus(InitializeRTUs());
	CRITICAL_SECTION *cs = InitializeTCPDriver();
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
	
	// Initialize T_DataForModBusHandle that has vectorOfPollMessages, vectorOfCommandMessages and a socket
	T_DataForModBusHandle tstruct = { std::vector<T_Message>(), std::vector<T_Message>(), connectSocket };

	T_DataForPolling *dfp = new T_DataForPolling();
	dfp->cs = cs[0];
	dfp->pollMessages = &tstruct.vectorOfPollMessages;
	dfp->rtus = &rtus;

	//tstruct->vectorOfMessages()
	//EnterCriticalSection(&cs);
	//tstruct.vectorOfPollMessages.push_back(*message);
	//LeaveCriticalSection(&cs);
	HANDLE hReceiveData = CreateThread(NULL, 0, &ReceiveDataFromModSim, &connectSocket, 0, &itReceiveDataFromModSim);
	HANDLE hSendDataToModSim = CreateThread(NULL, 0, &SendDataToModSim, &tstruct, 0, &itSendDataToModSim);
	Sleep(1);
	//SendCommandToSimulator(&tstruct.vectorOfCommandMessages, 0, 6, 1, 0x06, 4001, 300, cs[1], &rtus);
	//SendCommandToSimulator(&tstruct.vectorOfCommandMessages, 0, 6, 1, 0x06, 4002, 500, cs[1], &rtus);
	SendCommandToSimulator(&tstruct.vectorOfCommandMessages, 0, 6, 1, 0x05, 2001, 1, cs[1], &rtus);
	SendCommandToSimulator(&tstruct.vectorOfCommandMessages, 0, 6, 1, 0x05, 2002, 0, cs[1], &rtus);
	HANDLE hPollAllData = CreateThread(NULL, 0, &PollAllData, dfp, 0, &itPollAllData);

	while(true)
	{
		system("cls");
		printf("ANALOG POINTS: \n");
		printf("Name\t\tAddress\tEGUMin\tEGUMax\tEGU\tRawMin\tRawMax\tRaw\tValue\tStatus\tTimeStamp\n");
		for(int i = 0; i<rtus[0].analogInputNum; i++)
		{
			time_t rawtime = rtus[0].analogInputs[i].timeStamp;
			struct tm *timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			int iResult = 0;
			if(strlen(rtus[0].analogInputs[i].name) >8)
			{
				printf("%s\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%s\t",rtus[0].analogInputs[i].name, rtus[0].analogInputs[i].address, rtus[0].analogInputs[i].EGUMin, 
																  rtus[0].analogInputs[i].EGUMax, rtus[0].analogInputs[i].EGU, rtus[0].analogInputs[i].RawMin,
																  rtus[0].analogInputs[i].RawMax, rtus[0].analogInputs[i].Raw, rtus[0].analogInputs[i].value,
																  rtus[0].analogInputs[i].status);
				printf(asctime(timeinfo));
			}
			else
			{
				printf("%s\t\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%s\t",rtus[0].analogInputs[i].name, rtus[0].analogInputs[i].address, rtus[0].analogInputs[i].EGUMin, 
																  rtus[0].analogInputs[i].EGUMax, rtus[0].analogInputs[i].EGU, rtus[0].analogInputs[i].RawMin,
																  rtus[0].analogInputs[i].RawMax, rtus[0].analogInputs[i].Raw, rtus[0].analogInputs[i].value,
																  rtus[0].analogInputs[i].status);
				printf(asctime(timeinfo));
			}
		}
		printf("DIGITAL DEVICES: \n");
		printf("Name\t\tReadOnly\tInAddress\tOutAddress\tState\tStatus\tCommand\n");
		/*for(int i = 0; i<rtus[0].digitalDevicesNum; i++)
		{
			printf("%s\t%s\t%d|%d\t%d|%d\t",rtus[0].digitalDevices[i].name, rtus[0].digitalDevices[i].readOnly ? "true" : "false", rtus[0].digitalDevices[i].inAddress[0], rtus[0].digitalDevices[i].inAddress[1],
				rtus[0].digitalDevices[i].outAddress[0], rtus[0].digitalDevices[i].outAddress[1], rtus[0].digitalDevices[i].state, rtus[0].digitalDevices[i].status,
				rtus[0].digitalDevices[i].command);
		}*/
		Sleep(2000);
	}

	getchar();
	closesocket(connectSocket);
	WSACleanup();
	return true;
}
