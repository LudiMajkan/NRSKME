// ModbusTCPDriver.cpp : Defines the entry point for the console application.
//
#include "ModbusTCPDriver.h"
#include "../CommonLib/SharedFunctions.h"
#include <vector>

CRITICAL_SECTION cs;
CRITICAL_SECTION csForHandleMap;
typedef struct StructForHandleMap
{
	HANDLE handle;
	char *response;
} T_StructForHandleMap;
std::map<short, T_StructForHandleMap> handleMap = std::map<short, T_StructForHandleMap>();


bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

DWORD WINAPI SendDataToModSim(LPVOID lParam)
{
	InitializeCriticalSection(&csForHandleMap);
	T_DataForModBusHandle *dataForModBusHandle = (T_DataForModBusHandle*)lParam;
	while (true)
	{
		if(!dataForModBusHandle->vectorOfCommandMessages.empty())
		{
			EnterCriticalSection(&cs);
			T_Message messageFromCommandVector = dataForModBusHandle->vectorOfCommandMessages[0];
			dataForModBusHandle->vectorOfCommandMessages.erase(dataForModBusHandle->vectorOfCommandMessages.begin());
			LeaveCriticalSection(&cs);
			char functionCode = messageFromCommandVector.data[0];
			short startingAddress = *(short*)(messageFromCommandVector.data + 1);
			switch(functionCode)
			{
				case 5:
					{
						short valueToWrite = *(short*)(messageFromCommandVector.data + 3);
						WriteSingleCoil(messageFromCommandVector.header, FunctionCode::WRITE_SINGLE_COIL, startingAddress, valueToWrite, dataForModBusHandle->connectSocket);
						//T_Message *data = ReceiveResponse(dataForModBusHandle->connectSocket);
						break;
					}

				case 6:
					{
						short valueToWrite = *(short*)(messageFromCommandVector.data + 3);
						WriteSingleRegister(messageFromCommandVector.header, FunctionCode::WRITE_SINGLE_REGISTER, startingAddress, valueToWrite, dataForModBusHandle->connectSocket);
						break;
					}
			}
		}
		if (!dataForModBusHandle->vectorOfPollMessages.empty())
		{
			EnterCriticalSection(&cs);
			T_Message messageFromPollVector = dataForModBusHandle->vectorOfPollMessages[0];
			dataForModBusHandle->vectorOfPollMessages.erase(dataForModBusHandle->vectorOfPollMessages.begin());
			LeaveCriticalSection(&cs);
			char functionCode = messageFromPollVector.data[0];
			short startingAddress = *(short*)(messageFromPollVector.data + 1);
			T_StructForHandleMap helper = {messageFromPollVector.handle, messageFromPollVector.response};
			EnterCriticalSection(&csForHandleMap);
			handleMap[messageFromPollVector.header.transactionIdentifier] = helper;
			LeaveCriticalSection(&csForHandleMap);
			char* messageToSend;
			int sentBytes = 0;
			switch(functionCode)
			{
				case 1:
					{
						short numberOfCoils = *(short*)(messageFromPollVector.data + 3);
						ReadCoils(messageFromPollVector.header, FunctionCode::READ_COILS, startingAddress, numberOfCoils, dataForModBusHandle->connectSocket);
						break;
					}
				case 2:
					{
						short numberOfDiscreteInputs = *(short*)(messageFromPollVector.data + 3);
						ReadCoils(messageFromPollVector.header, FunctionCode::READ_DESCRETE_INPUTS, startingAddress, numberOfDiscreteInputs, dataForModBusHandle->connectSocket);
						break;
					}
				case 3:
					{
						short numberOfHoldingRegisters = *(short*)(messageFromPollVector.data + 3);
						ReadRegisters(messageFromPollVector.header, FunctionCode::READ_HOLDING_REGISTERS, startingAddress, numberOfHoldingRegisters, dataForModBusHandle->connectSocket);
						break;
					}
				case 4:
					{
						short numberOfInputRegisters = *(short*)(messageFromPollVector.data + 3);
						sentBytes = ReadRegisters(messageFromPollVector.header, FunctionCode::READ_INPUT_REGISTERS, startingAddress, numberOfInputRegisters, dataForModBusHandle->connectSocket);
						printf("Bytes sent (read input registers): %d\n", sentBytes);
						break;
					}
			}
		} 
	}

	return 0;
}

DWORD WINAPI ReceiveDataFromModSim(LPVOID lParam)
{
	SOCKET connSock = *(SOCKET*)lParam;
	while (true)
	{
		if(handleMap.size() > 0)
		{
			T_Message *response = ReceiveResponse(connSock);
			EnterCriticalSection(&csForHandleMap);
			HANDLE h = handleMap[response->header.transactionIdentifier].handle;
			LeaveCriticalSection(&csForHandleMap);
			char *chResp = (char*)malloc(sizeof(char)*(6 + response->header.length));
			EnterCriticalSection(&csForHandleMap);
			realloc(handleMap[response->header.transactionIdentifier].response, sizeof(char)*(6 + response->header.length));
			LeaveCriticalSection(&csForHandleMap);
			*(short*)(chResp + 4) = response->header.length;
			*(short*)(chResp) = response->header.transactionIdentifier;
			*(short*)(chResp + 2) = response->header.protocolIdentifier;
			chResp[6] = response->header.unitIdentifier;
			memcpy(chResp + 7, response->data, response->header.length - 1);
			EnterCriticalSection(&csForHandleMap);
			memcpy(handleMap[response->header.transactionIdentifier].response, chResp, response->header.length + 6);
			LeaveCriticalSection(&csForHandleMap);
			ReleaseSemaphore(h, 1, NULL);
			WaitForSingleObject(h, INFINITE);
			EnterCriticalSection(&csForHandleMap);
			handleMap.erase(response->header.transactionIdentifier);
			LeaveCriticalSection(&csForHandleMap);
		}
	}
	return 0;
}

char* PrepareMessage(T_Header header, FunctionCode functionCode, short startingAddress, short data)
{
	char *messageToSend = (char*)malloc(sizeof(char)*12);			//inicijalizacija poruke
	*(short*)messageToSend = htons(header.transactionIdentifier);	//transID
	*((short*)messageToSend + 1) = htons(0);						//Protocol ID
	*((short*)messageToSend + 2) = htons(header.length);			//Length
	messageToSend[6] = header.unitIdentifier;						//Unit ID
	messageToSend[7] = functionCode + 1;							//Function code
	*((short*)messageToSend + 4) = htons(startingAddress);			//Starting Address
	*((short*)messageToSend + 5) = htons(data);						//Data can be: 1. Number of coils or registers to read; 2. Coil or register value to write
	return messageToSend;
}

int ReadCoils(T_Header header, FunctionCode functionCode, short startingAddress, short numberOfCoils, SOCKET connectSocket)
{
	char *messageToSend = PrepareMessage(header, functionCode, startingAddress, numberOfCoils);
	return SendData(12, messageToSend,connectSocket);
}

int ReadRegisters(T_Header header, FunctionCode functionCode, short startingAddress, short numberOfRegisters, SOCKET connectSocket)
{
	char *messageToSend = PrepareMessage(header, functionCode, startingAddress, numberOfRegisters);
	return SendData(12, messageToSend,connectSocket);
}

int WriteSingleCoil(T_Header header, FunctionCode functionCode, short startingAddress, short valueForCoil, SOCKET connectSocket)
{
	char *messageToSend = PrepareMessage(header, functionCode, startingAddress, valueForCoil);
	return SendData(12, messageToSend,connectSocket);
}

int WriteSingleRegister(T_Header header, FunctionCode functionCode, short startingAddress, short valueForRegister, SOCKET connectSocket)
{
	char *messageToSend = PrepareMessage(header, functionCode, startingAddress, valueForRegister);
	return SendData(12, messageToSend,connectSocket);
}

T_Message* ReceiveResponse(SOCKET connectSocket)
{
	char *firstResult = Receive(7, connectSocket);
	int length = ntohs(*((short*)firstResult + 2));
	char *secondResult = Receive(length - 1, connectSocket);
	if(secondResult[0] == 0x81)
	{
		printf("Error reading coil/s\n");
		return NULL;
	}
	else if(secondResult[0] == 0x82)
	{
		printf("Error reading inputs\n");
		return NULL;
	}
	else if(secondResult[0] == 0x83)
	{
		printf("Error reading holding registers\n");
		return NULL;
	}
	else if(secondResult[0] == 0x84)
	{
		printf("Error reading input registers\n");
		return NULL;
	}
	else if(secondResult[0] == 0x85)
	{
		printf("Error writing single coil\n");
		return NULL;
	}
	else if(secondResult[0] == 0x86)
	{
		printf("Error writing single\n");
		return NULL;
	}
	else if(secondResult[0] == 0x87)
	{
		printf("Error reading exception status\n");
		return NULL;
	}
	else if(secondResult[0] == 0x88)
	{
		printf("Error in diagnostic\n");
		return NULL;
	}

	T_Message *response = (T_Message*)malloc(sizeof(T_Message));
	response->header.length = length;
	response->header.transactionIdentifier = ntohs(*(short*)firstResult);
	response->header.protocolIdentifier = ntohs(*((short*)firstResult + 1));
	response->header.unitIdentifier = firstResult[6];
	response->data = secondResult;
	return response;
}

SOCKET ConnectToRTU(unsigned short port, char *ipAddress)
{
	// socket used to communicate with server
	SOCKET connectSocket;
	// variable used to store function return value
	int iResult;
	// create and initialize address structure
	sockaddr_in serverAddress;
	// Buffer used for storing incoming data
	char recvbuf[DEFAULT_BUFLEN];

	//char *ipAddress = "127.0.0.1";
	//unsigned short port = 5001;

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return NULL;
	}

	// create a socket
	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);


	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return NULL;
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
	serverAddress.sin_port = htons(port);
	// connect to server specified in serverAddress and socket connectSocket
	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
		return NULL;
	}
	return connectSocket;
}

short getAvailableTransactionID()
{
	for (int i = 1; i < handleMap.size() + 1; i++)
	{
		if (handleMap.count(i) == 0)
		{
			return i;
		}
	}
	return handleMap.size() + 1;
}

CRITICAL_SECTION InitializeTCPDriver()
{
	InitializeCriticalSection(&cs);
	return cs;
}

