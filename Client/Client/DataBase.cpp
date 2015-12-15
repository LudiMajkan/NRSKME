#include "DataBase.h"

std::vector<RTU> InitializeRTUs()
{
	FILE * file;
	char line[256];
	size_t len = 0;
	size_t read;
	file = fopen("../DataBaseConfig.txt", "r");
	int numOfRTUs;
	int numOfAnalogInputs;
	int numOfAnalogOutputs;
	int numOfDigitalDevices;
	/*int numOfDigitalInputs;
	int numOfDigitalOutputs;*/
	std::vector<RTU> RTUs = std::vector<RTU>();
	if (file)
	{
		fgets(line, sizeof(line), file);
		line[strlen(line) - 1] = '\0';
		numOfRTUs = atoi(line);
		for (int currRtu = 0; currRtu < numOfRTUs; currRtu++)
		{
			fgets(line, sizeof(line), file);
			RTU *rtu = new RTU();
			rtu->analogInputNum = 0;
			rtu->analogOutputNum = 0;
			rtu->digitalDevicesNum = 0;
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			rtu->id = atoi(line);
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			rtu->industrialProtocol = atoi(line);
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			rtu->transportProtocol = (char*)malloc(sizeof(char)*strlen(line));
			strcpy(rtu->transportProtocol, line);
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			rtu->ipAddress = (char*)malloc(sizeof(char)*strlen(line));
			strcpy(rtu->ipAddress, line);
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			rtu->port = atoi(line);
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			numOfAnalogInputs = atoi(line);
			//analogInput
			for (int currAnalogInput = 0; currAnalogInput < numOfAnalogInputs; currAnalogInput++)
			{
				fgets(line, sizeof(line), file);
				AnalogInput *ai = new AnalogInput();
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->name = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(ai->name, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->address = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->EGUMin = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->EGUMax = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->EGU = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(ai->EGU, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->RawMin = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->RawMax = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->Raw = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->value = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->status = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(ai->status, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ai->timeStamp = atoi(line);
				rtu->analogInputs.push_back(*ai);
				rtu->analogInputNum++;
			}
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			numOfAnalogOutputs = atoi(line);
			for (int currAnalogOutput = 0; currAnalogOutput < numOfAnalogOutputs; currAnalogOutput++)
			{
				fgets(line, sizeof(line), file);
				AnalogOutput *ao = new AnalogOutput();
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->name = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(ao->name, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->address = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->EGUMin = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->EGUMax = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->EGU = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(ao->EGU, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->RawMin = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->RawMax = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->Raw = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->value = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->status = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(ao->status, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->timeStamp = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->EGUSetpoint = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				ao->setpointTimeout = atoi(line);
				rtu->analogOutputs.push_back(*ao);
				rtu->analogOutputNum++;
			}
			fgets(line, sizeof(line), file);
			line[strlen(line) - 1] = '\0';
			numOfDigitalDevices = atoi(line);
			for (int currDigitalDevices = 0; currDigitalDevices < numOfDigitalDevices; currDigitalDevices++)
			{
				fgets(line, sizeof(line), file);
				DigitalDevice *di = new DigitalDevice();
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->name = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(di->name, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->readOnly = atoi(line);		//PROVERI!
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->inAddress[0] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->inAddress[1] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->outAddress[0] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->outAddress[1] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->state[0] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->state[1] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->status = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(di->status, line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->command[0] = atoi(line);
				fgets(line, sizeof(line), file);
				line[strlen(line) - 1] = '\0';
				di->command[1] = atoi(line);
				rtu->digitalDevices.push_back(*di);
				rtu->digitalDevicesNum++;
			}
			RTUs.push_back(*rtu);
		}
	}
	return RTUs;
}