#ifndef _SERIAL_MON_H
#define _SERIAL_MON_H

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "application.h"

typedef void (*SerialReadCallback)(char *message, int len);

void SerialMonitorInit(SerialReadCallback readCallback);

void SerialMonitorInputProc(void);

void SerialMonitorPrint(const char* message, ...);

#endif