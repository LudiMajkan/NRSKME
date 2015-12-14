//#pragma pack(1)
#pragma once
#include <stdio.h>
#include <tchar.h>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <vector>
#include <map>

#define DEFAULT_BUFLEN 512

extern CRITICAL_SECTION cs;

typedef enum {
	READ_COILS, 
	READ_DESCRETE_INPUTS, 
	READ_HOLDING_REGISTERS, 
	READ_INPUT_REGISTERS, 
	WRITE_SINGLE_COIL,
	WRITE_SINGLE_REGISTER
} FunctionCode;

typedef struct Header
{
	short transactionIdentifier;
	short protocolIdentifier;
	short length;
	char unitIdentifier;
} T_Header;

typedef struct Message
{
	T_Header header;
	char *data;
	HANDLE handle;
	char *response;
} T_Message;

typedef struct DataForModBusHandle
{
	// A vector of messages to send to RTU
	std::vector<T_Message> vectorOfPollMessages;
	std::vector<T_Message> vectorOfCommandMessages;

	// A socket with connection data to the RTU
	SOCKET connectSocket;

} T_DataForModBusHandle;

SOCKET ConnectToRTU(unsigned short port, char *ipAddress);
bool InitializeWindowsSockets();

DWORD WINAPI SendDataToModSim(LPVOID lParam);
DWORD WINAPI ReceiveDataFromModSim(LPVOID lParam);
char* PrepareMessage(T_Header header, FunctionCode functionCode, short startingAddress, short data);
int ReadCoils(T_Header header, FunctionCode functionCode, short startingAddress, short numberOfCoils, SOCKET connectSocket);
int ReadRegisters(T_Header header, FunctionCode functionCode, short startingAddress, short numberOfRegisters, SOCKET connectSocket);
int WriteSingleCoil(T_Header header, FunctionCode functionCode, short startingAddress, short valueForCoil, SOCKET connectSocket);
int WriteSingleRegister(T_Header header, FunctionCode functionCode, short startingAddress, short valueForRegister, SOCKET connectSocket);
T_Message* ReceiveResponse(SOCKET connectSocket);
char* PrepareMessage(T_Header header, FunctionCode functionCode, short startingAddress, short data);
SOCKET ConnectToRTU(unsigned short port, char *ipAddress);
short getAvailableTransactionID();
CRITICAL_SECTION getCriticalSection();
CRITICAL_SECTION *InitializeTCPDriver();
