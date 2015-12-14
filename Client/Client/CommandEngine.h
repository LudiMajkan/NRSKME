#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <vector>
#include "ModbusTCPDriver.h"
#include "DataModel.h"

void SendCommandToSimulator(std::vector<T_Message> *cmdMessages, short protId, short len, char unitId,
	char fnCode, short addr, short val, CRITICAL_SECTION csForCommandVector, std::vector<RTU> *rtuVector);