#pragma once
#include <stdio.h>
#include <vector>
#include <WinSock2.h>
typedef struct AnalogInput {
	char* name;
	short address;
	int EGUMin;
	int EGUMax;
	char *EGU;
	int RawMin;
	int RawMax;
	int Raw;
	int value;
	char* status;
	time_t timeStamp;
	time_t lastDataChanged;
	bool inAlarm;

};

typedef struct AnalogOutput {
	char* name;
	short address;
	int EGUMin;
	int EGUMax;
	char *EGU;
	int RawMin;
	int RawMax;
	int Raw;
	int value;
	char* status;
	time_t timeStamp;
	int EGUSetpoint;
	int setpointTimeout;
};

typedef struct DigitalDevice {
	char* name;
	bool readOnly;
	short inAddress[2];
	short outAddress[2];
	bool state[2];
	char* status;
	bool command[2];
	int commandTimeout;
	bool inAlarm;
};

typedef struct RTU {
	SOCKET socket;
	short id;
	short industrialProtocol;
	char* transportProtocol;
	char* ipAddress;
	int port;
	std::vector<AnalogInput> analogInputs;
	int analogInputNum;
	std::vector<AnalogOutput> analogOutputs;
	int analogOutputNum;
	std::vector<DigitalDevice> digitalDevices;
	int digitalDevicesNum;
};
