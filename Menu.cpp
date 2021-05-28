/*******************************************************
* MENU CLASS
* DESC: The menu class handles menu interactions on the UI
* Author: Jonathan L Clark
* Date: 5/27/2021
*******************************************************/
#include <stdio.h>
#include "Menu.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Config.h"
#include "Models.h"

/******************************************************************
* SOLID DEFAULT CONSTRUCTOR
******************************************************************/
Menu::Menu(Adafruit_ILI9341 *inTft)
{
   tft = inTft;
}

/******************************************************************
* MENU PAUSE
* DESC: Handles pausing the game.
******************************************************************/
uint8_t Menu::Pause()
{
   unsigned long msTicks = 0;
   unsigned long selectDebounce = 0;
   unsigned long nextJoystickTime = 0;
   int16_t yPosition = 100;
   int selectedOption = 0;
   tft->setTextColor(ILI9341_BLUE);
   tft->setTextSize(3);  
   tft->setCursor(100, 50);
   tft->print("Paused");
   tft->setCursor(100, 100);
   tft->setTextSize(2);
   tft->print("Resume");
   tft->setCursor(100, 130);
   tft->print("High Scores");
   tft->setCursor(100, 160);
   tft->print("Exit");
   tft->fillRect(80, yPosition, 10, 10, COLOR_RED);
   bool paused = true;
   

   while (paused)
   {
       msTicks = millis();
       // Fire button
       if (selectDebounce < msTicks && digitalRead(TOP_LEFT) == 0)
       {
          paused = false;
          selectDebounce = msTicks + 200;
       }
       // Joystick/ Menu button handler
       if (digitalRead(JOYSTICK_BTN))
       {
           //paused = false;
           //debounceTimes[0] = msTicks + 200; // Right now 200ms debounce time
       }
       if (nextJoystickTime < msTicks)
       {
          float xScaler = (float)analogRead(JOYSTICK_X) - (float)(ADC_MAX / 2);
          xScaler = xScaler / (float)(ADC_MAX / 2);
          float yScaler = (float)analogRead(JOYSTICK_Y) - (float)(ADC_MAX / 2);
          yScaler = yScaler / (float)(ADC_MAX / 2);
          if (yScaler < -0.2)
          {
              selectedOption--;
          }
          nextJoystickTime = msTicks + 200;
       }

    
   }


   return 0;

  
}
