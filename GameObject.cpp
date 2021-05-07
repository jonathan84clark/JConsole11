/*******************************************************
* SPRITE CLASS
* DESC: The sprite class handles the behavior of a 2D sprite
* Gameobject. The class is desinged to be inherited and subclasses
* can be used to emulate different behaviors.
* This class was re-written to work with JConsole11
* Author: Jonathan L Clark
* Date: 5/2/2021
*******************************************************/
#include <stdio.h>
#include "SPI.h"
#include "GameObject.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

GameObject::GameObject()
{

}

GameObject::GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors)
{
   xPos = inXPos;
   yPos = inYPos;
   height = inHeight;
   width = inWidth;
   image = pcolors;
   tft = inTft;
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

void GameObject::Move(int16_t deltaX, int16_t deltaY)
{
   int16_t prevX = xPos;
   int16_t prevY = yPos;
   uint8_t prevDeleted = 0;
   uint8_t yMoveStopped = 0;
   uint8_t xMoveStopped = 0;

   xPos += deltaX;
   yPos += deltaY;
   // Pre-calculate boundaries to ensure we don't leave the screen
   if (yPos < 0)
   {
       yMoveStopped = 1;
       yPos = 0;
   }
   else if ((yPos + height) > 240)
   {
       yMoveStopped = 2;
       yPos = 240 - height;
   }

   if (xPos < 0)
   {
      xMoveStopped = 1;
      xPos = 0;
   }
   else if ((xPos + width) > 320)
   {
      xMoveStopped = 2;
      xPos = 320 - width;
   }

   if (xMoveStopped)
   {
     
   }
   // If the object we are moving is greater in width than the amount we are moving we just delete the object
   else if (abs(deltaX) > width)
   {
       tft->fillRect(yPos, prevX, height, width, ILI9341_BLACK);
       prevDeleted = 1;
   }
   // Moving right, only delete what we absolutley need to
   else if (deltaX > 0)
   {
       tft->fillRect(yPos, prevX, height, abs(deltaX), ILI9341_BLACK);
   }
   // Moving left, only delete what we absolutley need to
   else
   {
       tft->fillRect(yPos, (prevX + width), height, abs(deltaX), ILI9341_BLACK);
   }

   if (yMoveStopped)
   {
    
   }
   // If the object we are moving is greater in height then the amount we are moving we just delete the object
   else if (abs(deltaY) > height)
   {
       if (!prevDeleted)
       {
          tft->fillRect(yPos, prevX, height, width, ILI9341_BLACK);
       }
   }
   else if (deltaY > 0)
   {
       tft->fillRect(prevY, xPos, abs(deltaY), width, ILI9341_BLACK);
   }
   else
   {
       tft->fillRect((prevY + height), xPos, abs(deltaY), width, ILI9341_BLACK);
   }

   if (!(yMoveStopped && xMoveStopped))
   {
      tft->drawRGBBitmap(yPos, xPos, image, height, width);
   }
}
