/*******************************************************
* BAR CLASS
* DESC: The bar or health bar is used to visually display
* the player's health or another status
* Author: Jonathan L Clark
* Date: 5/26/2021
*******************************************************/
#include <stdio.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Bar.h"

/******************************************************************
* BAR DEFAULT CONSTRUCTOR
******************************************************************/
Bar::Bar()
{
   
}

/******************************************************************
* BAR CONSTRUCTOR
******************************************************************/
Bar::Bar(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inContrast, uint16_t inBgColor)
{
   yPos = inYPos;
   xPos = inXPos;
   height = inWidth;
   width = inHeight;
   tft = inTft;
   color = inColor;
   contrast = inContrast;
   tft->fillRect(xPos, yPos, width, height, color);
}

/******************************************************************
* UPDATE
* DESC: Updates the health bar with a new health value
******************************************************************/
void Bar::Update(float newHealth)
{
   int16_t colorWidth = (int16_t)((float)width * newHealth);
   int16_t contrastWidth = width - colorWidth;
   int16_t contrastXStart = xPos + colorWidth;
   tft->fillRect(contrastXStart, yPos, contrastWidth, height, contrast);
}
