#include <stdio.h>
#include "DataModel.h"

bool CalculateEGU(AnalogInput *ai);
void AnalizeDigitalDevice(DigitalDevice *dd);
void AnalizeSetpointTimeout(int i, RTU *rtu);
void AnalizeCommandTimeout(int i, RTU *rtu);