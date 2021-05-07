/*******************************************************
* SPRITE CLASS
* DESC: The sprite class handles the behavior of a 2D sprite
* Gameobject. The class is desinged to be inherited and subclasses
* can be used to emulate different behaviors.
* Author: Jonathan L Clark
* Date: 5/2/2021
*******************************************************/
#ifndef _SPRITE_
#define _SPRITE_

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class GameObject
{
   public:
      // Public Functions
      GameObject();
      void Move(int16_t deltaX, int16_t deltaY);
      GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors);
   
   private:
      Adafruit_ILI9341 *tft;
      uint16_t* image;
      int16_t xPos;
      int16_t yPos;
      int16_t width;
      int16_t height;
};

#endif
