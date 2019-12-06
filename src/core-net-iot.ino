#include "include/net-iot.h"

static double _temp = 0;

static bool _tempInCelsius = true;

static int _NetIoTcloudCmdRecv(String cloudMessage)
{
    int rc = 0;

    //Print Cloud message string to console
    SerialMonitorPrint(cloudMessage.c_str());

    if(cloudMessage == "tempC")
    {
        _tempInCelsius = true;

        DisplayOLEDchangeUnit(true);

        NetIoTupdate();       
    }
    else if(cloudMessage == "tempF")
    {
        _tempInCelsius = false;

        DisplayOLEDchangeUnit(false);

        NetIoTupdate();
    }
    else
    {
        // Error, command not recognized
        rc = 1;
    }

    return rc;
}

void NetIotInit(void)
{
    // Register/Expose Variable
    // Device -> to -> Cloud (polling)
    _temp = SensorTempGetAverage();

    Particle.variable("temp", _temp);

    // Register particle cloud function
    // This will allow cloud control input
    // Cloud -> to -> Device
    Particle.function("sendCmd", _NetIoTcloudCmdRecv);
}

void NetIoTstart(void)
{
    // Start Particle cloud connection
    Particle.connect();
}

bool NetIoTready(void)
{
    // Check status on Cloud connection
    return(Particle.connected());
}

void NetIoTStop(void)
{
    //TO-DO
}

void NetIoTupdate(void)
{
    double _tempInC;

    // Update Cloud variables
    // Update Temperature

    _tempInC = (double)SensorTempGetAverage();

    if(_tempInCelsius)
    {
        _temp = _tempInC;
    }
    else
    {
        // Convert Celsius to Fahrenheit 
        _temp = (_tempInC*(9/5)) + 32;
    }
}

void NetIoTnotify(void)
{
    if(Particle.connected())
    {
        Particle.publish("temperature", "19 F", PUBLIC);
    }
}

