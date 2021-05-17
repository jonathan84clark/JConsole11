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
#include "Models.h"

/******************************************************************
* GAME OBJECT DEFAULT CONSTRUCTOR
******************************************************************/
GameObject::GameObject()
{
   prevXStopped = false;
   prevYStopped = false;
   active = false;
   imageSize = 0;
   rotation = UP;
   isSolid = false;
}

/******************************************************************
* GAME OBJECT DESTRUCTOR
******************************************************************/
GameObject::~GameObject()
{
   delete [] image;
}

/******************************************************************
* GAME OBJECT CONSTRUCTOR
******************************************************************/
GameObject::GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inBgColor, uint16_t solidColor)
{
   ActivateSolid(inTft, inXPos, inYPos, inWidth, inHeight, inBgColor, solidColor);
}

void GameObject::ActivateSolid(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inBgColor, uint16_t solidColor)
{
   if (active)
   {
      //Serial.println("Call0");
      //tft->fillRect(yPos, xPos, height, width, bg_color);
      //delete[] image;
   }
   xPos = inXPos;
   yPos = inYPos;
   height = inHeight;
   width = inWidth;
   //original = pcolors;
   tft = inTft;
   bg_color = inBgColor;
   prevXStopped = false;
   prevYStopped = false;
   rotation = UP;
   imageSize = width * height;
   image = new uint16_t[imageSize];
   isSolid = true;
   active = true;
   for (int i = 0; i < imageSize; i++)
   {
      image[i] = solidColor;
   }
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

/******************************************************************
* GAME OBJECT CONSTRUCTOR
******************************************************************/
GameObject::GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors, uint16_t inBgColor)
{
   xPos = inXPos;
   yPos = inYPos;
   height = inHeight;
   width = inWidth;
   original = pcolors;
   tft = inTft;
   bg_color = inBgColor;
   prevXStopped = false;
   prevYStopped = false;
   rotation = UP;
   isSolid = false;
   active = true;
   imageSize = width * height;
   image = new uint16_t[imageSize];
   for (int i = 0; i < imageSize; i++)
   {
      if (original[i] == COLOR_WHITE)
      {
         image[i] = bg_color;
      }
      else
      {
         image[i] = original[i];
      }
   }
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}


/******************************************************************
* ROTATE UP
* DESC: Rotates the gameobject to be pointing up on the screen
******************************************************************/
void GameObject::RotateUp()
{
    // A left/right rotation to up requires swapping the width and height
   if (rotation == LEFT || rotation == RIGHT)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
   // Solid objects don't need pixels copied
   if (!isSolid)
   {
      for (int i = 0; i < imageSize; i++)
      {
         if (original[i] == COLOR_WHITE)
         {
            image[i] = bg_color;
         }
         else
         {
            image[i] = original[i];
         }
      }
   }
   rotation = UP;
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

/******************************************************************
* ROTATE DOWN
* DESC: Rotates the gameobject to be pointing down on the screen
******************************************************************/
void GameObject::RotateDown()
{
    // A left/right rotation to up requires swapping the width and height
   if (rotation == LEFT || rotation == RIGHT)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
   if (!isSolid)
   {
      int16_t readStart = imageSize - 1;
      int16_t index = 0;
      // 180 degree flip
      for (int i = 0; i < width; i++)
      {
         for (int j = 0; j < height; j++)
         {
            if (original[readStart] == COLOR_WHITE)
            {
               image[index] = bg_color;
            }
            else
            {
               image[index] = original[readStart];
            }
            index++;
            readStart--;
         }
      }
   }
   rotation = DOWN;
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

/******************************************************************
* ROTATE LEFT
* DESC: Rotates the gameobject left
******************************************************************/
void GameObject::RotateLeft()
{
   int16_t imageIndex = 0;
   int16_t readStart = 0;
   imageIndex = imageSize - 1;
   // A left/right rotation to up requires swapping the width and height
   if (rotation == UP || rotation == DOWN)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
   if (!isSolid)
   {
      for (int i = 0; i < width; i++)
      {
         readStart = i;
         for (int j = 0; j < height; j++)
         {
            if (original[readStart] == COLOR_WHITE)
            {
               image[imageIndex] = bg_color;
            }
            else
            {
               image[imageIndex] = original[readStart];
            }
            imageIndex--;
            readStart += width;
         }
      }
   }
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

/******************************************************************
* ROTATE RIGHT
* DESC: Rotates the gameobject right
******************************************************************/
void GameObject::RotateRight()
{
   int16_t imageIndex = 0;
   int16_t readStart = 0;
   if (rotation == UP || rotation == DOWN)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
   if (!isSolid)
   {
      for (int i = 0; i < width; i++)
      {
         readStart = i;
         for (int j = 0; j < height; j++)
         {
            if (original[readStart] == COLOR_WHITE)
            {
               image[imageIndex] = bg_color;
            }
            else
            {
               image[imageIndex] = original[readStart];
            }
            imageIndex++;
            readStart += width;
         }
      }
   }
   tft->drawRGBBitmap(yPos, xPos, image, height, width);
}

/******************************************************************
* CHECK COLLISION
* DESC: Checks the input game object to determine if a collision has
* occured.
******************************************************************/
uint8_t GameObject::CheckCollision(GameObject* other)
{
  uint8_t collision = 0x00;
  // Calculate all the edges
  int myTopEdge = yPos + height;
  int otherTopEdge = other->yPos + other->height;
  int myRightEdge = xPos + width;
  int otherRightEdge = other->xPos + other->width;
  
   // Handle x direction collisions
   if (yPos < otherTopEdge && myTopEdge > other->yPos)
   {
       // Object collided with the other on its right edge
       if (myRightEdge > other->xPos && myRightEdge < otherRightEdge)
       {
          //velocity.x = velocity.x * -1.0 * bounciness;
          collision |= 0x01;
       }
       // Object collided with the other on its left edge
       if (xPos < otherRightEdge && xPos > other->xPos)
       {
           //velocity.x = velocity.x * -1.0 * bounciness;
          collision |= 0x01;
       }
   }

   // Handle y direction collisions
   //if (framesSinceYCollision > 4 && xPos < otherRightEdge && myRightEdge > other->xPos)
   if (xPos < otherRightEdge && myRightEdge > other->xPos)
   {
      // Hitting an object on the bottom edge
      if (yPos < otherTopEdge && yPos > other->yPos)
      {
         //velocity.y = velocity.y * -1.0 * bounciness;
         collision |= 0x02;
      }
      // Hitting an object on the top edge
      if (myTopEdge > other->yPos && myTopEdge < otherTopEdge)
      {
         //velocity.y = velocity.y * -1.0 * bounciness;
         collision |= 0x02;
      }
   }
   physics.HandleCollision(collision);
   
   return collision;
}

/******************************************************************
* SET VELOCITY
* DESC: Sets the velocity of the input gameobject
******************************************************************/
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
    if (!active)
    {
       return; // Inactive objects don't do anything
    }
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

void GameObject::Disable()
{
   active = false;
    tft->fillRect(yPos, xPos, height, width, bg_color);
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

   if ((deltaX + deltaY) == 0)
   {
      //Serial.println("Call");
      //return;
   }

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

   if (xMoveStopped)
   {
      if (!prevXStopped)
      {
         tft->fillRect(prevY, prevX, height, width, bg_color);
         prevXStopped = true;
      }
   }
   // If the object we are moving is greater in width than the amount we are moving we just delete the object
   else if (abs(deltaX) >= width)
   {
       tft->fillRect(prevY, prevX, height, width, bg_color);
       //prevDeleted = 1;
   }
   // Moving right, only delete what we absolutley need to
   else if (deltaX > 0)
   {
       tft->fillRect(prevY, prevX, height, abs(deltaX), bg_color);
   }
   // Moving left, only delete what we absolutley need to
   else
   {
       
       tft->fillRect(prevY, (xPos + width), height, abs(deltaX), bg_color);
   }

   if (yMoveStopped)
   {
      if (!prevYStopped)
      {
         tft->fillRect(prevY, prevX, height, width, bg_color);
         prevYStopped = true;
      }
   }
   // If the object we are moving is greater in height then the amount we are moving we just delete the object
   else if (abs(deltaY) >= height)
   {
       //if (!prevDeleted)
       //{
        tft->fillRect(prevY, prevX, height, width, bg_color);
      // }
   }
   else if (deltaY > 0)
   {
       tft->fillRect(prevY, prevX, abs(deltaY), width, bg_color);
   }
   else
   {
       tft->fillRect((prevY+height+deltaY), prevX, abs(deltaY), width, bg_color);
   }

   //if (!(yMoveStopped && xMoveStopped))
   //{
      tft->drawRGBBitmap(yPos, xPos, image, height, width);
   //}
}
