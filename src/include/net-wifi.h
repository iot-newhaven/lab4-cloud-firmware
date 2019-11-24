#ifndef _NET_WIFI_H
#define _NET_WIFI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "application.h"

#include "include/serial-monitor.h"

void NetWifiStart(const char *wifiAPname,
                  const char *password);
                  
void NetWifiStop(void);

bool NetWifiReady(void);

#endif