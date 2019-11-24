#ifndef _OLED_DISPLAY_H
#define _OLED_DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "application.h"

#include "SparkFunMicroOLED.h" // Include MicroOLED library

void DisplayOLEDinit(void);

void DisplayOLEDupdateTemp(float tempCelsius);

void DisplayOLEDchangeUnit(bool displayInCelsius);

#endif