/*
 * Project lab4-cloud-firmware
 * Description:
 * Author:
 * Date:
 */

SYSTEM_THREAD(ENABLED);

SYSTEM_MODE(SEMI_AUTOMATIC);

#include "include/sensor.h"
#include "include/display-oled.h"
#include "include/serial-monitor.h"
#include "include/net-wifi.h"
#include "include/net-iot.h"

#define BOARD_SYSTEM_LED                D7

#define SYSTEM_HEARTBEAT_TICK_MS        (250)

#define SYSTEM_DISPLAY_UPDATE_MS        (5000)

#define WIFI_AP_NAME    "ececs"
#define WIFI_AP_PASSW   "buckman203"

typedef enum {
    SYS_STATE_INIT,
    SYS_STATE_NET_CONNECT,
    SYS_STATE_NET_PENDING,
    SYS_STATE_NET_READY,
    SYS_STATE_CLOUD_PENDING,
    SYS_STATE_CLOUD_READY
}SystemState_t;

static SystemState_t _systemState = SYS_STATE_INIT;

static bool _ButtonPress = false;

// Temperature sensor Init and timer objects
Timer timerSystemUIupdate(SYSTEM_DISPLAY_UPDATE_MS, SystemUIrefresh);

// Display Average Temperature
void SystemUIrefresh(void)
{
    // Get the average Temperature from the sensor task
    // Update OLED display
    DisplayOLEDupdateTemp(SensorTempGetAverage());

    // refresh/update Cloud data
    NetIoTupdate();
}


// System heartbeat 
// Use software timer, this indicates state of the System
Timer timerSysHeartbeat(SYSTEM_HEARTBEAT_TICK_MS, SystemHeartbeat);

void SystemHeartbeat(void)
{
    static bool firstTime = true;
    static bool ledState = true;

    if (firstTime)
    {
        pinMode(BOARD_SYSTEM_LED, OUTPUT);
        firstTime = false;
        digitalWrite(BOARD_SYSTEM_LED, HIGH);
    }
    else
    {
        if (ledState)
        {
            digitalWrite(BOARD_SYSTEM_LED, LOW);
            ledState = false;
        }
        else
        {
            digitalWrite(BOARD_SYSTEM_LED, HIGH);
            ledState = true;
        }
    }
}

void SystemStateProc(void)
{
    switch(_systemState)
    {
        case SYS_STATE_INIT:
            _systemState = SYS_STATE_NET_CONNECT;
            break;

        case SYS_STATE_NET_CONNECT:
            NetWifiStop();

            // Start WiFi connection
            NetWifiStart(WIFI_AP_NAME, WIFI_AP_PASSW);

            _systemState = SYS_STATE_NET_PENDING;
            break;

        case SYS_STATE_NET_PENDING:
            // Check if we are connected to WiFi AP
            if(NetWifiReady())
            {
                _systemState = SYS_STATE_NET_READY;
            }
            break;

        case SYS_STATE_NET_READY:
            // Connected to WiFi AP
            NetIoTstart();
            SerialMonitorPrint("Connecting to Cloud...");
            _systemState = SYS_STATE_CLOUD_PENDING;
            break;

        case SYS_STATE_CLOUD_PENDING:
            if(NetIoTready())
            {
                _systemState = SYS_STATE_CLOUD_READY;
                SerialMonitorPrint("Cloud ready!");
            }
            break;

        case SYS_STATE_CLOUD_READY:
            if(_ButtonPress)
            {
                NetIoTnotify();

                _ButtonPress = false;
            }
            break;
    }

    // Run the Serial Monitor processor
    // Check for incoming commands from serial port
    SerialMonitorInputProc();
}

// setup() runs once, when the device is first turned on.
void setup()
{
    // Put initialization like pinMode and begin functions here.

    // Start the Serial Monitor 
    SerialMonitorInit(NULL);
    
    // Initialize OLED display
    DisplayOLEDinit();
    
    // Start Sensor task
    SensorTempInit();
    
    // Start UI refresh timer
    // Update the display (OLED) every SYSTEM_DISPLAY_UPDATE_MS
    timerSystemUIupdate.start();
    
    // Initialize Cloud engine
    NetIotInit();

    // Start System heartbeat
    timerSysHeartbeat.start();

    System.on(button_click, button_clicked);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    // The core of your code will likely live here.
    SystemStateProc();
}

void button_clicked(system_event_t event, int param)
{
    int times = system_button_clicks(param);
    Serial.printlnf("button was clicked %d times", times);

    _ButtonPress = true;
}