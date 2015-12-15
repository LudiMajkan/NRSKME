#include<stdio.h>
#include "PollEngine.h"
#include "DataBase.h"
#include "CommandEngine.h"
#include <time.h>

HANDLE semaphoreForClient;
bool shutdownApp = false;

typedef struct StructForCommandClient
{
	std::vector<RTU> *rtus;
	std::vector<T_Message> *vectorOfCommandMessages;
	CRITICAL_SECTION csForCommandVector;
} T_StructForCommandClient;

DWORD WINAPI SoundAlarm(LPVOID lParam)
{
	AnalogInput *ai = (AnalogInput*)lParam;
	while (!shutdownApp)
	{
		if (ai->inAlarm)
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			system("COLOR 4F");
			printf("\nALARM, CONFIRM!!!!!!");
			getchar();
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
			while (ai->inAlarm)
			{
				printf("Alarm, analogInput name: %s\n", ai->name);
				Sleep(2000);
			}
			system("COLOR 0F");
		}
		Sleep(1000);
	}
	
	return 0;
}

DWORD WINAPI CommandFromClient(LPVOID lParam)
{
	T_StructForCommandClient *client = (T_StructForCommandClient*)lParam;
	int value = 0;
	while(true)
	{
		char c = getchar();
		if(c == '1')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			printf("Enter consumption: ");
			scanf("%d", &value);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2001, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2002, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4001, value, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '2')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2001, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2002, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4001, 0, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '3')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			printf("Enter consumption: ");
			scanf("%d", &value);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2003, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2004, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4002, value, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '4')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2003, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2004, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4002, 0, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '5')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			printf("Enter consumption: ");
			scanf("%d", &value);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2005, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2006, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4003, value, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '6')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2005, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2006, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4003, 0, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '7')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			printf("Enter consumption: ");
			scanf("%d", &value);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2007, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2008, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4004, value, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if(c == '8')
		{
			WaitForSingleObject(semaphoreForClient, INFINITE);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2007, 0, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x05, 2008, 1, client->csForCommandVector, client->rtus);
			SendCommandToSimulator(client->vectorOfCommandMessages, 0, 6, 1, 0x06, 4004, 0, client->csForCommandVector, client->rtus);
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
		}
		else if (c == '9')
		{
			#pragma region exiting
			WaitForSingleObject(semaphoreForClient, INFINITE);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("================================================================================================================\n");
			printf("|                                                   GOODBYE!                                                   |\n");
			printf("================================================================================================================\n");
			Sleep(100);
			shutdownApp = true;
			ReleaseSemaphore(semaphoreForClient, 1, NULL);
			break;
			#pragma endregion
		}
	}
	return 0;
}

DWORD WINAPI RefreshConsole(LPVOID lParam)
{
	std::vector<RTU> *rtus = (std::vector<RTU>*)lParam;
	while(true)
	{
		WaitForSingleObject(semaphoreForClient, INFINITE);
		if (shutdownApp)
		{
			break;
		}
		system("cls");
		printf("================================================================================================================\n");
		printf("|                                         WELCOME TO OUR LITTLE SCADA!                                         |\n");
		printf("================================================================================================================\n");
		printf("RTU ID : 1\n");
		printf("RTU port : 5001\n");
		printf("RTU ipAddress : %s\n", rtus->at(0).ipAddress);
		printf("================================================================================================================\n");
		printf("ANALOG POINTS: \n");
		printf("----------------------------------------------------------------------------------------------------------------\n");
		printf("Name\t\tAddress\tEGUMin\tEGUMax\tEGU\tRawMin\tRawMax\tRaw\tValue\tStatus\tTimeStamp\n");
		printf("----------------------------------------------------------------------------------------------------------------\n");
		for(int i = 0; i<rtus->at(0).analogInputNum; i++)
		{
			time_t rawtime = rtus->at(0).analogInputs[i].timeStamp;
			struct tm *timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			int iResult = 0;
			if(strlen(rtus->at(0).analogInputs[i].name) >8)
			{
				printf("%s\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%s\t",rtus->at(0).analogInputs[i].name, rtus->at(0).analogInputs[i].address, rtus->at(0).analogInputs[i].EGUMin, 
																  rtus->at(0).analogInputs[i].EGUMax, rtus->at(0).analogInputs[i].EGU, rtus->at(0).analogInputs[i].RawMin,
																  rtus->at(0).analogInputs[i].RawMax, rtus->at(0).analogInputs[i].Raw, rtus->at(0).analogInputs[i].value,
																  rtus->at(0).analogInputs[i].status);
				printf(asctime(timeinfo));
			}
			else
			{
				printf("%s\t\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%s\t",rtus->at(0).analogInputs[i].name, rtus->at(0).analogInputs[i].address, rtus->at(0).analogInputs[i].EGUMin, 
																  rtus->at(0).analogInputs[i].EGUMax, rtus->at(0).analogInputs[i].EGU, rtus->at(0).analogInputs[i].RawMin,
																  rtus->at(0).analogInputs[i].RawMax, rtus->at(0).analogInputs[i].Raw, rtus->at(0).analogInputs[i].value,
																  rtus->at(0).analogInputs[i].status);
				printf(asctime(timeinfo));
			}
		}
		printf("\n");
		printf("================================================================================================================\n");
		printf("DIGITAL DEVICES: \n");
		printf("----------------------------------------------------------------------------------------------------------------\n");
		printf("Name\t\tReadOnly\tInAddress\tOutAddress\tState\tStatus\tCommand\n");
		printf("----------------------------------------------------------------------------------------------------------------\n");
		for(int i = 0; i<rtus->at(0).digitalDevicesNum; i++)
		{
			if(strlen(rtus->at(0).digitalDevices[i].name) >8)
			{
				if(rtus->at(0).digitalDevices[i].outAddress[0] == 0 && rtus->at(0).digitalDevices[i].outAddress[0] == 0)
				{
						printf("%s\t%s\t\t%d|%d\t%d|%d\t\t%d|%d\t%s\t%d|%d\n",rtus->at(0).digitalDevices[i].name, rtus->at(0).digitalDevices[i].readOnly ? "true" : "false", 
						rtus->at(0).digitalDevices[i].inAddress[0], rtus->at(0).digitalDevices[i].inAddress[1], rtus->at(0).digitalDevices[i].outAddress[0],
						rtus->at(0).digitalDevices[i].outAddress[1], rtus->at(0).digitalDevices[i].state[0], rtus->at(0).digitalDevices[i].state[1],
						rtus->at(0).digitalDevices[i].status, rtus->at(0).digitalDevices[i].command[0], rtus->at(0).digitalDevices[i].command[1]);
				}
				else
				{

					printf("%s\t%s\t\t%d|%d\t%d|%d\t%d|%d\t%s\t%d|%d\n",rtus->at(0).digitalDevices[i].name, rtus->at(0).digitalDevices[i].readOnly ? "true" : "false", 
						rtus->at(0).digitalDevices[i].inAddress[0], rtus->at(0).digitalDevices[i].inAddress[1], rtus->at(0).digitalDevices[i].outAddress[0],
						rtus->at(0).digitalDevices[i].outAddress[1], rtus->at(0).digitalDevices[i].state[0], rtus->at(0).digitalDevices[i].state[1],
						rtus->at(0).digitalDevices[i].status, rtus->at(0).digitalDevices[i].command[0], rtus->at(0).digitalDevices[i].command[1]);
				}
			}
			else
			{
				printf("%s\t\t%s\t\t%d|%d\t%d|%d\t%d|%d\t%s\t%d|%d\n",rtus->at(0).digitalDevices[i].name, rtus->at(0).digitalDevices[i].readOnly ? "true" : "false", 
					rtus->at(0).digitalDevices[i].inAddress[0], rtus->at(0).digitalDevices[i].inAddress[1], rtus->at(0).digitalDevices[i].outAddress[0],
					rtus->at(0).digitalDevices[i].outAddress[1], rtus->at(0).digitalDevices[i].state[0],rtus->at(0).digitalDevices[i].state[1],
					rtus->at(0).digitalDevices[i].status, rtus->at(0).digitalDevices[i].command[0], rtus->at(0).digitalDevices[i].command[1]);
			}
		}
		printf("\n");
		printf("----------------------------------------------------------------------------------------------------------------\n");
		printf("Enter number to command:\n");
		printf("1. Enter heat consumption\n");
		printf("2. Turn off the heater\n");
		printf("3. Enter washing machine consumtion\n");
		printf("4. Turn off the washing machine\n");
		printf("5. Enter dish washing consumption\n");
		printf("6. Turn off the dish washer\n");
		printf("7. Enter tv consumption\n");
		printf("8. Turn off the tv\n");
		printf("9. EXIT\n");
		printf("-- Entered number: ");
		ReleaseSemaphore(semaphoreForClient, 1, NULL);
		Sleep(2000);
	}
	return 0;
}

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
	DWORD itRefreshConsole;
	DWORD itCommandFromClient;
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
	semaphoreForClient = CreateSemaphore(0, 1, 1, NULL);
	DWORD itSoundAlarm1;
	DWORD itSoundAlarm2;
	DWORD itSoundAlarm3;
	DWORD itSoundAlarm4;
	DWORD itSoundAlarm5;

	HANDLE alarmSounderAnaIn1 = CreateThread(NULL,0, &SoundAlarm, &rtus[0].analogInputs[0], 0, &itSoundAlarm1);
	HANDLE alarmSounderAnaIn2 = CreateThread(NULL, 0, &SoundAlarm, &rtus[0].analogInputs[1], 0, &itSoundAlarm2);
	HANDLE alarmSounderAnaIn3 = CreateThread(NULL, 0, &SoundAlarm, &rtus[0].analogInputs[2], 0, &itSoundAlarm3);
	HANDLE alarmSounderAnaIn4 = CreateThread(NULL, 0, &SoundAlarm, &rtus[0].analogInputs[3], 0, &itSoundAlarm4);
	HANDLE alarmSounderAnaIn5 = CreateThread(NULL, 0, &SoundAlarm, &rtus[0].analogInputs[4], 0, &itSoundAlarm5);

	HANDLE hPollAllData = CreateThread(NULL, 0, &PollAllData, dfp, 0, &itPollAllData);
	
	T_StructForCommandClient client = { &rtus, &tstruct.vectorOfCommandMessages, cs[1] };
	HANDLE hRefreshConsole = CreateThread(NULL,	0, &RefreshConsole, &rtus, 0, &itRefreshConsole);
	HANDLE hCommandFromClient = CreateThread(NULL, 0, &CommandFromClient, &client, 0, &itCommandFromClient);
	while(true)
	{
		Sleep(100);
		if (shutdownApp)
		{
			break;
		}
	}
	Sleep(1000);
	//getchar();
	closesocket(connectSocket);
	WSACleanup();
	return true;
}
