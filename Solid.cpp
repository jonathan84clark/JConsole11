/*******************************************************
* GAMEOBJECT CLASS
* DESC: The game object class is designed to provide a framework 
* for creating basic gameobjects using the ILI9341 LCD library
* Author: Jonathan L Clark
* Date: 5/2/2021
*******************************************************/
#include <stdio.h>
#include "SPI.h"
#include "Solid.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Physics.h"
#include "Models.h"

/******************************************************************
* SOLID DEFAULT CONSTRUCTOR
******************************************************************/
Solid::Solid()
{
   prevXStopped = false;
   prevYStopped = false;
   active = false;
   nextPhysicsTime = 0;
   //imageSize = 0;
   rotation = UP;
   collision_cool_down = 0;
}

/******************************************************************
* GAME OBJECT CONSTRUCTOR
******************************************************************/
Solid::Solid(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inBgColor)
{
   xPos = inXPos;
   yPos = inYPos;
   height = inHeight;
   width = inWidth;
   //original = pcolors;
   tft = inTft;
   bg_color = inBgColor;
   color = inColor;
   prevXStopped = false;
   prevYStopped = false;
   rotation = UP;
   nextPhysicsTime = 0;
   active = true;
   tft->fillRect(yPos, xPos, height, width, color);
   //tft->fillCircle(yPos, xPos, 6, color);
   //tft->fillTriangle(yPos, xPos, yPos+20, xPos, yPos + height, xPos / 2.0, color);
}

void Solid::Activate(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inBgColor)
{
   xPos = inXPos;
   yPos = inYPos;
   height = inHeight;
   width = inWidth;
   //original = pcolors;
   tft = inTft;
   bg_color = inBgColor;
   color = inColor;
   prevXStopped = false;
   prevYStopped = false;
   rotation = UP;
   //isSolid = false;
   active = true;
   tft->fillRect(yPos, xPos, height, width, color);
   //tft->fillCircle(yPos, xPos, 6, color);
   //tft->fillTriangle(yPos, xPos, yPos+20, xPos, yPos + height, xPos / 2.0, color);
}

void Solid::SetBehavior(bool inDisableOnHit, int16_t inHealth)
{
    health = inHealth;
    disableOnHit = inDisableOnHit;
}


/******************************************************************
* ROTATE UP
* DESC: Rotates the gameobject to be pointing up on the screen
******************************************************************/
void Solid::RotateUp()
{
    // A left/right rotation to up requires swapping the width and height
   if (rotation == LEFT || rotation == RIGHT)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
   rotation = UP;
   tft->fillRect(yPos, xPos, height, width, color);
}

/******************************************************************
* ROTATE DOWN
* DESC: Rotates the gameobject to be pointing down on the screen
******************************************************************/
void Solid::RotateDown()
{
    // A left/right rotation to up requires swapping the width and height
   if (rotation == LEFT || rotation == RIGHT)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
   rotation = DOWN;
   tft->fillRect(yPos, xPos, height, width, color);
}

/******************************************************************
* ROTATE LEFT
* DESC: Rotates the gameobject left
******************************************************************/
void Solid::RotateLeft()
{
   int16_t imageIndex = 0;
   int16_t readStart = 0;
   //imageIndex = imageSize - 1;
   // A left/right rotation to up requires swapping the width and height
   if (rotation == UP || rotation == DOWN)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
    tft->fillRect(yPos, xPos, height, width, color);
}

/******************************************************************
* ROTATE RIGHT
* DESC: Rotates the gameobject right
******************************************************************/
void Solid::RotateRight()
{
   int16_t imageIndex = 0;
   int16_t readStart = 0;
   if (rotation == UP || rotation == DOWN)
   {
       int16_t temp = width;
       width = height;
       height = temp;
   }
    tft->fillRect(yPos, xPos, height, width, color);
}

/******************************************************************
* CHECK COLLISION
* DESC: Checks the input game object to determine if a collision has
* occured.
******************************************************************/
uint8_t Solid::CheckCollision(Solid* other)
{
  if (collision_cool_down > 0)
  {
     collision_cool_down--;
     return 0x00;
  }

  uint8_t collision = 0x00;
  uint8_t otherCollision = 0x00;
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
          // Newtons 3rd law
          collision |= 0x01;
          otherCollision |= 0x02;
          collision_cool_down = 10;
       }
       // Object collided with the other on its left edge
       if (xPos < otherRightEdge && xPos > other->xPos)
       {
          // Newtons 3rd law
          collision |= 0x01;
          otherCollision |= 0x02;
          collision_cool_down = 10;
       }
   }

   // Handle y direction collisions
   if (xPos < otherRightEdge && myRightEdge > other->xPos)
   {
      // Hitting an object on the bottom edge
      if (yPos < otherTopEdge && yPos > other->yPos)
      {
         // Newtons 3rd law
         collision |= 0x02;
         otherCollision |= 0x01;
         
         collision_cool_down = 10;
      }
      // Hitting an object on the top edge
      if (myTopEdge > other->yPos && myTopEdge < otherTopEdge)
      {
         // Newtons 3rd law
         collision |= 0x02;
         otherCollision |= 0x01;
         
         collision_cool_down = 10;
      }
   }
   //physics.HandleCollision(collision);
   //other->physics.HandleCollision(collision);
   
   return collision;
   //return 0x00;
}

/******************************************************************
* SET VELOCITY
* DESC: Sets the velocity of the input gameobject
******************************************************************/
void Solid::SetVelocity(float inXVelocity, float inYVelocity)
{
   physics.SetVelocity(inXVelocity, inYVelocity);
}

/*********************************************************************
* PHYSICS MOVE
* DESC: Handles moving the gameobject based on ~newtonian physics
*********************************************************************/
void Solid::PhysicsMove(unsigned long milliseconds)
{
    if (!active)
    {
       return; // Inactive objects don't do anything
    }
    int16_t nextPosX = 0;
    int16_t nextPosY = 0;
    physics.Compute(xPos, yPos, &nextPosX, &nextPosY);
    uint8_t collisions = Move(xPos - nextPosX, yPos - nextPosY);
    if (disableOnHit && collisions) // Object hit a wall
    {
        Disable();
        return;
    }
    physics.HandleCollision(collisions);
    nextPhysicsTime = milliseconds + 50;
}

/*********************************************************************
* SET PHYSICS
* DESC: Handles moving the gameobject.
*********************************************************************/
void Solid::SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag)
{
   physics.SetPhysics(inMass, inFriction, inGravity, inBouncy, inDrag);
}

void Solid::Disable()
{
   active = false;
    tft->fillRect(yPos, xPos, height, width, bg_color);
}

/*********************************************************************
* MOVE
* DESC: Handles moving the gameobject.
*********************************************************************/
uint8_t Solid::Move(int16_t deltaX, int16_t deltaY)
{
   int16_t prevX = xPos;
   int16_t prevY = yPos;
   uint8_t prevDeleted = 0;
   uint8_t yMoveStopped = 0;
   uint8_t xMoveStopped = 0;
   uint8_t walls = 0x00;

   xPos += deltaX;
   yPos += deltaY;
   // Pre-calculate boundaries to ensure we don't leave the screen
   if (yPos < 0)
   {
       yMoveStopped = 1;
       yPos = 0;
       walls |= 0x02;
   }
   else if ((yPos + height) > 240)
   {
       yMoveStopped = 2;
       yPos = 240 - height;
       walls |= 0x02;
   }
   else
   {
      prevYStopped = false;
   }

   if (xPos < 0)
   {
      xMoveStopped = 1;
      xPos = 0;
      walls |= 0x01;
   }
   else if ((xPos + width) > 320)
   {
      xMoveStopped = 2;
      xPos = 320 - width;
      walls |= 0x01;
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
      tft->fillRect(prevY, prevX, height, width, bg_color);
   }
   else if (deltaY > 0)
   {
      tft->fillRect(prevY, prevX, abs(deltaY), width, bg_color);
   }
   else
   {
      tft->fillRect((prevY+height+deltaY), prevX, abs(deltaY), width, bg_color);
   }
    tft->fillRect(yPos, xPos, height, width, color);

   return walls;
}
