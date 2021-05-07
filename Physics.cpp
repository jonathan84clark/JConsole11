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
* PHYSICS COMPUTE
* DESC: Computes the next frame position based on the velocity, friction
* and gravity of the object.
*********************************************************************/
void Physics::Compute(int16_t posX, int16_t posY, int16_t* nextPosX, int16_t* nextposY)
{
  
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
* SET BOUNCY
*********************************************************************/
void Physics::SetBouncy(float inBouncy)
{
   bouncy = inBouncy;
}

/*********************************************************************
* SET GRAVITY
*********************************************************************/
void Physics::SetGravity(float inGravity)
{
   gravity = inGravity;
}
