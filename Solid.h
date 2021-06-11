/*******************************************************
* SOLID CLASS
* DESC: The solid object is designed to produce objects of a uniform
* shape and color. These objects use significantly fewer resources than
* the more advanced gameobject
* Author: Jonathan L Clark
* Date: 5/17/2021
*******************************************************/
#ifndef _SOLID_
#define _SOLID_

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Physics.h"

class Solid
{
   public:
      // Public Functions
      Solid();
      Solid(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inBgColor);
      uint8_t Move(int16_t deltaX, int16_t deltaY);
      void PhysicsMove(unsigned long milliseconds);
      void RotateDown();
      void RotateUp();
      void RotateLeft();
      void RotateRight();
      void Draw();
      void SetBehavior(bool inDisableOnHit, int16_t inHealth);
      void SetVelocity(float inXVelocity, float inYVelocity);
      void SetPhysics(float inMass, float inFriction, float inGravity, float inBouncy, float inDrag);
      uint8_t CheckCollision(Solid* other);
      uint8_t CheckCollision(Solid* other, int16_t* xIntercept);
      void Disable();
      void Activate(Adafruit_ILI9341 *inTft, int16_t inXPos, int16_t inYPos, int16_t inWidth, int16_t inHeight, uint16_t inColor, uint16_t inBgColor);
      bool getActive() { return active;}
      unsigned long getNextPhysicsTime() { return nextPhysicsTime;}
      int16_t getHeight() { return height; }
      int16_t getWidth() { return width; }
      int16_t getXPos() { return xPos; }
      int16_t getYPos() { return yPos; }
      float getVelocityX() { return physics.getVelocityX();}
      float getVelocityY() { return physics.getVelocityY();}
   
   private:
      Adafruit_ILI9341 *tft;
      Physics physics;
      uint8_t collision_cool_down;
      uint16_t color;
      uint16_t bg_color;
      unsigned long nextPhysicsTime;
      int16_t xPos;
      int16_t yPos;
      int16_t width;
      int16_t height;
      int16_t health;
      Rotation rotation;
      bool prevXStopped;
      bool prevYStopped;
      bool active;
      bool disableOnHit;
};

#endif
