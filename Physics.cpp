/*******************************************************
* PHYSICS CLASS
* DESC: The physics class handles moving a gameobject in a realistic
* way in the game. It also handles object interactions.
* This class was re-written to work with JConsole11
* Author: Jonathan L Clark
* Date: 5/6/2021
*******************************************************/
#include <stdio.h>
#include "Physics.h"
#include "Arduino.h"

/*********************************************************************
* PHYSICS CONSTRUCTOR
*********************************************************************/
Physics::Physics()
{
   friction = 0.0;
   velocityX = 0.0;
   velocityY = 0.0;
   mass = 0.0;
   bouncy = 0.0;
   gravity = 0.0;
   enabled = false;
}

/*********************************************************************
* PHYSICS (ACTIVE) CONSTRUCTOR
*********************************************************************/
Physics::Physics(float inMass, float inFriction, float inGravity)
{
   mass = inMass;
   friction = inFriction;
   gravity = inGravity;
   bouncy = 0.0;
   enabled = true;
   velocityX = 0.0;
   velocityY = 0.0;
}

/*********************************************************************
* HANDLE COLLISION
* DESC: Handles a physics collision with another object
*********************************************************************/
void Physics::HandleCollision(uint8_t collision)
{
    if (!enabled)
    {
        return;
    }
    if (collision & 0x01)
    {
       velocityX = velocityX * -1.0 * bouncy;
    }
    if (collision & 0x02)
    {
        velocityY = velocityY * -1.0 * bouncy;
    }
}

/*********************************************************************
* PHYSICS COMPUTE
* DESC: Computes the next frame position based on the velocity, friction
* and gravity of the object.
*********************************************************************/
void Physics::Compute(int16_t posX, int16_t posY, int16_t* nextPosX, int16_t* nextPosY)
{
   //if (!enabled)
   //{
   //   *nextPosY = posY;
   //   *nextPosX = posX;
   //   return;
   //}
   //Calculate velocity due to gravity
   velocityY = velocityY + gravity * 0.3;
   velocityX = velocityX - (drag * velocityX); // Drag is proportional to velocity, here we assume it's linear
   velocityY = velocityY - (drag * velocityY);

   *nextPosY = posY + (int16_t)(velocityY * PIXEL_SCALER);
   *nextPosX = posX + (int16_t)(velocityX * PIXEL_SCALER);
}

/*********************************************************************
* SET VELOCITY
*********************************************************************/
void Physics::SetVelocity(float inVelX, float inVelY)
{
   velocityX = inVelX;
   velocityY = inVelY;
}

/*********************************************************************
* SET PHYSICS
*********************************************************************/
void Physics::SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag)
{
   mass = inMass;
   friction = inFriction;
   gravity = inGravity;
   bouncy = inBouncy;
   drag = inDrag;
   enabled = true;
}
