#ifndef _SENSOR_H
#define _SENSOR_H

#include <stdio.h>
#include <stdint.h>

#include "application.h"

#include "adxl362.h"

void SensorTempInit(void);

float SensorTempGetAverage(void);

#endif