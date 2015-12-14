#include "DataProcess.h"

void CalculateEGU(AnalogInput *ai)
{
	if(ai->RawMax < ai->Raw || ai->RawMin > ai->Raw)
	{
		printf("RAW ALARM!!!!!");
	}
	ai->value = (((float)ai->EGUMax - (float)ai->EGUMin) / ((float)ai->RawMax - (float)ai->RawMin))*((float)ai->Raw - (float)ai->RawMin) + (float)ai->EGUMin;
	if (ai->value > ai->EGUMax || ai->value < ai->EGUMin)
	{
		printf("ALARM!!!!!");
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