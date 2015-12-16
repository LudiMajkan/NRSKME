#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <vector>
#include "ModbusTCPDriver.h"
#include "DataModel.h"
#include "DataProcess.h"

typedef struct DataForPolling
{
	std::vector<RTU> *rtus;
	std::vector<T_Message> *pollMessages;
	CRITICAL_SECTION cs;
	CRITICAL_SECTION *csForCommandVector;
	std::vector<T_Message> *commandMessages;
}T_DataForPolling;

void PollDataFromRTU(std::vector<T_Message> *pollMessages, short protId, short len, char unitID, char funCode, short addr, short quantity, CRITICAL_SECTION cs);
DWORD WINAPI SendRequest(LPVOID lParam);
DWORD WINAPI PollAllData(LPVOID lParam);