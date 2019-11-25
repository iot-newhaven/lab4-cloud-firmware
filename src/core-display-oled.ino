#include "include/display-oled.h"

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(MODE_I2C, D8, 1); // I2C declaration

static bool _tempInCelsius = true;

void DisplayOLEDinit(void)
{
    oled.begin();         // Initialize the OLED

    oled.clear(PAGE);     // Clear the screen

    oled.setFontType(1);  // Set font to type 0

    oled.setCursor(0, 0); // Set cursor to top-left

    oled.print("Hello!");

    oled.display();

    delay(2000);
}

void DisplayOLEDupdateTemp(float tempCelsius)
{
    static char tempBuffer[32];
    float tempFahrenheit;
    int msgPixelWidth;
    int msgPixelHeight;
    uint8_t xPos;
    uint8_t yPos;
    
    oled.clear(PAGE);     // Clear the screen
    
    oled.setFontType(1);  // Set font to type 0

    msgPixelHeight = oled.getFontHeight();

    memset(tempBuffer, 0, sizeof(tempBuffer));

    if(_tempInCelsius)
    {
        snprintf(tempBuffer, sizeof(tempBuffer), "%.1f C", tempCelsius);
    }
    else
    {
        tempFahrenheit = (tempCelsius*(9/5)) + 32;

        snprintf(tempBuffer, sizeof(tempBuffer), "%.1f F", tempFahrenheit);
    }

    msgPixelWidth = strlen(tempBuffer)*oled.getFontWidth();

    yPos = (oled.getLCDHeight() - msgPixelHeight)/2;

    xPos = (oled.getLCDWidth() - msgPixelWidth)/2;
    
    oled.setCursor(xPos, yPos); // Set cursor to top-left
    
    oled.print(tempBuffer);

    oled.display();
}

void DisplayOLEDchangeUnit(bool displayInCelsius)
{
    _tempInCelsius = displayInCelsius;
}


