#include "PollEngine.h"
#include "DataBase.h"


typedef struct DataForSendRequest
{
	T_Message message;
	std::vector<T_Message> *pollMessages;
	HANDLE mutex;
	CRITICAL_SECTION cs;
}T_DataForSendRequest;

std::vector<RTU> *rtuVector;

void PollDataFromRTU(std::vector<T_Message> *pollMessages, short protID, short len, char unitID, char funCode, short addr, short quantity, CRITICAL_SECTION cs)
{
	short transID = getAvailableTransactionID();
	T_Header header = { transID, protID, len, unitID };
	char *data = (char*)malloc(sizeof(char)*5);
	data[0] = funCode;
	*(short*)(data + 1) = addr;
	*(short*)(data + 3) = quantity;
	T_Message *message = (T_Message*)malloc(sizeof(T_Message));
	message->header = header;
	message->data = data;
	T_DataForSendRequest *myData = (T_DataForSendRequest*)malloc(sizeof(T_DataForSendRequest));
	myData->message = *message;
	myData->pollMessages = pollMessages;
	myData->cs = cs;
	DWORD itSendRequest;
	HANDLE sendData = CreateThread(NULL, 0, &SendRequest, myData, 0, &itSendRequest);

}

DWORD WINAPI SendRequest(LPVOID lParam)
{
	T_DataForSendRequest *myData = (T_DataForSendRequest*)lParam;
	myData->mutex = CreateSemaphore(0,0,1,NULL);
	myData->message.handle = myData->mutex;
	myData->message.response = (char*)malloc(sizeof(char)*30);
	EnterCriticalSection(&(myData->cs));
	myData->pollMessages->push_back(myData->message);
	LeaveCriticalSection(&(myData->cs));
	WaitForSingleObject(myData->mutex, INFINITE);
	//Data nije nthos
	if (*(char*)(myData->message.response + 7) == 0x04)
	{
		for (int i = 0; i < (*rtuVector)[0].analogInputs.size(); i++)
		{
			short value = ntohs(*(short*)(myData->message.response + 9 + i * 2));
			printf("Value: %d\n", value);
			int bla = rtuVector->at(0).analogInputs[i].Raw;
			rtuVector->at(0).analogInputs[i].Raw = value;
			CalculateEGU(&rtuVector->at(0).analogInputs[i]);
		}
	}
	else if (*(char*)(myData->message.response + 7) == 0x02)
	{
		char mask = 1;
		char responseCh = *(char*)(myData->message.response + 9);
		short numOfBytes = *(char*)(myData->message.response + 8);
		for (int i = 1; i < numOfBytes + 1; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					if (j + (i - 1) * 4 < rtuVector->at(0).digitalDevices.size())
					{
						rtuVector->at(0).digitalDevices[j + (i - 1) * 4].state[k] = responseCh & mask;
						mask = mask << 1;
					}
					else
					{
						i = numOfBytes + 1;
						j = 4;
						break;
					}
				}
			}
			if (i < numOfBytes + 1)
			{
				responseCh = *(char*)(myData->message.response + 9 + i);
				mask = 1;
			}
		}
		printf("");
	}
	ReleaseSemaphore(myData->mutex, 1, NULL);
	return 0;
}

DWORD WINAPI PollAllData(LPVOID lParam)
{
	T_DataForPolling *myData = (T_DataForPolling*)lParam;
	rtuVector = myData->rtus;
	while(true)
	{
		for(int i = 0; i < myData->rtus->size(); i++)
		{
			RTU rtu = (*myData->rtus)[i];
			PollDataFromRTU(myData->pollMessages,rtu.industrialProtocol, 6, rtu.id, 0x04, rtu.analogInputs[0].address, 5, myData->cs);
			//DataProcessing za AnalogInput-e
			PollDataFromRTU(myData->pollMessages, rtu.industrialProtocol, 6, rtu.id, 0x02, rtu.digitalDevices[0].inAddress[0], 10, myData->cs);
			//DataProcessing za DigitalDevice-ove
		}
		Sleep(2000);
	}
}
