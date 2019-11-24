#include "include/serial-monitor.h"

static char _messageOutBuffer[255];

static char _messageInBuffer[255];

static SerialReadCallback _serialReadCallback = NULL;

void SerialMonitorInit(SerialReadCallback readCallback)
{
    delay(100);

    if(readCallback != NULL)
    {
        _serialReadCallback = readCallback;
    }

    Serial.begin(115200);
}

void SerialMonitorPrint(const char* message, ...)
{
    va_list args;

    va_start(args, message);
    
    memset(_messageOutBuffer, 0, sizeof(_messageOutBuffer));

    vsnprintf(_messageOutBuffer, sizeof(_messageOutBuffer), message, args);

    va_end(args);

    Serial.printf(_messageOutBuffer);

    Serial.println();
}

void SerialMonitorInputProc(void)
{
    int byteOffset = 0;

    while (Serial.available())
    {
        _messageInBuffer[byteOffset] = Serial.read();
        Serial.write(_messageInBuffer[byteOffset]);
        byteOffset++;
    }

    if(byteOffset > 0)
    {
        if(_serialReadCallback != NULL)
        {
            _serialReadCallback((char *)_messageInBuffer, byteOffset);
        }

        byteOffset = 0;
    }
}