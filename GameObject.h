/*******************************************************
* GAMEOBJECT CLASS
* DESC: The game object class is designed to provide a framework 
* for creating basic gameobjects using the ILI9341 LCD library
* Author: Jonathan L Clark
* Date: 5/2/2021
*******************************************************/
#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Physics.h"

class GameObject
{
   public:
      // Public Functions
      GameObject();
      void Move(int16_t deltaX, int16_t deltaY);
      void PhysicsMove();
      void SetVelocity(float inXVelocity, float inYVelocity);
      void SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag);
      GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors);
   
   private:
      Adafruit_ILI9341 *tft;
      Physics physics;
      uint16_t* image;
      int16_t xPos;
      int16_t yPos;
      int16_t width;
      int16_t height;
      bool prevXStopped;
      bool prevYStopped;
};

#endif
