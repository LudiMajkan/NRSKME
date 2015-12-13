#include "DataProcess.h"

void CalculateEGU(AnalogInput *ai)
{
	ai->value = (((float)ai->EGUMax - (float)ai->EGUMin) / ((float)ai->RawMax - (float)ai->RawMin))*((float)ai->Raw - (float)ai->RawMin) + (float)ai->EGUMin;
	if (ai->value > ai->EGUMax || ai->value < ai->EGUMin)
	{
		printf("ALARM!!!!!");
	}
}