/*******************************************************
* BAR CLASS
* DESC: The bar or health bar is used to visually display
* the player's health or another status
* Author: Jonathan L Clark
* Date: 5/26/2021
*******************************************************/
#ifndef _BAR_
#define _BAR_

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class Bar
{
   public:
      // Public Functions
      Bar();
      Bar(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inContrast, uint16_t inBgColor);
      void Update(float newHealth);
      int16_t getHeight() { return height; }
      int16_t getWidth() { return width; }
      int16_t getXPos() { return xPos; }
      int16_t getYPos() { return yPos; }
   
   private:
      Adafruit_ILI9341 *tft;
      int16_t xPos;
      int16_t yPos;
      int16_t width;
      int16_t height;
      uint16_t color;
      uint16_t contrast;
};

#endif
