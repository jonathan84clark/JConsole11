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
#include "Solid.h"


class GameObject
{
   public:
      // Public Functions
      GameObject();
      ~GameObject(); // destructor
      uint8_t Move(int16_t deltaX, int16_t deltaY);
      void PhysicsMove();
      void RotateDown();
      void RotateUp();
      void RotateLeft();
      void RotateRight();
      void SetVelocity(float inXVelocity, float inYVelocity);
      void SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag);
      uint8_t CheckCollision(GameObject* other);
      uint8_t CheckCollision(Solid* other);
      void Disable();
      GameObject(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t* pcolors, uint16_t inBgColor);
      int16_t getXPos() { return xPos; }
      int16_t getYPos() { return yPos; }
      int16_t getWidth() { return width; }
      int16_t getHeight() { return height; }
      Rotation getRotation() { return rotation; }
   
   private:
      Adafruit_ILI9341 *tft;
      Physics physics;
      uint8_t collision_cool_down;
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
      bool active;
};

#endif
