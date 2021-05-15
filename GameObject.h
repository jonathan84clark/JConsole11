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

enum Rotation {UP, DOWN, LEFT, RIGHT};

class GameObject
{
   public:
      // Public Functions
      GameObject();
      ~GameObject(); // destructor
      void Move(int16_t deltaX, int16_t deltaY);
      void PhysicsMove();
      void RotateDown();
      void RotateUp();
      void RotateLeft();
      void RotateRight();
      void SetVelocity(float inXVelocity, float inYVelocity);
      void SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag);
      uint8_t CheckCollision(GameObject* rightObject);
      void ActivateSolid(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inBgColor, uint16_t solidColor);
      GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors, uint16_t inBgColor);
      GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inBgColor, uint16_t solidColor);
   
   private:
      Adafruit_ILI9341 *tft;
      Physics physics;
      uint16_t* original;
      uint16_t* image;
      uint16_t bg_color;
      uint16_t imageSize;
      int16_t xPos;
      int16_t yPos;
      int16_t width;
      int16_t height;
      Rotation rotation;
      bool prevXStopped;
      bool prevYStopped;
      bool isSolid;
      bool active;
};

#endif
