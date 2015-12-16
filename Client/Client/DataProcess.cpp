#include "DataProcess.h"

bool CalculateEGU(AnalogInput *ai)
{
	ai->value = (((float)ai->EGUMax - (float)ai->EGUMin) / ((float)ai->RawMax - (float)ai->RawMin))*((float)ai->Raw - (float)ai->RawMin) + (float)ai->EGUMin + 0.5;
	if(ai->RawMax < ai->Raw || ai->RawMin > ai->Raw)
	{
		return true;
		//printf("\nRAW ALARM!!!!!\n");
	}
	
	else if (ai->value > ai->EGUMax || ai->value < ai->EGUMin)
	{
		return true;
		//printf("EGU ALARM!!!!!\n");
	}
	else
	{
		return false;
	}
}

void AnalizeDigitalDevice(DigitalDevice *dd)							//10 otvoreno, 01 zatvoreno, 00 tranzient, 11 error
{
	if(dd->state[0] == true && dd->state[1] == false)
	{
		printf("Error alarm");
	}
	else if(dd->state[0] == true && dd->state[1] == true)
	{
		printf("Tranzient alarm");
	}
}

void AnalizeCommandTimeout(int i, RTU *rtu)
{
	if(rtu->digitalDevices[i].command[0] != rtu->digitalDevices[i].state[0] || rtu->digitalDevices[i].command[1] != rtu->digitalDevices[i].state[1])
	{
		if(rtu->digitalDevices[i].commandTimeout == 0)
		{
			rtu->digitalDevices[i].inAlarm = true;
		}
	}
	else
	{
		rtu->digitalDevices[i].inAlarm = false;
	}
}

void AnalizeSetpointTimeout(int i, RTU *rtu)
{
	bool isAlarmLimits = CalculateEGU(&(rtu->analogInputs[i]));
	if(rtu->analogInputs[i].value != rtu->analogOutputs[i].EGUSetpoint)
	{
		if(rtu->analogOutputs[i].setpointTimeout == 0)
		{
			rtu->analogInputs[i].inAlarm = true;
			return;
		}
	}
	else if(isAlarmLimits)
	{
		rtu->analogInputs[i].inAlarm = true;
		return;
	}
	else
	{
		rtu->analogInputs[i].inAlarm = false;
		return;
	}
}