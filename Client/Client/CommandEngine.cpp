#include "CommandEngine.h"

typedef struct DataToForwardToThread
{
	T_Message message;
	std::vector<T_Message> *vectorOfMessages;
	HANDLE mutex;
	CRITICAL_SECTION csForCommandVector;
	std::vector<RTU> *rtdb;
} T_DataToForwardToThread;


//Ova metoda je za upis komande u digitalnu tacku (Mora metoda i za analognu koja je ispod)
void WriteCommandToDatabase(std::vector<RTU> *database, short command, short inAddress)
{
	for (int i = 0; i < database->at(0).digitalDevicesNum; i++)
	{
		if (database->at(0).digitalDevices[i].outAddress[0] == inAddress)
		{

			database->at(0).digitalDevices[i].command[0] = command;
		}
		else if (database->at(0).digitalDevices[i].outAddress[1] == inAddress)
		{
			database->at(0).digitalDevices[i].command[1] = command;
		}
	}
}

void WriteSetpointToDatabase(std::vector<RTU> *database, short value, short address)
{
	for (int i = 0; i < database->at(0).analogOutputNum; i++)
	{
		if (database->at(0).analogOutputs[i].address == address)
		{
			database->at(0).analogOutputs[i].EGUSetpoint = value;
		}
	}
}

DWORD WINAPI SendCommand(LPVOID lParam)
{
	T_DataToForwardToThread *data = (T_DataToForwardToThread*)lParam;

	//std::vector<T_Message> *cmdMessages = data->vectorOfMessages;
	data->mutex = CreateSemaphore(0, 0, 1, NULL);
	data->message.response = (char*)malloc(sizeof(char) * 30);
	data->message.handle = data->mutex;
	char funCode = data->message.data[0];
	short inAddress = *(short*)(data->message.data + 1);
	short value = *(short*)(data->message.data + 3);
	EnterCriticalSection(&(data->csForCommandVector));
	data->vectorOfMessages->push_back(data->message);
	LeaveCriticalSection(&(data->csForCommandVector));

	WaitForSingleObject(data->mutex, INFINITE);

	if (data->message.response[7] > 0x80)
	{
		printf("ERROR!!!");
	}

	if (funCode == 0x05)
	{
		WriteCommandToDatabase(data->rtdb, value, inAddress);
	}
	else if (funCode == 0x06)
	{
		WriteSetpointToDatabase(data->rtdb, value, inAddress);
	}
	ReleaseSemaphore(data->mutex, 1, NULL);
	return 0;
}

void SendCommandToSimulator(std::vector<T_Message> *cmdMessages, short protId, short len, char unitId, char fnCode,
	short addr, short val, CRITICAL_SECTION csForCommandVector, std::vector<RTU> *rtuVector)
{
	short transId = getAvailableTransactionID();
	// Prepare data for the message
	T_Header header = { transId, protId, len, unitId };

	char* data = (char*)malloc(sizeof(char) * 5);
	data[0] = fnCode;
	*((short*)(data + 1)) = addr;
	*((short*)(data + 3)) = val;
	T_Message *message = (T_Message*)malloc(sizeof(T_Message));
	message->header = header;
	message->data = data;
	// End of preparing data for the message

	// Prepare message to send to thread
	T_DataToForwardToThread *messageData = (T_DataToForwardToThread*)malloc(sizeof(T_DataToForwardToThread));
	messageData->message = *message;
	messageData->vectorOfMessages = cmdMessages;

	messageData->csForCommandVector = csForCommandVector;
	messageData->rtdb = rtuVector;

	// End of preparing message to send to thread

	DWORD threadId;
	HANDLE tredf1 = CreateThread(NULL, 0, &SendCommand, messageData, 0, &threadId);
}