/*******************************************************
* PHYSICS CLASS
* DESC: The physics class handles moving a gameobject in a realistic
* way in the game. It also handles object interactions.
* This class was re-written to work with JConsole11
* Author: Jonathan L Clark
* Date: 5/6/2021
*******************************************************/
#ifndef _PHYSICS_
#define _PHYSICS_


class Physics
{
   public:
       Physics();
       Physics(float inMass, float inFriction, float inGravity);
       void Compute(int16_t posX, int16_t posY, int16_t* nextPosX, int16_t* nextposY);
       void SetVelocity(float inVelX, float inVelY);
       void SetBouncy(float inBouncy);
       void SetGravity(float inGravity);
       bool getEnabled() { return enabled;}
   
   private:
      float friction;
      float velocityX;
      float velocityY;
      float mass;
      float bouncy;
      float gravity;
      bool enabled;
      
};

#endif
