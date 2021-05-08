/*******************************************************
* GAMEOBJECT CLASS
* DESC: The game object class is designed to provide a framework 
* for creating basic gameobjects using the ILI9341 LCD library
* Author: Jonathan L Clark
* Date: 5/2/2021
*******************************************************/
#include <stdio.h>
#include "SPI.h"
#include "GameObject.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Physics.h"

GameObject::GameObject()
{
   prevXStopped = false;
   prevYStopped = false;
}

GameObject::GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors)
{
   xPos = inXPos;
   yPos = inYPos;
   height = inHeight;
   width = inWidth;
   image = pcolors;
   tft = inTft;
   prevXStopped = false;
   prevYStopped = false;
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

void GameObject::SetVelocity(float inXVelocity, float inYVelocity)
{
   physics.SetVelocity(inXVelocity, inYVelocity);
}

/*********************************************************************
* PHYSICS MOVE
* DESC: Handles moving the gameobject based on ~newtonian physics
*********************************************************************/
void GameObject::PhysicsMove()
{
    int16_t nextPosX = 0;
    int16_t nextPosY = 0;
    physics.Compute(xPos, yPos, &nextPosX, &nextPosY);
    Move(xPos - nextPosX, yPos - nextPosY);
}

/*********************************************************************
* SET PHYSICS
* DESC: Handles moving the gameobject.
*********************************************************************/
void GameObject::SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag)
{
   physics.SetPhysics(inMass, inFriction, inGravity, inBouncy, inDrag);
}

/*********************************************************************
* MOVE
* DESC: Handles moving the gameobject.
*********************************************************************/
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
   else
   {
      prevYStopped = false;
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
   else
   {
      prevXStopped = false;
   }
   //Serial.print("Prev: ");
   //Serial.print(prevX);
   //Serial.print(",");
   //Serial.print(prevY);
   //Serial.print(" Current: ");
   //Serial.print(xPos);
   //Serial.print(",");
   //Serial.println(yPos);

   if (xMoveStopped)
   {
      if (!prevXStopped)
      {
         tft->fillRect(prevY, prevX, height, width, ILI9341_BLACK);
         prevXStopped = true;
      }
   }
   // If the object we are moving is greater in width than the amount we are moving we just delete the object
   else if (abs(deltaX) >= width)
   {
       tft->fillRect(prevY, prevX, height, width, ILI9341_BLACK);
       //prevDeleted = 1;
   }
   // Moving right, only delete what we absolutley need to
   else if (deltaX > 0)
   {
       tft->fillRect(prevY, xPos, height, abs(deltaX), ILI9341_BLACK);
   }
   // Moving left, only delete what we absolutley need to
   else
   {
       
       tft->fillRect(prevY, (xPos + width), height, abs(deltaX), ILI9341_BLACK);
   }

   if (yMoveStopped)
   {
      if (!prevYStopped)
      {
         tft->fillRect(prevY, prevX, height, width, ILI9341_BLACK);
         prevYStopped = true;
      }
   }
   // If the object we are moving is greater in height then the amount we are moving we just delete the object
   else if (abs(deltaY) >= height)
   {
       //if (!prevDeleted)
       //{
        tft->fillRect(prevY, prevX, height, width, ILI9341_BLACK);
      // }
   }
   else if (deltaY > 0)
   {
       tft->fillRect(prevY, prevX, abs(deltaY), width, ILI9341_BLACK);
   }
   else
   {
       Serial.println("move down");
       tft->fillRect((prevY+height+deltaY), prevX, abs(deltaY), width, ILI9341_BLACK);
   }

   //if (!(yMoveStopped && xMoveStopped))
   //{
      tft->drawRGBBitmap(yPos, xPos, image, height, width);
   //}
}
