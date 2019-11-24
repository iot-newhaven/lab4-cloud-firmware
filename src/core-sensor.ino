#include "include/sensor.h"

#define SENSOR_SAMPLE_RATE_MS           (100)

#define SENSOR_TEMP_LOG_SAMPLES         (100)

int16_t XValue, YValue, ZValue, Temperature;

float tempAvgCelsius = 0;

float sensorTempLog[SENSOR_TEMP_LOG_SAMPLES];

bool sensorLogFirsTime = true;

int sensorLogOffset = 0;

float sensorTempAvg = 0;

ADXL362 sensor;

static void _sensorTempGetRawVal(void)
{
    // read all three axis in burst to ensure all measurements correspond to same sample time
    //sensor.readXYZTData(XValue, YValue, ZValue, Temperature);
    Temperature = sensor.readTemp();
    //Serial.println(Temperature);
}

static float _sensorTempGetCelsius(void)
{
    float tempCelsius = 0;

    tempCelsius = (float)Temperature * 0.065;

    return tempCelsius;
}

// Reads sample from the sensor
static void _sensorTempGetSample(void)
{
    int i;

    if(sensorLogFirsTime)
    {
        for(i = 0 ; i < SENSOR_TEMP_LOG_SAMPLES ; i++)
        {
            sensorTempLog[i] = 0.0;
        }

        sensorLogFirsTime = false;

        sensorLogOffset = 0;
    }

    // Get raw data values from sensor
    _sensorTempGetRawVal();

    // Convert raw value to celsius
    // record sample
    sensorTempLog[sensorLogOffset] = _sensorTempGetCelsius();

    sensorLogOffset++;

    if(sensorLogOffset > SENSOR_TEMP_LOG_SAMPLES)
    {
        sensorLogOffset = 0;
    }

    // Calculate running Average
    sensorTempAvg = 0;

    for (i = 0; i < SENSOR_TEMP_LOG_SAMPLES; i++)
    {
        sensorTempAvg += sensorTempLog[i];
    }

    sensorTempAvg = (sensorTempAvg/SENSOR_TEMP_LOG_SAMPLES);
}

Timer timerSensorTempSample(SENSOR_SAMPLE_RATE_MS, _sensorTempGetSample);

void SensorTempInit(void)
{
    sensor.begin(SS);      // Setup SPI protocol, issue device soft reset
    sensor.beginMeasure(); // Switch ADXL362 to measure mode

    // Start Temperature sample timer
    timerSensorTempSample.start();
}

float SensorTempGetAverage(void)
{
    return(sensorTempAvg);
}