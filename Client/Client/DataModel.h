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
	time_t timeStamp;			//timestamp
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
	time_t timeStamp;				//timestamp
	int EGUSetpoint;
	time_t setpointTimeout;			//setpointtimeout
};

typedef struct DigitalDevice {
	char* name;
	bool readOnly;
	short inAddress[2];					//bio char
	short outAddress[2];				//bio char
	bool state[2];						//Pitaj sledeci put
	char* status;						//Pitaj sledeci put
	bool command[2];					//Pitaj
	time_t lastDataChanged;
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
	/*std::vector<DigitalDevice> digitalInputs;
	int digitalInputNum;
	std::vector<DigitalDevice> digitalOutputs;
	int digitalOutputNum;*/
};
