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
#include "Bar.h"
#include "Menu.h"
#include "Config.h"
#include "BrickBreaker.h"
#include "Debris.h"
#include "Tanks.h"

/******************************************************************
* SOLID DEFAULT CONSTRUCTOR
******************************************************************/
Menu::Menu(Adafruit_ILI9341 *inTft, uint16_t inBg_color)
{
   tft = inTft;
   bg_color = inBg_color;
}

void Menu::DrawMenu(char* title,char options[][15], int numOptions, int selectedOption)
{
   int yPos = 80;
   int16_t yPosition = 82 + selectedOption * 30;
   tft->fillScreen(bg_color);
   tft->setTextColor(ILI9341_BLUE);
   tft->setTextSize(3);  
   tft->setCursor(80, 20);
   tft->print(title);
   tft->setTextSize(2);

   for (int i = 0; i < numOptions; i++)
   {
      tft->setCursor(100, yPos);
      //tft->setTextSize(2);
      tft->print(options[i]);
      yPos += 30;
   }
   tft->fillRect(80, yPosition, 10, 10, COLOR_RED);
}

int Menu::MultiOption(char* title, char options[][15], int numOptions)
{
   unsigned long msTicks = 0;
   unsigned long selectDebounce = 0;
   unsigned long nextJoystickTime = 0;
   int selectedOption = 0;
   DrawMenu(title, options, numOptions, selectedOption);
   bool paused = true;
   

   while (paused)
   {
       msTicks = millis();
       // Fire button
       if (selectDebounce < msTicks && digitalRead(TOP_LEFT) == 0)
       {
          // Brick Breaker selected
          if (selectedOption == 0)
          {
              Debris(tft);
              DrawMenu(title, options, numOptions, selectedOption);
          }
          // Brick Breaker selected
          else if (selectedOption == 1)
          {
              BrickBreaker(tft);
              DrawMenu(title, options, numOptions, selectedOption);
          }
          // Tanks option selected
          else if (selectedOption == 2)
          {
              Tanks(tft);
              DrawMenu(title, options, numOptions, selectedOption);
          }
          selectDebounce = msTicks + 200;
       }
       if (nextJoystickTime < msTicks)
       {
          float yScaler = (float)analogRead(JOYSTICK_Y) - (float)(ADC_MAX / 2);
          yScaler = yScaler / (float)(ADC_MAX / 2);
          if (yScaler > 0.2)
          {
              int16_t yPosition = 82 + selectedOption * 30;
              tft->fillRect(80, yPosition, 10, 10, bg_color);
              selectedOption++;
              if (selectedOption == numOptions)
              {
                  selectedOption = 0;
              }
              yPosition = 82 + selectedOption * 30;
              tft->fillRect(80, yPosition, 10, 10, COLOR_RED);
              nextJoystickTime = msTicks + 200;
          }
          if (yScaler < -0.2)
          {
              int16_t yPosition = 82 + selectedOption * 30;
              tft->fillRect(80, yPosition, 10, 10, bg_color);
              selectedOption--;
              if (selectedOption < 0)
              {
                  selectedOption = 2;
              }
              yPosition = 82 + selectedOption * 30;
              tft->fillRect(80, yPosition, 10, 10, COLOR_RED);
              nextJoystickTime = msTicks + 200;
          }
          
       }
   }
   tft->fillRect(80, 10, 150, 200, bg_color);

   return selectedOption;
}

/******************************************************************
* MENU PAUSE
* DESC: Handles pausing the game.
******************************************************************/
int Menu::Pause()
{
   unsigned long msTicks = 0;
   unsigned long selectDebounce = 0;
   unsigned long nextJoystickTime = 0;
   int16_t yPosition = 102;
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
       if (nextJoystickTime < msTicks)
       {
          float yScaler = (float)analogRead(JOYSTICK_Y) - (float)(ADC_MAX / 2);
          yScaler = yScaler / (float)(ADC_MAX / 2);
          if (yScaler > 0.2)
          {
              tft->fillRect(80, yPosition, 10, 10, bg_color);
              selectedOption++;
              if (selectedOption == 3)
              {
                  selectedOption = 0;
                  yPosition = 102;
              }
              else
              {
                 yPosition += 30;
              }
              tft->fillRect(80, yPosition, 10, 10, COLOR_RED);
              nextJoystickTime = msTicks + 200;
          }
          if (yScaler < -0.2)
          {
              tft->fillRect(80, yPosition, 10, 10, bg_color);
              selectedOption--;
              if (selectedOption < 0)
              {
                  selectedOption = 2;
                  yPosition = 162;
              }
              else
              {
                 yPosition -= 30;
              }
              tft->fillRect(80, yPosition, 10, 10, COLOR_RED);
              nextJoystickTime = msTicks + 200;
          }
          
       }

    
   }
   tft->fillRect(80, 10, 150, 200, bg_color);

   return selectedOption;

  
}
