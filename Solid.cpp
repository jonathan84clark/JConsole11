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
   disableOnHit = false;
   active = false;
   lastCollision = NULL;
   nextPhysicsTime = 0;
   rotation = UP;
   collision_cool_down = 0;
}

/******************************************************************
* GAME OBJECT CONSTRUCTOR
******************************************************************/
Solid::Solid(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inBgColor, int objectId)
{
   yPos = inYPos;
   xPos = inXPos;
   height = inHeight;
   width = inWidth;
   tft = inTft;
   bg_color = inBgColor;
   color = inColor;
   collision_cool_down = 0;
   id = objectId;
   prevXStopped = false;
   prevYStopped = false;
   disableOnHit = false;
   lastCollision = NULL;
   rotation = UP;
   nextPhysicsTime = 0;
   active = true;
   tft->fillRect(xPos, yPos, width, height, color);
   //tft->fillCircle(xPos, yPos, 6, color);
   //tft->fillTriangle(xPos, yPos, xPos+20, yPos, xPos + tempwidth, yPos / 2.0, color);
}

void Solid::Activate(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inBgColor, int objectId)
{
   yPos = inYPos;
   xPos = inXPos;
   height = inHeight;
   width = inWidth;
   tft = inTft;
   bg_color = inBgColor;
   color = inColor;
   prevXStopped = false;
   prevYStopped = false;
   disableOnHit = false;
   lastCollision = NULL;
   id = objectId;
   collision_cool_down = 0;
   rotation = UP;
   //isSolid = false;
   active = true;
   tft->fillRect(xPos, yPos, width, height, color);
   //tft->fillCircle(xPos, yPos, 6, color);
   //tft->fillTriangle(xPos, yPos, xPos+20, yPos, xPos + tempwidth, yPos / 2.0, color);
}

void Solid::Draw()
{
   tft->fillRect(xPos, yPos, width, height, color);
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
    // A left/right rotation to up requires swapping the width and tempwidth
   if (rotation == LEFT || rotation == RIGHT)
   {
       int16_t temp = height;
       height = width;
       width = temp;
   }
   rotation = UP;
   tft->fillRect(xPos, yPos, width, height, color);
}

/******************************************************************
* ROTATE DOWN
* DESC: Rotates the gameobject to be pointing down on the screen
******************************************************************/
void Solid::RotateDown()
{
    // A left/right rotation to up requires swapping the width and tempwidth
   if (rotation == LEFT || rotation == RIGHT)
   {
       int16_t temp = height;
       height = width;
       width = temp;
   }
   rotation = DOWN;
   tft->fillRect(xPos, yPos, width, height, color);
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
   // A left/right rotation to up requires swapping the height and width
   if (rotation == UP || rotation == DOWN)
   {
       int16_t temp = height;
       height = width;
       width = temp;
   }
    tft->fillRect(xPos, yPos, width, height, color);
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
       int16_t temp = height;
       height = width;
       width = temp;
   }
    tft->fillRect(xPos, yPos, width, height, color);
}

/******************************************************************
* CHECK COLLISION
* DESC: Checks the input game object to determine if a collision has
* occured.
******************************************************************/
uint8_t Solid::CheckCollision(Solid* other, int16_t* xIntercept, bool ignoreLast)
{
  if (collision_cool_down > 0)
  {
     collision_cool_down--;
     return 0x00;
  }
  if (other == lastCollision && !ignoreLast)
  {
     return 0x00; // The last object we collided with is the object we just collided with
  }
  if (!other->getActive() || !active)
  {
      return 0x00;
  }

  uint8_t collision = 0x00;
  uint8_t otherCollision = 0x00;
  // Calculate all the edges
  int myTopEdge = xPos + width;
  int otherTopEdge = other->xPos + other->width;
  int myRightEdge = yPos + height;
  int otherRightEdge = other->yPos + other->height;
  int otherCenterX = other->xPos + (other->width / 2);

   // Handle x direction collisions
   if (xPos <= otherTopEdge && myTopEdge >= other->xPos)
   {
       // Object collided with the other on its right edge
       if (myRightEdge >= other->yPos && myRightEdge <= otherRightEdge)
       {
          // Newtons 3rd law
          collision |= 0x01;
          otherCollision |= 0x02;
          *xIntercept = otherCenterX - xPos;
          //*xIntercept = otherRightEdge - other->yPos;
          collision_cool_down = 10;
       }
       // Object collided with the other on its left edge
       if (yPos <= otherRightEdge && yPos >= other->yPos)
       {
          // Newtons 3rd law
          collision |= 0x01;
          otherCollision |= 0x02;
          *xIntercept = otherCenterX - xPos;
          //*xIntercept = otherRightEdge - other->yPos;
          collision_cool_down = 10;
       }
   }

   // Handle y direction collisions
   if (yPos <= otherRightEdge && myRightEdge >= other->yPos)
   {
      // Hitting an object on the bottom edge
      if (xPos <= otherTopEdge && xPos >= other->xPos)
      {
         // Newtons 3rd law
         collision |= 0x02;
         otherCollision |= 0x01;
         *xIntercept = otherCenterX - xPos;
         //*xIntercept = myTopEdge - other->xPos;
         collision_cool_down = 10;
      }
      // Hitting an object on the top edge
      if (myTopEdge >= other->xPos && myTopEdge <= otherTopEdge)
      {
         // Newtons 3rd law
         collision |= 0x02;
         otherCollision |= 0x01;
         *xIntercept = otherCenterX - xPos;
         //*xIntercept = myTopEdge - other->xPos;
         collision_cool_down = 10;
      }
      // Figure out where the paddle hit
      
   }
   lastCollision = other;
   
   return collision;
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
  if (other == lastCollision)
  {
     
     return 0x00; // The last object we collided with is the object we just collided with
  }
  if (!other->getActive() || !active)
  {
      return 0x00;
  }

  uint8_t collision = 0x00;
  uint8_t otherCollision = 0x00;
  // Calculate all the edges
  int myTopEdge = xPos + width;
  int otherTopEdge = other->xPos + other->width;
  int myRightEdge = yPos + height;
  int otherRightEdge = other->yPos + other->height;

   // Handle x direction collisions
   if (xPos < otherTopEdge && myTopEdge > other->xPos)
   {
       // Object collided with the other on its right edge
       if (myRightEdge > other->yPos && myRightEdge < otherRightEdge)
       {
          // Newtons 3rd law
          collision |= 0x01;
          otherCollision |= 0x02;
          collision_cool_down = 10;
       }
       // Object collided with the other on its left edge
       if (yPos < otherRightEdge && yPos > other->yPos)
       {
          // Newtons 3rd law
          collision |= 0x01;
          otherCollision |= 0x02;
          collision_cool_down = 10;
       }
   }

   // Handle y direction collisions
   if (yPos < otherRightEdge && myRightEdge > other->yPos)
   {
      // Hitting an object on the bottom edge
      if (xPos < otherTopEdge && xPos > other->xPos)
      {
         // Newtons 3rd law
         collision |= 0x02;
         otherCollision |= 0x01;
         
         collision_cool_down = 10;
      }
      // Hitting an object on the top edge
      if (myTopEdge > other->xPos && myTopEdge < otherTopEdge)
      {
         // Newtons 3rd law
         collision |= 0x02;
         otherCollision |= 0x01;
         
         collision_cool_down = 10;
      }
   }
   lastCollision = other;
   
   return collision;
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
    physics.Compute(yPos, xPos, &nextPosX, &nextPosY);
    uint8_t collisions = Move(yPos - nextPosX, xPos - nextPosY);
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
    tft->fillRect(xPos, yPos, width, height, bg_color);
}

/*********************************************************************
* MOVE
* DESC: Handles moving the gameobject.
*********************************************************************/
uint8_t Solid::Move(int16_t deltaX, int16_t deltaY)
{
   int16_t prevX = yPos;
   int16_t prevY = xPos;
   uint8_t prevDeleted = 0;
   uint8_t yMoveStopped = 0;
   uint8_t xMoveStopped = 0;
   uint8_t walls = 0x00;

   yPos += deltaX;
   xPos += deltaY;
   // Pre-calculate boundaries to ensure we don't leave the screen
   if (xPos < 0)
   {
       yMoveStopped = 1;
       xPos = 0;
       walls |= 0x02;
       lastCollision = NULL; // Clear the last collision if we hit a wall
   }
   else if ((xPos + width) > 320)
   {
       yMoveStopped = 2;
       xPos = 320 - width;
       walls |= 0x02;
       lastCollision = NULL; // Clear the last collision if we hit a wall
   }
   else
   {
      prevYStopped = false;
   }

   if (yPos < 20)
   {
      xMoveStopped = 1;
      yPos = 20;
      walls |= 0x01;
      lastCollision = NULL; // Clear the last collision if we hit a wall
   }
   else if ((yPos + height) > 240)
   {
      xMoveStopped = 2;
      yPos = 240 - height;
      walls |= 0x01;
      lastCollision = NULL; // Clear the last collision if we hit a wall
   }
   else
   {
      prevXStopped = false;
   }

   if (xMoveStopped)
   {
      if (!prevXStopped)
      {
         tft->fillRect(prevY, prevX, width, height, bg_color);
         prevXStopped = true;
      }
   }
   // If the object we are moving is greater in height than the amount we are moving we just delete the object
   else if (abs(deltaX) >= height)
   {
       tft->fillRect(prevY, prevX, width, height, bg_color);
   }
   // Moving right, only delete what we absolutley need to
   else if (deltaX > 0)
   {
       tft->fillRect(prevY, prevX, width, abs(deltaX), bg_color);
   }
   // Moving left, only delete what we absolutley need to
   else
   {
       
       tft->fillRect(prevY, (yPos + height), width, abs(deltaX), bg_color);
   }

   if (yMoveStopped)
   {
      if (!prevYStopped)
      {
         tft->fillRect(prevY, prevX, width, height, bg_color);
         prevYStopped = true;
      }
   }
   // If the object we are moving is greater in width then the amount we are moving we just delete the object
   else if (abs(deltaY) >= width)
   {
      tft->fillRect(prevY, prevX, width, height, bg_color);
   }
   else if (deltaY > 0)
   {
      tft->fillRect(prevY, prevX, abs(deltaY), height, bg_color);
   }
   else
   {
      tft->fillRect((prevY+width+deltaY), prevX, abs(deltaY), height, bg_color);
   }
    tft->fillRect(xPos, yPos, width, height, color);

   return walls;
}
